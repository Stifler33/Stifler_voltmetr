#include <stifler_voltage_manager.h>

/**
 * какую полярность ампеража мы считаем разрядкой а какую зарядкой
 * @param amperage измеренный ток
 * @return true если зарядка false если разрядка
 */
auto amperage_polarity = [](float amperage){
    return amperage > 0;
};

auto increment_cc = [](int *cc){
    if (*cc < pwm_duty::max_pu){
        ledcWrite(ch_pwm::cc, *cc++);
    }
};

auto decrement_cc = [](int *cc){
    if (*cc > 0){
        ledcWrite(ch_pwm::cc, *cc--);
    }
};

auto increment_cv = [](int *cv){
    if (*cv < pwm_duty::max_pu){
        ledcWrite(ch_pwm::cv, *cv++);
    }
};

auto decrement_cv = [](int *cv){
    if (*cv > 0){
        ledcWrite(ch_pwm::cv, *cv--);
    }
};

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
    relay.loop();
    is_ready = voltmetr.read_voltage(&real_voltage) 
    && voltmetr.pm_voltage_amperage(&pm_voltage, &pm_amperage);
}

bool Stifler_voltage_manager::charge(float desired_voltage, float desired_amperage){
    if (charge_voltage != desired_voltage){
        charge_voltage = desired_voltage;
        set_pu_voltage(charge_voltage);
    }
    relay.end.on();
    relay.pu.on();
    bool is_voltage = (desired_voltage - real_voltage) < difference_min_voltage;
    bool is_amperage = pm_amperage < min_amperage_charge;    
    if (!is_voltage && is_amperage){
        set_pu_voltage(0.0);
        charge_voltage = 0.0;
        relay.end.on();
        relay.pu.on();
        return true;
    }
    correct_amperage(desired_amperage);
    return false;
}

void Stifler_voltage_manager::correct_amperage(float desired_amperage){
    if (amperage_polarity(pm_amperage)){
        if (pm_amperage < desired_amperage){
            increment_cc(&duty_pwm_cc);
        }else{
            decrement_cc(&duty_pwm_cc);
        }
    }else{
        if (abs(pm_amperage) < desired_amperage){
            increment_cc(&duty_pwm_cc);
        }else{
            decrement_cc(&duty_pwm_cc);
        }
    }
}