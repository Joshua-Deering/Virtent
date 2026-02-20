#include <Arduino.h>
#include <cstdint>

#include "Ble_HID.hpp"
#include "Motor_Control.hpp"
#include "PinDefinitions.hpp"
#include "config.hpp"
#include "ssi.hpp"

enum RunState {
    HIBERNATE,
    SLEEP,
    ACTIVE,
};
RunState cur_state = RunState::ACTIVE;

BleHID transport;

ssi::EncoderData current_scrollpos;
ssi::EncoderData last_scrollpos;

uint32_t cur_detent = 0;
uint32_t num_detents = 10;
double cur_detent_angle = 0.0;

unsigned long step_delay_us = 10000;
MotorControl motor;

unsigned long last_wake = 0;

void setPowerMode(RunState state) {
    if (state == RunState::ACTIVE) {
        setCpuFrequencyMhz(240);
    } else if (state == RunState::SLEEP) {
        setCpuFrequencyMhz(80);
    }

    motor.set_en(true);
}

void setup() {
    Serial.begin(115200);
    pinMode(pin::LED1, OUTPUT);
    pinMode(pin::LED2, OUTPUT);
    pinMode(pin::LED3, OUTPUT);
    pinMode(pin::LED4, OUTPUT);

    pinMode(pin::SSI_CSN, OUTPUT);
    pinMode(pin::SSI_CLK, OUTPUT);
    pinMode(pin::SSI_DO, INPUT);
    pinMode(pin::PUSH, INPUT);

    pinMode(pin::CHG_CHG, INPUT_PULLUP);
    pinMode(pin::CHG_PGOOD, INPUT_PULLUP);

    pinMode(pin::TMC_DIAG, INPUT);

    // enable votage booster
    pinMode(pin::TMC_PWR_EN, OUTPUT);
    digitalWrite(pin::TMC_PWR_EN, LOW);

    setPowerMode(RunState::ACTIVE);
    motor.setup();
    // transport.begin();

    // status led
    digitalWrite(pin::LED1, HIGH);

    // last_wake = millis();
    // last_scrollpos = ssi::get_next(pin::SSI_CSN, pin::SSI_CLK, pin::SSI_DO);
}

bool disabled = false;

void loop() {
    // digitalWrite(pin::LED2, digitalRead(pin::TMC_DIAG) ? HIGH : LOW);
    // digitalWrite(pin::LED2, digitalRead(pin::CHG_CHG) ? LOW : HIGH);

    // Read sensor
    current_scrollpos = ssi::get_next(pin::SSI_CSN, pin::SSI_CLK, pin::SSI_DO);
    delay(50);

    //   delayMicroseconds(step_delay_us);
    //   motor.spin_open_loop(20);

    //   if (step_delay_us > 4000) {
    //     step_delay_us -= 10;
    //   }

    // Check for movement
    bool moved = abs(current_scrollpos.angle - last_scrollpos.angle) > CONFIG::SCROLL_WAKE_THRESHOLD;

    if (moved) {
        last_scrollpos = current_scrollpos;
        last_wake = millis(); // Reset idle timer

        if (cur_state != RunState::ACTIVE) {
            // If we were in hibernate, wake bluetooth
            if (cur_state == RunState::HIBERNATE) {
                Serial.println("Wake up detected! Reconnecting Bluetooth...");
                transport.begin(); // reconnect bt
            }
            setPowerMode(RunState::ACTIVE);
            cur_state = RunState::ACTIVE;
        }
    }

    switch (cur_state) {
    case ACTIVE:
        // do pid stuff, send scroll events
        if (millis() - last_wake > CONFIG::ACTIVE_TO_SLEEP_TIMEOUT) {
            cur_state = RunState::SLEEP;
            setPowerMode(RunState::SLEEP);
            Serial.println("Entering sleep mode...");
        }
        
        //run pid control
        

        delayMicroseconds(CONFIG::ACTIVE_DELAY);
        break;
    case SLEEP:
        if (millis() - last_wake > CONFIG::SLEEP_TO_HIBERNATE_TIMEOUT) {
            Serial.println("Entering Hibernation, disconnecting bluetooth...");
            transport.end();
            cur_state = RunState::HIBERNATE;
        }

        delay(CONFIG::SLEEP_INTERVAL);
        break;
    case HIBERNATE:

        esp_sleep_enable_timer_wakeup(CONFIG::HIBERNATE_INTERVAL);
        esp_light_sleep_start();
        break;
    }
}