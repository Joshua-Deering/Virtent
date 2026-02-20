#pragma once

namespace pin {

    // ssi/sensor pins
    constexpr int SSI_CSN = 4;
    constexpr int SSI_CLK = 5;
    constexpr int SSI_DO = 6;
    constexpr int PUSH = 8;

    // led pins
    constexpr int LED1 = 1;
    constexpr int LED2 = 2;
    constexpr int LED3 = 15;
    constexpr int LED4 = 16;

    // motor control pins
    constexpr int TMC_DIAG = 9;
    constexpr int TMC_EN = 10;
    constexpr int TMC_UH = 11;
    constexpr int TMC_VH = 12;
    constexpr int TMC_WH = 13;
    constexpr int TMC_UL = 14;
    constexpr int TMC_VL = 17;
    constexpr int TMC_WL = 18;

    // motor power IC pins
    constexpr int TMC_PWR_EN = 21;

    // battery management IC pins
    constexpr int CHG_PGOOD = 36;
    constexpr int CHG_CHG = 35;

}