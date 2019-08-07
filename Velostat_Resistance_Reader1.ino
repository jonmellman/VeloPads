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

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  for (int i = 0; i < NUM_PINS; i++) {
    float resistance = readResistanceFromPin(analogPins[i]);
    Serial.print(formatOutput(analogPins[i], resistance));
    Serial.print("   ");
  }
  
  Serial.println();

  delay(100);
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
