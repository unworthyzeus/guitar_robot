#pragma once
#include <Arduino.h>
#include <stdint.h>
#include "Config.h"

/**
 * @brief Abstract Base Class for Driver
 */
class IDriver {
public:
    virtual void init() = 0;
    virtual void setPWM(uint8_t channel, uint16_t on, uint16_t off) = 0;
    virtual ~IDriver() {}
};

/**
 * @brief HAL for 10-Motor Barre Chord Architecture
 */
class HAL {
public:
    static void init();
    static void loop(); // Use loop instead of update for consistency

    // Bar Control (Frets 1-4)
    // fretNum: 1 to 4. 0 = Release all bars.
    static void setGlobalFret(uint8_t fretNum);

    // Plucker Control (Strings 0-5)
    static void pluckString(uint8_t stringIdx);

private:
    static IDriver* _driver;
    static uint8_t _currentBar; // Tracks which bar is currently down
    
    // Helper to move servo
    static void moveServo(uint8_t channel, uint16_t value);
};
