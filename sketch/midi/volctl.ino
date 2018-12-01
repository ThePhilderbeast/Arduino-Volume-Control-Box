#include "MIDIUSB.h"

// Connection Baud Rate
#define SERIAL_RATE 115200

//MIDI channel to use
#define MIDI_CHANNEL 0
//CC slots 14-31 are recognized as assignable controls by MIDI standard
#define MIDI_CC_START 14
//total channels
#define MIDI_CHANNELS 3
//minimum change
#define MIN_CHANGE 1

//DEFINE THE PINS TO USE
byte faderpins[] = {A2, A1, A0};
byte ledpins[] = {2, 4, 6};
byte buttonPins[] = {3, 5, 7};

#define PRESS_DELAY 250;

long lastPushTime[MIDI_CHANNELS];
bool ledState[MIDI_CHANNELS];

bool mutestate[MIDI_CHANNELS];
byte midiValue[MIDI_CHANNELS];

unsigned long currentMillis;

void setup() {
    Serial.begin(SERIAL_RATE);

    //set pin modes
    
    for(byte led : ledpins)
    {
        pinMode(led, OUTPUT);
    }
    
    for(byte button : buttonPins)
    {
        pinMode(button, INPUT_PULLUP);
    }

}

void muteOn(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
    MidiUSB.sendMIDI(noteOn);
}

void muteOff(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
    MidiUSB.sendMIDI(noteOff);
}

void faderChange(byte channel, byte control, byte value) {
    midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
    MidiUSB.sendMIDI(event);
    MidiUSB.flush();
}

byte getFaderMidiVal(int faderVal)
{
    int midival = (faderVal / 1024.0) * 128;
    return midival;
}

void loop() {

    currentMillis = millis();

    for (byte channel = 0; channel < MIDI_CHANNELS; channel++)
    // for(byte fader : faderpins)
    {
        byte newval = getFaderMidiVal(analogRead(faderpins[channel]));
        // if (abs(midiValue[channel] - newval) > MIN_CHANGE)
        if (midiValue[channel] != newval)
        {
            midiValue[channel] = newval;
            Serial.println(newval);
            faderChange(MIDI_CHANNEL, MIDI_CC_START + channel, newval);
        }

        if (!digitalRead(buttonPins[channel]) && lastPushTime[channel] < currentMillis) {
            lastPushTime[channel] = currentMillis + PRESS_DELAY;
            // turn LED on:
            ledState[channel] = !ledState[channel];
            faderChange(MIDI_CHANNEL, MIDI_CC_START + channel + MIDI_CHANNELS, 127);
            digitalWrite(ledpins[channel], ledState[channel]);
        }

    }
}