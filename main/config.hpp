#pragma once
#include <cstdint>

namespace CONFIG {
    static uint64_t HIBERNATE_INTERVAL = 200 * 1000; // us (200ms)
    static uint64_t SLEEP_INTERVAL = 50;             // ms
    static uint64_t ACTIVE_DELAY = 500;              // us (0.5ms)

    static double SCROLL_WAKE_THRESHOLD = 20.0; // deg

    static long ACTIVE_TO_SLEEP_TIMEOUT = 10 * 1000;     // ms
    static long SLEEP_TO_HIBERNATE_TIMEOUT = 120 * 1000; // ms
}