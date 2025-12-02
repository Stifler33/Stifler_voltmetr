#include <stifler_wifi.h>
#include <stifler_mqtt.h>
#include <GyverINA.h>
#include <stifler_voltage_manager.h>

Stifler_voltage_manager volt_manager;

GTimer<millis> wait_pub(1000, true);

void switch_rl_end(String state){
  state == "on" ? volt_manager.relay.end.on() :
  volt_manager.relay.end.off();
}

void switch_rl_pu(String state){
  state == "on" ? volt_manager.relay.pu.on() :
  volt_manager.relay.pu.off();
}

void set_delta(String delta){

}

bool charge = true;

void set_charge(String flag){
  if (flag == "on"){
    charge = false;
  }
  if (flag == "off"){
    charge = true;
    volt_manager.off();
  }
}

bool discharge = true;

void run_discharge(String value){
  if (value == "on"){
    discharge = false;
  }
  if (value == "off"){
    discharge = true;
    volt_manager.off();
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

float discharge_cc;
void set_discharge_cc(String cc){
  discharge_cc = cc.toFloat();
}

float discharge_cv;
void set_discharge_cv(String cv){
  discharge_cv = cv.toFloat();
}

void set_duty_cc(String value){
  volt_manager.set_duty_cc(value.toInt());
}

void set_min_amp_charge(String value){
  min_amperage_charge = value.toFloat();
}

void set_difference_charge_voltage(String value){
  difference_min_voltage = value.toFloat();
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
  add_pub_topic("relay_end_status", "charger/test/rl_end_status");
  add_pub_topic("relay_pu_status", "charger/test/rl_pu_status");

  add_pub_topic("cc", "charger/test/cc");
  add_pub_topic("cv", "charger/test/cv");

  add_pub_topic("power", "charger/test/power");
  add_pub_topic("mAh", "charger/test/mAh");

  add_sub_topic("set_charge", "charger/test/set_charge", set_charge);
  add_sub_topic("run_discharge", "charger/test/run_discharge", run_discharge);

  add_sub_topic("set_charge_cc", "charger/test/set_charge_cc", set_charge_cc);
  add_sub_topic("set_charge_cv", "charger/test/set_charge_cv", set_charge_cv);

  add_sub_topic("set_discharge_cc", "charger/test/set_discharge_cc", set_discharge_cc);
  add_sub_topic("set_discharge_cv", "charger/test/set_discharge_cv", set_discharge_cv);

  add_sub_topic("relay_end", "charger/test/rl_end", switch_rl_end);
  add_sub_topic("relay_pu", "charger/test/rl_pu", switch_rl_pu);
  add_sub_topic("set_duty_cc", "charger/test/set_duty_cc", set_duty_cc);

  add_sub_topic("set_char_amp", "charger/test/set_char_amp", set_min_amp_charge);
  add_sub_topic("set_diff", "charger/test/set_diff", set_difference_charge_voltage);

  init_brocker();  
}

void loop(){
    if (loop_status_wifi()){
        loop_mqtt();
    }
    volt_manager.loop();    
    if (wait_pub){  
      public_data("voltage", String(volt_manager.real_voltage).c_str(), true);
      public_data("amperage", String(volt_manager.pm_amperage).c_str(), true);
      public_data("voltage_ina", String(volt_manager.pm_voltage).c_str(), true);
      public_data("relay_end_status", volt_manager.relay.end.state ? "on" : "off", true);
      public_data("relay_pu_status", volt_manager.relay.pu.state ? "on" : "off", true);
      public_data("cc", volt_manager.get_duty_cc().c_str(), true);
      public_data("cv", volt_manager.get_duty_cv().c_str(), true);

      public_data("power", String(volt_manager.power).c_str());
      public_data("mAh", String(volt_manager.mAh).c_str());

      // if (!charge){
      //   public_data("calib_data", "charge");
      // }else{
      //   public_data("calib_data", "off");
      // }
      
    }

    if (!charge){
      charge = volt_manager.charge(charge_cv, charge_cc);
    }
    if (!discharge){
      discharge = volt_manager.discharge(discharge_cv, discharge_cc);
    }
}
