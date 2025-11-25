#include <stifler_wifi.h>
#include <stifler_mqtt.h>
#include <charger_test_board.h>
#include <GyverINA.h>

INA219 ina219(0.01, 32.0);

Stifler_voltmetr voltmetr;
int delta = 0;
GTimer<millis> wait_pub(1000, true);
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

void set_delta(String delta){
  voltmetr.set_delta(delta.toInt());
}

void setup(){

  set_pin_reset(0, LOW);
  init_stif();
  arduino_ota_initial();
  add_pub_topic("connect", "esp32/connect"); 
  add_pub_topic("voltage", "charger/test/voltage");
  add_pub_topic("amperage", "charger/test/amperage");

  add_sub_topic("relay_end", "charger/test/rl_end", switch_end_relay);
  add_sub_topic("relay_pu", "charger/test/rl_pu", switch_pu_relay);
  add_sub_topic("relay_plus", "charger/test/rl_plus", switch_plus_relay);
  add_sub_topic("relay_minus", "charger/test/rl_minus", switch_minus_relay);
  add_sub_topic("delta", "charger/test/set_delta", set_delta);
  add_sub_topic("cc", "charger/test/cc", CC);
  add_sub_topic("cv", "charger/test/cv", CV);
  add_sub_topic("dis", "charger/test/dis", discharge);

  init_brocker();
  init_output();

  Wire.begin(18, 5);
  voltmetr.begin(0, 1);
  voltmetr.set_delta(24420);
}

void loop(){
    if (loop_status_wifi()){
        loop_mqtt();
    }
    loop_relay();    
    if (wait_pub){
      String volt = String(voltmetr.read_voltage());
      public_data("voltage", volt.c_str());
      if (ina219.begin()){
        public_data("amperage", String(ina219.getCurrent()).c_str());
      }
    }
}


// void loop(){
//   Serial.println(voltmetr.read_voltage());
//   delay(100);
// }
