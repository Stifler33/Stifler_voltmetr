#include "charger_test_board.h"

Stifler_relay relay;

bool flag_map_volt = false;
int counter_measure = 0;
Voltage_map volt_map;

uint8_t pm_sda = 18;
uint8_t pm_scl = 5;
uint8_t dis_bat = 23;
uint8_t temp_sensors = 27;

uint8_t cc = 25;
uint8_t cv = 26;

uint8_t ch_cc = 4;
uint8_t ch_cv = 5;

uint8_t ch_dis = 6;

uint8_t pwm_duty_on = 130;
uint8_t res_pwm_cccv = 10;

void init_output(){    

    relay.begin();    
    ledcSetup(ch_cc, pwm_freq, res_pwm_cccv);
    ledcAttachPin(cc, ch_cc);

    ledcSetup(ch_cv, pwm_freq, res_pwm_cccv);
    ledcAttachPin(cv, ch_cv);
    
    ledcSetup(ch_dis, pwm_freq, 10);
    ledcAttachPin(dis_bat, ch_dis);

}

void switch_end_relay(String type){
    if (type == "on"){
        relay.end.on();        
    }
    if (type == "off"){
        relay.end.off();
    }
}

void switch_pu_relay(String type){
    if (type == "on"){
        relay.pu.on();
    }
    if (type == "off"){
        relay.pu.off();
    }
}

void switch_plus_relay(String type){
    if (type == "on"){
        relay.plus.on();
    }
    if (type == "off"){
        relay.plus.off();
    }
}

void switch_minus_relay(String type){
    if (type == "on"){
        relay.minus.on();
    }
    if (type == "off"){
        relay.minus.off();
    }
}

void CC(String value){
    int pwm = value.toInt();
    ledcWrite(ch_cc, pwm);
}

void CC(int value){    
    ledcWrite(ch_cc, value);
}

void CV(String value){
    int pwm = value.toInt();
    ledcWrite(ch_cv, pwm);
}

void discharge(String value){
    int v = value.toInt();
    ledcWrite(ch_dis, v);
}

void map_volt(String value){
    if (value == "on"){
        flag_map_volt = true;
        counter_measure = 0;
        switch_end_relay("on");
        switch_pu_relay("on");
        // public_data("relay_end", "on");
        // public_data("relay_pu", "on");
        // unsubscribe("charger/test/cv");        
    }
    if (value == "off"){
        flag_map_volt = false;
        counter_measure = 0;
        switch_end_relay("off");
        switch_pu_relay("off");
        // public_data("relay_end", "off");
        // public_data("relay_pu", "off");
        // add_sub_topic("cv", "charger/test/cv", CV);
    }
}

bool wait_voltage(float new_voltage){
    static float old_voltage;
    if (abs(new_voltage - old_voltage) < 0.005){
        return true;
    }else{
        old_voltage = new_voltage;
        return false;
    }
}

void set_voltage(String voltage){
    CC(20);
    CV(String(volt_map.get_duty(voltage.toFloat())));
}