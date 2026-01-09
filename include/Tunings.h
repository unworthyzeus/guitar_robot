#pragma once
#include <stdint.h>
#include <Arduino.h>

enum TuningType {
    TUNING_STANDARD,
    TUNING_OPEN_E,
    TUNING_OPEN_G,
    TUNING_DROP_D
};

struct TuningProfile {
    const char* name;
    uint8_t baseNotes[6]; // MIDI notes for strings 1..6 (High E to Low E)
};

// String 0 = High E, String 5 = Low E
// Standard: E4(64), B3(59), G3(55), D3(50), A2(45), E2(40)
static const TuningProfile TUNINGS[] = {
    { "Standard", {64, 59, 55, 50, 45, 40} },
    { "Open E",   {64, 59, 52, 47, 44, 40} }, // E, B, G#, E, B, E
    { "Open G",   {62, 59, 55, 50, 43, 38} }, // D, B, G, D, G, D
    { "Drop D",   {64, 59, 55, 50, 45, 38} }  // E, B, G, D, A, D (Standard but Low E -> D)
};

class TuningManager {
public:
    static TuningType currentTuning;
    
    static const uint8_t* getBaseNotes() {
        return TUNINGS[currentTuning].baseNotes;
    }

    static void setTuning(TuningType t) {
        if(t >= 0 && t <= 3) {
            currentTuning = t;
        }
    }
    
    static const char* getName() {
        return TUNINGS[currentTuning].name;
    }
};

// Define definition in cpp or just header-only for simplicity in this small project?
// For header-only static, we need inline or separate cpp. Let's make it simple extern in cpp.
// Actually, let's just make it a singleton or global instance in .cpp to avoid link errors.
