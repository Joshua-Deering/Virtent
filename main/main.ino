#include <Arduino.h>
#include <cstdint>

#include "Ble_HID.hpp"
#include "config.hpp"
#include "ssi.hpp"
#include "Motor_Control.hpp"

#define LED_PIN 2
// ssi pins
#define SSI_CSN 6
#define SSI_CLK 5
#define SSI_DO 4

enum RunState {
    HIBERNATE,
    SLEEP,
    ACTIVE,
};
RunState cur_state = RunState::ACTIVE;

BleHID transport;

ssi::EncoderData current_scrollpos;
ssi::EncoderData last_scrollpos;

unsigned long step_delay_us = 10000;
MotorControl motor;

unsigned long last_wake = 0;

void setPowerMode(RunState state) {
    if (state == RunState::ACTIVE) {
        setCpuFrequencyMhz(240);
    } else if (state == RunState::SLEEP) {
        setCpuFrequencyMhz(80);
    }

    motor.setup();
    motor.set_en(true);
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    setPowerMode(RunState::ACTIVE);
    transport.begin();

    last_wake = millis();
    last_scrollpos = ssi::get_next(SSI_CSN, SSI_CLK, SSI_DO);
}

bool disabled = false;

void loop() {
    // // Read sensor
    // current_scrollpos = ssi::get_next(SSI_CSN, SSI_CLK, SSI_DO);

    // // Check for movement
    // bool moved = abs(current_scrollpos.angle - last_scrollpos.angle) > CONFIG::SCROLL_WAKE_THRESHOLD;

    // if (moved) {
    //     last_scrollpos = current_scrollpos;
    //     last_wake = millis(); // Reset idle timer

    //     if (cur_state != RunState::ACTIVE) {
    //         // If we were in hibernate, wake bluetooth
    //         if (cur_state == RunState::HIBERNATE) {
    //             Serial.println("Wake up detected! Reconnecting Bluetooth...");
    //             transport.begin(); // reconnect bt
    //         }
    //         setPowerMode(RunState::ACTIVE);
    //         cur_state = RunState::ACTIVE;
    //     }
    // }

    // switch (cur_state) {
    // case ACTIVE:
    //     // do pid stuff, send scroll events
    //     if (millis() - last_wake > CONFIG::ACTIVE_TO_SLEEP_TIMEOUT) {
    //         cur_state = RunState::SLEEP;
    //         setPowerMode(RunState::SLEEP);
    //         Serial.println("Entering sleep mode...");
    //     }

    //     delayMicroseconds(CONFIG::ACTIVE_DELAY);
    //     break;
    // case SLEEP:
    //     if (millis() - last_wake > CONFIG::SLEEP_TO_HIBERNATE_TIMEOUT) {
    //         Serial.println("Entering Hibernation, disconnecting bluetooth...");
    //         transport.end();
    //         cur_state = RunState::HIBERNATE;
    //     }

    //     delay(CONFIG::SLEEP_INTERVAL);
    //     break;
    // case HIBERNATE:

    //     esp_sleep_enable_timer_wakeup(CONFIG::HIBERNATE_INTERVAL);
    //     esp_light_sleep_start();
    //     break;
    // }
}