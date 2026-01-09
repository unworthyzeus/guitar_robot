#include "HAL.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Concrete Driver Wrapper
class PCA9685Driver : public IDriver {
    Adafruit_PWMServoDriver _pwm;
public:
    PCA9685Driver(uint8_t addr) : _pwm(addr) {}
    
    void init() override {
        Wire.begin(I2C_SDA, I2C_SCL);
        _pwm.begin();
        _pwm.setOscillatorFrequency(27000000);
        _pwm.setPWMFreq(50); // Servo standard 50Hz
    }
    
    void setPWM(uint8_t channel, uint16_t on, uint16_t off) override {
        _pwm.setPWM(channel, on, off);
    }
};

// Static Members
IDriver* HAL::_driver = nullptr;
uint8_t HAL::_currentBar = 0;
unsigned long HAL::_pluckTimers[6] = {0};
bool HAL::_pluckActive[6] = {false};

// Config for actuation duration
#define PLUCK_DURATION_MS 100 

void HAL::init() {
    static PCA9685Driver pca(PCA_ADDR);
    _driver = &pca;
    _driver->init();
    
    // Reset all mechanims to REST position
    setGlobalFret(0); // Lift all bars
    for(int i=0; i<NUM_STRINGS; i++) {
        moveServo(i, PLUCK_REST); // Center pluckers
        _pluckActive[i] = false;
    }
}

void HAL::loop() {
    // Non-blocking logic: Check if we need to retract any plucker
    unsigned long now = millis();
    
    for(int i=0; i<NUM_STRINGS; i++) {
        if (_pluckActive[i]) {
            if (now - _pluckTimers[i] >= PLUCK_DURATION_MS) {
                // Time's up! Return to rest.
                moveServo(i, PLUCK_REST);
                _pluckActive[i] = false;
            }
        }
    }
}

void HAL::moveServo(uint8_t channel, uint16_t value) {
    if (_driver) {
        _driver->setPWM(channel, 0, value);
    }
}

void HAL::setGlobalFret(uint8_t fretNum) {
    // Avoid redundant moves
    if (_currentBar == fretNum) return;
    
    // Logic:
    // If fretNum > 0: Lower that bar (Press string). Lift others.
    // If fretNum == 0: Lift ALL bars (Open strings).
    
    // 1. Lift currently active bar if different
    if (_currentBar != 0 && _currentBar != fretNum) {
        // Map Bar 1..4 to Channels 6..9
        uint8_t oldCh = 5 + _currentBar; 
        moveServo(oldCh, BAR_UP); 
    }
    
    // 2. Press new bar
    if (fretNum > 0) {
        uint8_t newCh = 5 + fretNum;
        moveServo(newCh, BAR_DOWN);
    }
    
    _currentBar = fretNum;
}

uint8_t HAL::getGlobalFret() {
    return _currentBar;
}

void HAL::pluckString(uint8_t stringIdx) {
    if (stringIdx >= NUM_STRINGS) return;
    
    // Non-blocking Pluck
    // 1. Move to HIT
    moveServo(stringIdx, PLUCK_HIT);
    
    // 2. Record Time
    _pluckActive[stringIdx] = true;
    _pluckTimers[stringIdx] = millis();
    
    // 3. Return immediately! (Retraction happens in loop())
}
