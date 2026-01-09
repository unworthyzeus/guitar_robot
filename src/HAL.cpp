#include "HAL.h"
#include "Config.h"
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

// --- Concrete Driver Implementation ---
class PCA9685Driver : public IDriver {
public:
    PCA9685Driver(uint8_t addr) : _pwm(Adafruit_PWMServoDriver(addr)) {}

    void init() override {
        _pwm.begin();
        _pwm.setPWMFreq(60); // Analog servos run at ~60 Hz
    }

    void setChannel(uint8_t channel, uint16_t value) override {
        // Value mapping for Servo: 0-180 -> roughly 150-600 pulse length
        // But for abstraction, let's assume value is directly the PWM pulse length (0-4096)
        // If we want to support logic:
        if (value == 0) {
            _pwm.setPWM(channel, 0, 0); // Off
        } else {
             _pwm.setPWM(channel, 0, value);
        }
    }

private:
    Adafruit_PWMServoDriver _pwm;
};

// --- HAL Static Members ---
GuitarString* HAL::_strings[NUM_STRINGS];
// We need two drivers for >16 channels
IDriver* driver1; // 0x40 (Strings 1-3?)
IDriver* driver2; // 0x41 (Strings 4-6?)
// actually, a better abstraction is a "CompositeDriver" or looking up driver by channel.
// keeping it simple: The HAL knows about the hardware topology.

PCA9685Driver pwm1(0x40);
PCA9685Driver pwm2(0x41);

void HAL::init() {
    Wire.begin(I2C_SDA, I2C_SCL);
    
    driver1 = &pwm1;
    driver2 = &pwm2;
    
    driver1->init();
    driver2->init();

    for (int i = 0; i < NUM_STRINGS; i++) {
        _strings[i] = new GuitarString(i);
        // Assign driver based on index ? 
        // For simplicity, let's pass a "Global Virtual Driver" or handled inside GuitarString
        // Actually, let's make GuitarString responsible for mapping logical IDs to (Driver, Channel)
    }
}

void HAL::update() {
    for (int i = 0; i < NUM_STRINGS; i++) {
        if (_strings[i]) _strings[i]->update();
    }
}

GuitarString* HAL::getString(uint8_t index) {
    if (index >= NUM_STRINGS) return nullptr;
    return _strings[index];
}

// --- GuitarString Implementation ---

GuitarString::GuitarString(uint8_t stringId) : _id(stringId), _currentFret(0), _isPlucking(false), _pluckStartTime(0) {
    // _driver will be resolved dynamically or assigned.
}

// Helper to map (String, Fret) -> (Driver*, Channel)
// Scheme:
// String 0-2 -> Driver 1
// String 3-5 -> Driver 2
// Per String:
//   Ch 0: Plucker
//   Ch 1-4: Frets 1-4
void getHardwareMap(uint8_t stringId, uint8_t elementIndex, IDriver** outDriver, uint8_t* outChannel) {
    // elementIndex: 0 = Plucker, 1 = Fret 1, 2 = Fret 2...
    
    if (stringId < 3) {
        *outDriver = &pwm1;
    } else {
        *outDriver = &pwm2;
    }
    
    // 5 channels per string
    uint8_t localStringIndex = stringId % 3;
    uint8_t baseChannel = localStringIndex * 5;
    *outChannel = baseChannel + elementIndex;
}

void GuitarString::pluck() {
    IDriver* drv;
    uint8_t ch;
    getHardwareMap(_id, 0, &drv, &ch); // 0 = Plucker
    
    if (drv) {
        // Start Pluck
        drv->setChannel(ch, SERVO_MAX); // Down
        _isPlucking = true;
        _pluckStartTime = millis();
    }
}

void GuitarString::update() {
    if (_isPlucking) {
        if (millis() - _pluckStartTime >= ACTUATION_TIME_MS) {
            // End Pluck
            IDriver* drv;
            uint8_t ch;
            getHardwareMap(_id, 0, &drv, &ch);
            if (drv) drv->setChannel(ch, SERVO_MIN); // Up
            _isPlucking = false;
        }
    }
}

void GuitarString::setFret(int8_t fretNumber) {
    if (fretNumber < 0) fretNumber = 0; // treat <0 as open
    
    if (_currentFret == fretNumber) return; // No change

    IDriver* drv;
    uint8_t ch;
    
    // Release old fret
    if (_currentFret > 0) {
        getHardwareMap(_id, _currentFret, &drv, &ch);
        if (drv) drv->setChannel(ch, SERVO_MIN); // Lift finger
    }
    
    // Press new fret
    if (fretNumber > 0) {
        if (fretNumber > MAX_FRETS) fretNumber = MAX_FRETS; // Clamp
        getHardwareMap(_id, fretNumber, &drv, &ch);
        if (drv) drv->setChannel(ch, SERVO_MAX); // Press finger
    }
    
    _currentFret = fretNumber;
}

void GuitarString::releaseFret() {
    setFret(0);
}
