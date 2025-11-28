/**
 * Здесь реализована работа с реле
 */
#include <stifler_relay.h>

GTimer<millis> kick_end(100, false, GTMode::Timeout);
GTimer<millis> kick_pu(100, false, GTMode::Timeout);
GTimer<millis> kick_plus(100, false, GTMode::Timeout);
GTimer<millis> kick_minus(100, false, GTMode::Timeout);

Stifler_relay::Stifler_relay(){
    end.state = false;
    pu.state = false;
    minus.state = false;
    plus.state = false;
}

void Stifler_relay::begin(){
    ledcSetup(ch_pwm::rl_end, pwm_freq, res_pwm::relay);
    ledcSetup(ch_pwm::rl_pu, pwm_freq, res_pwm::relay);
    ledcSetup(ch_pwm::rl_plus, pwm_freq, res_pwm::relay);
    ledcSetup(ch_pwm::rl_minus, pwm_freq, res_pwm::relay);
    
    ledcAttachPin(pin::rl::end, ch_pwm::rl_end);
    ledcAttachPin(pin::rl::plus, ch_pwm::rl_pu);
    ledcAttachPin(pin::rl::minus, ch_pwm::rl_minus);
    ledcAttachPin(pin::rl::plus, ch_pwm::rl_plus);
}

void Stifler_relay::end::on(){
    if (!state){
        state = true;
        ledcWrite(ch_pwm::rl_end, pwm_duty::max);
        kick_end.start();
    } 
}

void Stifler_relay::end::off(){
    if (state){
        state = false;
        ledcWrite(ch_pwm::rl_end, pwm_duty::min);        
    } 
}

void Stifler_relay::pu::on(){
    if (!state){
        state = true;
        ledcWrite(ch_pwm::rl_pu, pwm_duty::max);
        kick_pu.start();
    } 
}

void Stifler_relay::pu::off(){
    if (state){
        state = false;
        ledcWrite(ch_pwm::rl_pu, pwm_duty::min);
    } 
}

void Stifler_relay::plus::on(){
    if (!state){
        state = true;
        ledcWrite(ch_pwm::rl_plus, pwm_duty::max);
        kick_plus.start();
    } 
}

void Stifler_relay::plus::off(){
    if (state){
        state = false;
        ledcWrite(ch_pwm::rl_plus, pwm_duty::min);        
    } 
}

void Stifler_relay::minus::on(){
    if (!state){
        state = true;
        ledcWrite(ch_pwm::rl_minus, pwm_duty::max);
        kick_minus.start();
    } 
}

void Stifler_relay::minus::off(){
    if (state){
        state = false;
        ledcWrite(ch_pwm::rl_minus, pwm_duty::min);        
    } 
}

void Stifler_relay::loop(){
    if (kick_end){
        ledcWrite(ch_pwm::rl_end, pwm_duty::relay);
    }

    if (kick_pu){
        ledcWrite(ch_pwm::rl_pu, pwm_duty::relay);
    }

    if (kick_plus){
        ledcWrite(ch_pwm::rl_plus, pwm_duty::relay);
    }

    if (kick_minus){
        ledcWrite(ch_pwm::rl_minus, pwm_duty::relay);
    }
}

