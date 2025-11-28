#include <stifler_voltage_manager.h>

bool Stifler_voltage_manager::begin(){
    ledcSetup(ch_pwm::rl_end, pwm_freq, res_pwm::relay);
    ledcSetup(ch_pwm::rl_pu, pwm_freq, res_pwm::relay);
    ledcSetup(ch_pwm::rl_plus, pwm_freq, res_pwm::relay);
    ledcSetup(ch_pwm::rl_minus, pwm_freq, res_pwm::relay);
    ledcSetup(ch_pwm::cc, pwm_freq, res_pwm::cc_cv);
    ledcSetup(ch_pwm::cv, pwm_freq, res_pwm::cc_cv);
    ledcSetup(ch_pwm::dis, pwm_freq, res_pwm::cc_cv);

    ledcAttachPin(rl_end, ch_pwm::rl_end);
    ledcAttachPin(rl_pu, ch_pwm::rl_pu);
    ledcAttachPin(rl_minus, ch_pwm::rl_minus);
    ledcAttachPin(rl_plus, ch_pwm::rl_plus);
    ledcAttachPin(cc, ch_pwm::cc);
    ledcAttachPin(cv, ch_pwm::cv);
    ledcAttachPin(dis_bat, ch_pwm::dis);
    
    return map_v.init();    
}