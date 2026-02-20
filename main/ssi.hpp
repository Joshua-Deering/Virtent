#pragma once

#include <cstdint>

namespace ssi {

    enum MagStatus {
        OK,
        TOO_STRONG,
        TOO_WEAK
    };

    struct EncoderData {
        double angle;
        bool loss_of_track;
        bool push;
        MagStatus mag_status;
        // MSB -> LSB
        // bit 3: loss of track
        // bit 2: push
        // bit 1: mag field too weak
        // bit 0: mag field too strong
    };

    EncoderData get_next(int CSN_PIN, int CLK_PIN, int DO_PIN) {
        auto read_bits = [&](int bit_count) -> unsigned int {
            unsigned int data = 0;
            for (int i = 0; i < bit_count; i++) {
                digitalWrite(CLK_PIN, LOW);
                delayMicroseconds(1);
                digitalWrite(CLK_PIN, HIGH);
                delayMicroseconds(1);
                int do_stat = digitalRead(DO_PIN);
                data = (data << 1) | do_stat;
                Serial.print(do_stat);
            }
            Serial.println();
            return data;
        };

        // start angle read cycle
        digitalWrite(CLK_PIN, HIGH);
        digitalWrite(CSN_PIN, LOW);
        delayMicroseconds(1);

        EncoderData out;
        unsigned int raw_angle = read_bits(14);
        unsigned int raw_status = read_bits(4);
        unsigned int raw_crc = read_bits(6);

        out.angle = ((double)raw_angle / 16384.0) * 360.0;
        out.loss_of_track = ((raw_status >> 3) & 1) == 1;
        out.push = ((raw_status >> 2) & 1) == 1;
        if (((raw_status >> 1 ) & 1) == 1) {
            out.mag_status = MagStatus::TOO_WEAK;
        } else if (raw_status & 1 == 1) {
            out.mag_status = MagStatus::TOO_STRONG;
        } else out.mag_status = MagStatus::OK;

        // end angle read cycle
        digitalWrite(CSN_PIN, HIGH);

        return out;
    }

}