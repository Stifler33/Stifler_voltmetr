#include "charger_test_board.h"

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

int freq_pwm = 20000;
uint8_t pwm_duty_on = 130;

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

    ledcSetup(ch_cc, freq_pwm, 8);
    ledcAttachPin(cc, ch_cc);

    ledcSetup(ch_cv, freq_pwm, 8);
    ledcAttachPin(cv, ch_cv);
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