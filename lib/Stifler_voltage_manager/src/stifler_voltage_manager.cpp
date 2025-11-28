#include <stifler_voltage_manager.h>

Stifler_voltage_manager::Stifler_voltage_manager(){
    delta_voltage = 24420;
    min_amperage_charge = 0.1;
    duty_pwm_cc = 0;
    duty_pwm_cv = 0;    
}

bool Stifler_voltage_manager::begin(){

    ledcSetup(ch_pwm::cc, pwm_freq, res_pwm::cc_cv);
    ledcSetup(ch_pwm::cv, pwm_freq, res_pwm::cc_cv);
    ledcSetup(ch_pwm::dis, pwm_freq, res_pwm::cc_cv);

    ledcAttachPin(pin::pu::cc, ch_pwm::cc);
    ledcAttachPin(pin::pu::cv, ch_pwm::cv);
    ledcAttachPin(pin::load, ch_pwm::dis);

    relay.begin();
    voltmetr.begin(0, 1);
    voltmetr.set_delta(delta_voltage);
    
    return map_v.init();    
}

bool Stifler_voltage_manager::set_pu_voltage(float voltage){
    ledcWrite(ch_pwm::cv, map_v.get_duty(voltage));
    return true;
}

void Stifler_voltage_manager::loop(){
    is_ready = voltmetr.read_voltage(&real_voltage) 
    && voltmetr.pm_voltage_amperage(&pm_voltage, &pm_amperage);
}

bool Stifler_voltage_manager::charge(float desired_voltage, float desired_amperage){
    bool is_voltage = real_voltage < desired_voltage;
    bool is_amperage = pm_amperage < min_amperage_charge;
   
}

void Stifler_voltage_manager::correct_amperage(float desired_amperage){
    float difference_amperage = pm_amperage - desired_amperage;
    if (difference_amperage > 0.05){
        if (duty_pwm_cc < pwm_duty::max_pu){
            duty_pwm_cc--;
            ledcWrite(ch_pwm::cc, duty_pwm_cc);
        }
    }
    if (difference_amperage < -0.05){
        if (duty_pwm_cc > 0 && duty_pwm_cc < pwm_duty::max_pu){
            duty_pwm_cc++;
            ledcWrite(ch_pwm::cc, duty_pwm_cc);
        }
    }
}