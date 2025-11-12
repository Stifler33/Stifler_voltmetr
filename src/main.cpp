#include <Arduino.h>
#include <esp_adc_cal.h>
#include <driver/adc.h>

const int resolution = 512;
int raw_buffer[resolution];
int count_raw=0;

int pin_analog = 27;
void setup() {
  Serial.begin(115200);
  analogSetPinAttenuation(pin_analog, ADC_0db);
}

float result = 0.0;
int analog_intermediate = 0;

void loop() {  
  count_raw++;
  raw_buffer[count_raw] = analogRead(pin_analog);
  
  if (count_raw > resolution){
    count_raw = 0;
    analog_intermediate = 0;
    for (int i = 0; i < resolution; i++){
      analog_intermediate += raw_buffer[i];
    }
    analog_intermediate = analog_intermediate / resolution;
    result = analog_intermediate * 1.1 / 4095;
  }
  // Serial.println(result);
  Serial.println(result);
  // delay(100);
}
