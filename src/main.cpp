#include <Arduino.h>
#include <MIDI.h>
#include <MIDIUSB.h>

//MIDI_CREATE_DEFAULT_INSTANCE();

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}


void setup() {
  //Serial.begin(9600);
  Serial.begin(115200);
  pinMode(A0, INPUT);
}

int velocity;

void loop() {
  // Send note 42 with velocity 127 on channel 1
  velocity = analogRead(A0);
  velocity = velocity>>3;

  // noteOn(1,50,velocity);
  // MidiUSB.flush();
  // delay(100);
  // noteOff(1,50,velocity);
  // MidiUSB.flush();
  // delay(250);
  controlChange(5,16,velocity);
  MidiUSB.flush();
  delay(50);
}

