#pragma once

#include <cstdint>

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
        digitalWrite(EN, en ? HIGH : LOW);
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

    const uint8_t EN = 18;

    const uint8_t UH = 13;
    const uint8_t UL = 14;
    const uint8_t VH = 27;
    const uint8_t VL = 26;
    const uint8_t WH = 25;
    const uint8_t WL = 33;

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
        pinMode(EN, OUTPUT);

        pinMode(UH, OUTPUT);
        pinMode(UL, OUTPUT);
        pinMode(VH, OUTPUT);
        pinMode(VL, OUTPUT);
        pinMode(WH, OUTPUT);
        pinMode(WL, OUTPUT);
    }

    void setup_pwm() {
        // Configure timer implicitly via channel setup
        ledcAttach(UH, PWM_FREQ, PWM_RES);
        ledcAttach(VH, PWM_FREQ, PWM_RES);
        ledcAttach(WH, PWM_FREQ, PWM_RES);

        // Start with 0 duty
        ledcWrite(UH, 0);
        ledcWrite(VH, 0);
        ledcWrite(WH, 0);
    }

    void set_U_high(uint8_t duty) {
        digitalWrite(UL, LOW);
        delayMicroseconds(2);
        ledcWrite(UH, duty);
    }

    void set_U_low() {
        ledcWrite(UH, 0);
        delayMicroseconds(2);
        digitalWrite(UL, HIGH);
    }

    void set_U_floating() {
        ledcWrite(UH, 0);
        digitalWrite(UL, LOW);
    }

    void set_V_high(uint8_t duty) {
        digitalWrite(VL, LOW);
        delayMicroseconds(2);
        ledcWrite(VH, duty);
    }

    void set_V_low() {
        ledcWrite(VH, 0);
        delayMicroseconds(2);
        digitalWrite(VL, HIGH);
    }

    void set_V_floating() {
        ledcWrite(VH, 0);
        digitalWrite(VL, LOW);
    }

    void set_W_high(uint8_t duty) {
        digitalWrite(WL, LOW);
        delayMicroseconds(2);
        ledcWrite(WH, duty);
    }

    void set_W_low() {
        ledcWrite(WH, 0);
        delayMicroseconds(2);
        digitalWrite(WL, HIGH);
    }

    void set_W_floating() {
        ledcWrite(WH, 0);
        digitalWrite(WL, LOW);
    }

    void all_phases_off() {
        ledcWrite(UH, 0);
        ledcWrite(VH, 0);
        ledcWrite(WH, 0);
        digitalWrite(UL, LOW);
        digitalWrite(VL, LOW);
        digitalWrite(WL, LOW);
    }
};