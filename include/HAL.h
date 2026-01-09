#pragma once
#include <Arduino.h>
#include <stdint.h>

enum ActuatorType {
    ACTUATOR_SOLENOID,
    ACTUATOR_SERVO
};

/**
 * @brief Abstract Base Class for any driver (PCA9685, Shift Register, etc.)
 */
class IDriver {
public:
    virtual void init() = 0;
    virtual void setChannel(uint8_t channel, uint16_t value) = 0; // value: 0-4096 for PWM, or 0/1 for digital
    virtual ~IDriver() {}
};

/**
 * @brief Represents a physical string on the guitar.
 * Handles both the picking mechanism and the fretting mechanism.
 */
class GuitarString {
public:
    GuitarString(uint8_t stringId);
    
    void setDriver(IDriver* driver);
    
    // Actions
    void pluck();
    void setFret(int8_t fretNumber); // -1 or 0 for open, 1-N for specific frets
    void releaseFret();
    
    // Lifecycle
    void update();

private:
    uint8_t _id;
    int8_t _currentFret;
    IDriver* _driver;
    
    // Non-blocking actuation state
    bool _isPlucking;
    unsigned long _pluckStartTime;
    
    // Mapping helpers (to be defined in Config or implementation)
    uint8_t getFretChannel(int8_t fret);
    uint8_t getPluckChannel();
};

class HAL {
public:
    static void init();
    static void update(); // Call in main loop
    static GuitarString* getString(uint8_t index);
    
private:
    static GuitarString* _strings[6];
    static IDriver* _mainDriver; // Assuming one main driver for now
};
