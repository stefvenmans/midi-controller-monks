#include <Arduino.h>
#include <MIDI.h>
#include <MIDIUSB.h>

#define D0 19
#define D1 20
#define D2 18
#define D3 14
#define SIG 21
#define CHANNELS 10
#define MIDI_CHANNEL 5

int potChannels[] = {0 , 1, 3, 2, 4, 6, 7, 8, 9, 5};
int prevReadings [10];
int val = 0;

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
  return analogRead(SIG);
}

void setup() {
  Serial.begin(115200);
  pinMode(SIG, INPUT);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);

  memset(prevReadings, 0, sizeof(int)*CHANNELS);
}

void loop() {
  for(int i=0; i<CHANNELS; i++)
  {
    val = analogReadFromMultiplexer(potChannels[i]);
    // Check if analog value changed more than 1 unit (10 bit)
    if(val != prevReadings[i] && val != prevReadings[i]-1 && val != prevReadings[i]+1){
      if(i != 7){
        controlChange(5,16 + i, 127 - (val>>3));
        MidiUSB.flush();
        }
      prevReadings[i] = val;
    }
    
  }
  
  delay(5);
}
