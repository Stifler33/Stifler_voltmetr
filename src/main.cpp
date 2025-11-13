#include "stifler_voltmetr.h"
#include "Arduino.h"

Stifler_voltmetr voltmetr;

void setup(){
  Wire1.begin(5, 18, 100);
  voltmetr.begin(0, 19, 72U, &Wire1);  
  voltmetr.set_delta(30200);
  Serial.begin(115200);
}

void loop(){
  Serial.println(voltmetr.read_voltage());
  delay(100);
}
