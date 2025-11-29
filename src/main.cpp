#include <stifler_wifi.h>
#include <stifler_mqtt.h>
#include <GyverINA.h>
#include <stifler_voltage_manager.h>

Stifler_voltage_manager volt_manager;

GTimer<millis> wait_pub(1000, true);

void set_delta(String delta){

}

bool charge = false;

void set_charge(String flag){
  if (flag == "on"){
    charge = true;
  }
  if (flag == "off"){
    charge = false;
  }
}

float charge_cc;
void set_charge_cc(String cc){
  charge_cc = cc.toFloat();
}

float charge_cv;
void set_charge_cv(String cv){
  charge_cv = cv.toFloat();
}

void setup(){
  
  Serial.begin(115200);
  Serial.println("hello");
  Wire.begin(18, 5);
  volt_manager.begin();
  
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
  add_pub_topic("calibrate", "charger/test/calibrate");

  add_sub_topic("set_charge", "charger/test/set_charge", set_charge);
  add_sub_topic("set_charge_cc", "charger/test/set_charge_cc", set_charge_cc);
  add_sub_topic("set_charge_cv", "charger/test/set_charge_cv", set_charge_cv);

  init_brocker();  
}

void loop(){
    if (loop_status_wifi()){
        loop_mqtt();
    }
    volt_manager.loop();    
    if (wait_pub){    
      public_data("voltage", String(volt_manager.real_voltage).c_str());      
      public_data("amperage", String(volt_manager.pm_amperage).c_str());
      public_data("voltage_ina", String(volt_manager.pm_voltage).c_str());
      if (charge){
        public_data("calib_data", "charge");
      }else{
        public_data("calib_data", "off");
      }
    }

    if (charge){
      charge = volt_manager.charge(charge_cv, charge_cc);
    }
}
