#include <Arduino.h>
#include <MIDI.h>
#include <MIDIUSB.h>

//MIDI_CREATE_DEFAULT_INSTANCE();
#define D0 19
#define D1 20
#define D2 18
#define D3 14
#define SIG 21
#define CHANNELS 10
#define MIDI_CHANNEL 5

int potChannels[] = {0 , 1, 3, 2, 4, 6, 7, 8, 9, 5};
int lastReadings [10];
int midiControls [] = {16, 17};

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

int analogReadFromMultiplexer(int channel)
{
  digitalWrite(D0, channel & 0b0001);
  digitalWrite(D1, channel & 0b0010);
  digitalWrite(D2, channel & 0b0100);
  digitalWrite(D3, channel & 0b1000);
  delay(1);
  return 127 - (analogRead(SIG)>>3);
}


void setup() {
  //Serial.begin(9600);
  Serial.begin(115200);
  pinMode(A3, INPUT);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);

  memset(lastReadings, 0, sizeof(int)*10);
}

int velocity;
int val = 0;

void loop() {
  for(int i=0; i<CHANNELS; i++)
  {
    val = analogReadFromMultiplexer(potChannels[i]);
    if(val != lastReadings[i]){
      controlChange(5,16 + i,val);
      MidiUSB.flush();
    }
    lastReadings[i] = val;
  }
  
  delay(5);
}

