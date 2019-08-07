#include <pitchToFrequency.h>
#include <pitchToNote.h>
#include <frequencyToNote.h>
#include <MIDIUSB_Defs.h>
#include <MIDIUSB.h>

const int NUM_PINS = 2;
const int analogPins[NUM_PINS] = {0, 1};
const int Vin = 5;
const float REFERENCE_RESISTANCE_OHMS = 10000; // Value of the reference resistor on the breadboard.
const float MAX_RESISTANCE_OHMS = 8000; // Arbitrary, but somewhat calibrated to R1 of 10000

int playing = 0;
int velocity = 0;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
//  for (int i = 0; i < NUM_PINS; i++) {
//    float resistance = readResistanceFromPin(analogPins[i]);
//    Serial.print(formatOutput(analogPins[i], resistance));
//    Serial.print("   ");
//  }

  float resistance = readResistanceFromPin(analogPins[0]);

  if (!playing && resistance < 600) {
    playing = 1;
    noteOn(0, 48, 127);
    Serial.println("note on");
    Serial.println(resistance);
    delay(5);
  } else if (playing && resistance > 700) {
    playing = 0;
    noteOff(0, 48, 0);
    delay(5);
  }
}

float readResistanceFromPin(int pinNumber)
{
  float rawAnalogInput = analogRead(pinNumber);

  if (rawAnalogInput == 0) {
    return MAX_RESISTANCE_OHMS;
  }

  float buffer = rawAnalogInput * Vin;
  float Vout = buffer / 1023.0;
  
  buffer = (Vin/Vout) - 1;
  
  float outputResistanceOhms = REFERENCE_RESISTANCE_OHMS * buffer;
  outputResistanceOhms = min(outputResistanceOhms, MAX_RESISTANCE_OHMS); // Clamp output value

  return outputResistanceOhms;
}

String formatOutput(int analogPin, float resistance)
{
//  return String(resistance);
  return String(analogPin) + ": " + String(resistance);
}


// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second paraxmeter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}
