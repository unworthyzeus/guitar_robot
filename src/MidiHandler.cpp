#include "MidiHandler.h"
#include "HAL.h"
#include "Config.h"
#include <BLEMidi.h>

// Standard Tuning reference (MIDI Note numbers)
// E2=40, A2=45, D3=50, G3=55, B3=59, E4=64
const uint8_t STRING_BASE_NOTES[6] = {40, 45, 50, 55, 59, 64};

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

// Simple Mapping: Returns String Index and Fret Number
// Returns -1 in stringIndex if not found.
void mapNote(uint8_t note, int8_t* outString, int8_t* outFret) {
    *outString = -1;
    *outFret = 0;

    // 1. Octave Folding (Fit into range)
    int tempNote = note;
    while (tempNote < 40) tempNote += 12;
    while (tempNote > 68) tempNote -= 12;

    // 2. Find best string
    // Strategy: Prefer frets 0-4.
    for (int s = 0; s < NUM_STRINGS; s++) {
        int diff = tempNote - STRING_BASE_NOTES[s];
        if (diff >= 0 && diff <= MAX_FRETS) {
            *outString = s;
            *outFret = diff;
            return; // Take first validity
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
