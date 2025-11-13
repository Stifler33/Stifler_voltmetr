#include <Arduino.h>
// #include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include "stifler_voltmetr.h"

Stifler_voltmetr metr;
void setup(void)
{  
  Serial.begin(115200);
  Serial.println("Hello!");
  Wire1.begin(5, 18, 100);

  if (!metr.begin(0, 19, 72U, &Wire1)) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
  
  metr.set_delta(30200);
}

float voltage = 0;
void loop(void)
{  
  Serial.print(metr.read_voltage(&voltage));
  Serial.print(" ");
  Serial.print(voltage);
  Serial.println(" V");

  delay(100);
}