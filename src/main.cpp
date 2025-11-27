#include <Arduino.h>
#include <stifler_spiffs.h>

Voltage_map volt_map;

void setup(){    
  Serial.begin(115200);
  Serial.println("hello");  
  delay(3000);
  volt_map.init();
  
}

void loop(){

}
