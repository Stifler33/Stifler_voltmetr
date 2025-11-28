#include "charger_test_board.h"

bool flag_map_volt = false;
int counter_measure = 0;
Voltage_map volt_map;

uint8_t rl_minus = 12;
uint8_t rl_plus = 4;
uint8_t rl_end = 17;
uint8_t rl_pu = 13;
uint8_t pm_sda = 18;
uint8_t pm_scl = 5;
uint8_t dis_bat = 23;
uint8_t temp_sensors = 27;

uint8_t cc = 25;
uint8_t cv = 26;

uint8_t ch_rl_end = 0;
uint8_t ch_rl_pu = 1;
uint8_t ch_rl_plus = 2;
uint8_t ch_rl_minus = 3;

uint8_t ch_cc = 4;
uint8_t ch_cv = 5;

uint8_t ch_dis = 6;

int freq_pwm = 20000;
uint8_t pwm_duty_on = 130;
uint8_t res_pwm_cccv = 10;

GTimer<millis> kick_end(100, false, GTMode::Timeout);
GTimer<millis> kick_pu(100, false, GTMode::Timeout);
GTimer<millis> kick_plus(100, false, GTMode::Timeout);
GTimer<millis> kick_minus(100, false, GTMode::Timeout);

void init_output(){    

    ledcSetup(ch_rl_end, freq_pwm, 8);
    ledcAttachPin(rl_end, ch_rl_end);

    ledcSetup(ch_rl_pu, freq_pwm, 8);
    ledcAttachPin(rl_pu, ch_rl_pu);

    ledcSetup(ch_rl_plus, freq_pwm, 8);
    ledcAttachPin(rl_plus, ch_rl_plus);

    ledcSetup(ch_rl_minus, freq_pwm, 8);
    ledcAttachPin(rl_minus, ch_rl_minus);

    ledcSetup(ch_cc, freq_pwm, res_pwm_cccv);
    ledcAttachPin(cc, ch_cc);

    ledcSetup(ch_cv, freq_pwm, res_pwm_cccv);
    ledcAttachPin(cv, ch_cv);
    
    ledcSetup(ch_dis, freq_pwm, 10);
    ledcAttachPin(dis_bat, ch_dis);

}

void switch_end_relay(String type){
    if (type == "on"){
        ledcWrite(ch_rl_end, 255);
        kick_end.start();
    }
    if (type == "off"){
        ledcWrite(ch_rl_end, 0);
    }
}

void switch_pu_relay(String type){
    if (type == "on"){
        ledcWrite(ch_rl_pu, 255);
        kick_pu.start();
    }
    if (type == "off"){
        ledcWrite(ch_rl_pu, 0);
    }
}

void switch_plus_relay(String type){
    if (type == "on"){
        ledcWrite(ch_rl_plus, 255);
        kick_plus.start();
    }
    if (type == "off"){
        ledcWrite(ch_rl_plus, 0);
    }
}

void switch_minus_relay(String type){
    if (type == "on"){
        ledcWrite(ch_rl_minus, 255);
        kick_minus.start();
    }
    if (type == "off"){
        ledcWrite(ch_rl_minus, 0);
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


void loop_relay(){
    if (kick_end){
        ledcWrite(ch_rl_end, pwm_duty_on);
    }
    if (kick_pu){
        ledcWrite(ch_rl_pu, pwm_duty_on);
    }
    if (kick_plus){
        ledcWrite(ch_rl_plus, pwm_duty_on);
    }
    if (kick_minus){
        ledcWrite(ch_rl_minus, pwm_duty_on);
    }
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