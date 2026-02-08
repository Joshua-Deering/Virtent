#pragma once

#include <cstdint>

namespace ssi {

    struct EncoderData {
        double angle;
        uint8_t status;
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
                data = (data << 1) | digitalRead(DO_PIN);
            }
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
        out.status = (uint8_t)raw_status;

        // end angle read cycle
        digitalWrite(CSN_PIN, HIGH);

        return out;
    }

}