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

void HAL::init() {
    static PCA9685Driver pca(PCA_ADDR);
    _driver = &pca;
    _driver->init();
    
    // Reset all mechanims to REST position
    setGlobalFret(0); // Lift all bars
    for(int i=0; i<NUM_STRINGS; i++) {
        moveServo(i, PLUCK_REST); // Center pluckers
    }
}

void HAL::loop() {
    // If we need non-blocking movement logic later (e.g. slow moves), add here
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

void HAL::pluckString(uint8_t stringIdx) {
    if (stringIdx >= NUM_STRINGS) return;
    
    // Pluck Logic: Center -> Hit -> Center
    // NOTE: This blocks for 100ms. In a real advanced RTOS code we wouldn't block,
    // but for Arduino loop simple logic, it's safer to ensure full movement.
    
    moveServo(stringIdx, PLUCK_HIT);
    delay(100); // Wait for hit
    moveServo(stringIdx, PLUCK_REST); // Return
}
