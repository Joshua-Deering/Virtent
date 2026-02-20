#pragma once

#include <cstdint>

#include "PinDefinitions.hpp"

class MotorControl {
public:

    MotorControl() {}

    void setup() {
        set_motor_pin_modes();
        setup_pwm();
    }

    // for power saving modes
    void set_en(bool en) {
        enabled = en;
        digitalWrite(pin::TMC_EN, en ? HIGH : LOW);
    }

    void spin_open_loop(uint8_t pwm_strength) {
        static int current_step = 1;
        
        do_motor_step(current_step, pwm_strength);
        
        current_step++;
        if (current_step > 6) current_step = 1;
    }

    void stop() {
        all_phases_off();
    }

private:
    bool enabled = false;

    // PWM parameters
    const uint32_t PWM_FREQ = 20000; // 20 kHz
    const uint8_t PWM_RES = 8;       // 8-bit resolution
    const uint8_t PWM_MAX = 255;
    const uint8_t PWM_LIMIT = 128;     // 50% pwm

    void do_motor_step(int cur_step, uint8_t pwm) {
        if (!enabled) return;

        if (pwm > PWM_LIMIT)
            pwm = PWM_LIMIT;

        switch (cur_step) {
        case 1:
            set_W_floating();
            set_V_low();
            set_U_high(pwm);
            break;
        case 2:
            set_V_floating();
            set_W_low();
            set_U_high(pwm);
            break;
        case 3:
            set_U_floating();
            set_W_low();
            set_V_high(pwm);
            break;
        case 4:
            set_W_floating();
            set_U_low();
            set_V_high(pwm);
            break;
        case 5:
            set_V_floating();
            set_U_low();
            set_W_high(pwm);
            break;
        case 6:
            set_U_floating();
            set_V_low();
            set_W_high(pwm);
            break;
        default:
            all_phases_off();
        }
    }

    void set_motor_pin_modes() {
        pinMode(pin::TMC_EN, OUTPUT);

        pinMode(pin::TMC_UH, OUTPUT);
        pinMode(pin::TMC_UL, OUTPUT);
        pinMode(pin::TMC_VH, OUTPUT);
        pinMode(pin::TMC_VL, OUTPUT);
        pinMode(pin::TMC_WH, OUTPUT);
        pinMode(pin::TMC_WL, OUTPUT);
    }

    void setup_pwm() {
        // Configure timer implicitly via channel setup
        ledcAttach(pin::TMC_UH, PWM_FREQ, PWM_RES);
        ledcAttach(pin::TMC_VH, PWM_FREQ, PWM_RES);
        ledcAttach(pin::TMC_WH, PWM_FREQ, PWM_RES);

        // Start with 0 duty
        ledcWrite(pin::TMC_UH, 0);
        ledcWrite(pin::TMC_VH, 0);
        ledcWrite(pin::TMC_WH, 0);
    }

    void set_U_high(uint8_t duty) {
        digitalWrite(pin::TMC_UL, LOW);
        delayMicroseconds(2);
        ledcWrite(pin::TMC_UH, duty);
    }

    void set_U_low() {
        ledcWrite(pin::TMC_UH, 0);
        delayMicroseconds(2);
        digitalWrite(pin::TMC_UL, HIGH);
    }

    void set_U_floating() {
        ledcWrite(pin::TMC_UH, 0);
        digitalWrite(pin::TMC_UL, LOW);
    }

    void set_V_high(uint8_t duty) {
        digitalWrite(pin::TMC_VL, LOW);
        delayMicroseconds(2);
        ledcWrite(pin::TMC_VH, duty);
    }

    void set_V_low() {
        ledcWrite(pin::TMC_VH, 0);
        delayMicroseconds(2);
        digitalWrite(pin::TMC_VL, HIGH);
    }

    void set_V_floating() {
        ledcWrite(pin::TMC_VH, 0);
        digitalWrite(pin::TMC_VL, LOW);
    }

    void set_W_high(uint8_t duty) {
        digitalWrite(pin::TMC_WL, LOW);
        delayMicroseconds(2);
        ledcWrite(pin::TMC_WH, duty);
    }

    void set_W_low() {
        ledcWrite(pin::TMC_WH, 0);
        delayMicroseconds(2);
        digitalWrite(pin::TMC_WL, HIGH);
    }

    void set_W_floating() {
        ledcWrite(pin::TMC_WH, 0);
        digitalWrite(pin::TMC_WL, LOW);
    }

    void all_phases_off() {
        ledcWrite(pin::TMC_UH, 0);
        ledcWrite(pin::TMC_VH, 0);
        ledcWrite(pin::TMC_WH, 0);
        digitalWrite(pin::TMC_UL, LOW);
        digitalWrite(pin::TMC_VL, LOW);
        digitalWrite(pin::TMC_WL, LOW);
    }
};