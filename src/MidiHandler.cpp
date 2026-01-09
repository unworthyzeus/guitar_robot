#include "MidiHandler.h"
#include "HAL.h"
#include "Config.h"
#include "Tunings.h"
#include <BLEMidi.h>

// Define statics
TuningType TuningManager::currentTuning = TUNING_STANDARD;

void MidiHandler::init() {
    BLEMidiServer.begin(MIDI_NAME);
    BLEMidiServer.setOnPlayStatus([](bool playing){
        Serial.printf("MIDI Status: %s\n", playing ? "Playing" : "Stopped");
    });
    
    BLEMidiServer.setNoteOnCallback(onNoteOn);
    BLEMidiServer.setNoteOffCallback(onNoteOff);
}

void MidiHandler::loop() {
    // BLEMidi handles itself mostly in background/callbacks
}

// Smart Mapping: Tries to fit the note into the CURRENT Bar first.
void mapNote(uint8_t note, int8_t* outString, int8_t* outFret) {
    *outString = -1;
    *outFret = 0;

    int tempNote = note;
    while (tempNote < 40) tempNote += 12;
    while (tempNote > 68) tempNote -= 12;

    const uint8_t* baseNotes = TuningManager::getBaseNotes();
    
    // 1. SMART CHECK: Can we play this on the CURRENT Bar?
    // This prevents the bar from jumping if the note is available right here.
    int8_t currentBar = HAL::getGlobalFret(); // We need to add this getter to HAL!
    
    for (int s = 0; s < NUM_STRINGS; s++) {
        int diff = tempNote - baseNotes[s];
        if (diff == currentBar) { // Perfect match for current bar!
            *outString = s;
            *outFret = diff;
            return;
        }
    }

    // 2. Standard Search (Find anywhere valid)
    for (int s = 0; s < NUM_STRINGS; s++) {
        int diff = tempNote - baseNotes[s];
        if (diff >= 0 && diff <= MAX_FRETS) {
            *outString = s;
            *outFret = diff;
            return; 
        }
    }
}

void MidiHandler::onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (velocity == 0) {
        onNoteOff(channel, note, 0);
        return;
    }
    
    int8_t stringIdx, fretNum;
    mapNote(note, &stringIdx, &fretNum);
    
    if (stringIdx != -1) {
        Serial.printf("NoteOn: %d -> String %d, Bar %d\n", note, stringIdx, fretNum);
        
        // 1. Move the Global Bar (if needed)
        // This affects ALL strings
        HAL::setGlobalFret(fretNum);
        
        // 2. Pluck the specific string
        HAL::pluckString(stringIdx);
    } else {
        Serial.printf("NoteOn: %d -> Out of Range\n", note);
    }
}

void MidiHandler::onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
    // In this architecture, we don't really "release" a note individually by lifting a finger
    // because that would kill the barre for everyone else.
    // We also can't stop the string vibration easily without a damper.
    // So NoteOff is largely ignored or just logged.
    // The sound decays naturally.
}
