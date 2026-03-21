/**
 * @file Arduino.h
 * @brief Minimal shim so DAQv2-Comms compiles on Linux/server without the Arduino SDK.
 *
 * Only provides millis() (used by create_* serialization functions).
 * Parse functions don't need millis() at all.
 */
#pragma once

#include <cstdint>
#include <chrono>

inline uint32_t millis() {
    return static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch())
            .count() & 0xFFFFFFFF);
}
