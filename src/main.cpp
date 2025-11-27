#include <stifler_wifi.h>
#include <stifler_mqtt.h>
#include <charger_test_board.h>
#include <GyverINA.h>

INA219 ina219(0.01, 32.0);

Stifler_voltmetr voltmetr;
int delta = 0;
GTimer<millis> wait_pub(1000, true);


void set_delta(String delta){
  voltmetr.set_delta(delta.toInt());
}

void setup(){
  
  Serial.begin(115200);
  Serial.println("hello");
    
  set_pin_reset(0, LOW);
  init_stif();
  arduino_ota_initial();
  add_pub_topic("connect", "esp32/connect"); 

  add_pub_topic("voltage", "charger/test/voltage");
  add_pub_topic("voltage_ina", "charger/test/voltage_ina");

  add_pub_topic("amperage", "charger/test/amperage");
  add_pub_topic("relay_end", "charger/test/rl_end");
  add_pub_topic("relay_pu", "charger/test/rl_pu");

  add_pub_topic("cc", "charger/test/cc");
  add_pub_topic("cv", "charger/test/cv");

  add_pub_topic("calib_data", "charger/test/calib_data");

  add_sub_topic("relay_end", "charger/test/rl_end", switch_end_relay);
  add_sub_topic("relay_pu", "charger/test/rl_pu", switch_pu_relay);
  add_sub_topic("relay_plus", "charger/test/rl_plus", switch_plus_relay);
  add_sub_topic("relay_minus", "charger/test/rl_minus", switch_minus_relay);
  add_sub_topic("delta", "charger/test/set_delta", set_delta);
  add_sub_topic("cc", "charger/test/cc", CC);
  add_sub_topic("cv", "charger/test/cv", CV);
  add_sub_topic("dis", "charger/test/dis", discharge);
  add_sub_topic("calibrate", "charger/test/calibrate", map_volt);

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
      String volt;
      if (voltmetr.is_ready()){
        volt = String(voltmetr.read_voltage());
      }else{
        volt = -1;
      }     
      public_data("voltage", volt.c_str());
      if (ina219.begin()){
        public_data("amperage", String(ina219.getCurrent()).c_str());
        public_data("voltage_ina", String(ina219.getVoltage()).c_str());
      }else{
        public_data("amperage", String(-1).c_str());
        public_data("voltage_ina", String(-1).c_str());
      }

      if (flag_map_volt){
        if (counter_measure < max_counter_measure){
          if (wait_voltage(volt.toFloat())){
            save_calibration_value(counter_measure, volt.toFloat());
            public_data("calib_data", String(String(volt) + " " + String(counter_measure)).c_str());
            counter_measure++;
          }          
          CV(String(counter_measure));
        }else{
          flag_map_volt = false;
          counter_measure = 0;
        }
      }

    }
}


// void loop(){
//   Serial.println(voltmetr.read_voltage());
//   delay(100);
// }
