#include <stifler_wifi.h>
#include <stifler_mqtt.h>
#include <charger_test_board.h>
#include "stifler_voltmetr.h"

// void relay_switch(String message){
//   if (message == "on"){
//     digitalWrite(PIN_RELAY, HIGH);
//     public_data("relay_status", "on");
//   }
//   if (message == "off"){
//     digitalWrite(PIN_RELAY, LOW);
//     public_data("relay_status", "off");
//   }
// }

void setup(){    

  Wire1.begin(5, 18, 100);
  voltmetr.begin(0, 19, 72U, &Wire1);  
  voltmetr.set_delta(30200);
  // Serial.begin(115200);

  set_pin_reset(0, LOW);
  init_stif();
  arduino_ota_initial();
  add_pub_topic("connect", "esp32/connect");    
  add_sub_topic("relay_end", "charger/test/rl_end", switch_end_relay);
  add_sub_topic("relay_pu", "charger/test/rl_pu", switch_pu_relay);
  add_sub_topic("relay_plus", "charger/test/rl_plus", switch_plus_relay);
  add_sub_topic("relay_minus", "charger/test/rl_minus", switch_minus_relay);
  add_sub_topic("cc", "charger/test/cc", CC);
  add_sub_topic("cv", "charger/test/cv", CV);
  init_brocker();
  init_output();
}

void loop(){
    if (loop_status_wifi()){
        loop_mqtt();
    }
    loop_relay();



// void loop(){
//   Serial.println(voltmetr.read_voltage());
//   delay(100);
// }
