#include <stifler_voltage_manager.h>

// таймер для зарядки
GTimer<millis> wait_end_task(10000, false, GTMode::Timeout);

// Задаем необходимы начальные параметры
Stifler_voltage_manager::Stifler_voltage_manager(){
    delta_voltage = 24420;
    cc.pwm_duty = 0;
    cc.fixed_duty = pwm_duty::max_pu;

    cv.pwm_duty = 0;
    cv.fixed_duty = pwm_duty::max_pu;

    dis.pwm_duty = 0;
    dis.fixed_duty = pwm_duty::max_pu; 

    is_charge = false;
    is_discharge = false;
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
    cv.pwm_duty = map_v.get_duty(voltage);
    ledcWrite(ch_pwm::cv, cv.pwm_duty);
    return true;
}

void Stifler_voltage_manager::loop(){
    relay.loop();
    // is_ready = voltmetr.read_voltage(&real_voltage) 
    // && voltmetr.pm_voltage_amperage(&pm_voltage, &pm_amperage);

    is_ready = voltmetr.values(
        &real_voltage,
        &pm_amperage,
        &pm_voltage,
        &power,
        &mAh
    );
}

bool Stifler_voltage_manager::charge(float desired_voltage, float desired_amperage){

    if (charge_voltage != desired_voltage){
        charge_voltage = desired_voltage;
        set_pu_voltage(charge_voltage);
    }

    if (desired_amperage != charge_amperage){
        charge_amperage = desired_amperage;
        cc.fixed_duty = pwm_duty::max_pu;
    }
    relay.end.on();
    relay.pu.on();
    is_charge = true;

    bool is_voltage = (desired_voltage - real_voltage) < difference_min_voltage;
    bool is_amperage = abs(pm_amperage) < min_amperage_charge;

    if (is_voltage && is_amperage){
        if (!wait_end_task.running()){
            wait_end_task.start();
        }
    }else{
        wait_end_task.stop();
    }

    if (wait_end_task){
        off();
        is_charge = false;
        return true;
    }
   
    correct_amperage_charge();
    return false;
}

bool Stifler_voltage_manager::discharge(float desired_voltage, float desired_amperage){
    if (desired_voltage != discharge_voltage){
        discharge_voltage = desired_voltage;
    }

    if (desired_amperage != discharge_amperage){
        discharge_amperage = desired_amperage;
        dis.fixed_duty = pwm_duty::max_pu;
    }

    relay.end.on();    
    is_discharge = true;

    bool is_voltage = (real_voltage - discharge_voltage) < difference_min_voltage;
    bool is_amperage = abs(pm_amperage) < min_amperage_charge;

    if (is_voltage && is_amperage){
        if (!wait_end_task.running()){
            wait_end_task.start();
        }
    }else{
        wait_end_task.stop();
    }

    if (wait_end_task){
        off();
        is_discharge = false;
        return true;
    }
    correct_amperage_dischage();
    return false;
}

void Stifler_voltage_manager::correct_amperage_charge(){
    float difference_amper = charge_amperage - abs(pm_amperage);

    if (difference_amper > range_difference_amperage){
        cc.increment();
        return;
    }

    if (difference_amper < -range_difference_amperage){
        cc.decrement();
        return;
    }

    cc.fix_it_duty();
}

void Stifler_voltage_manager::correct_amperage_dischage(){
    float difference_amper = discharge_amperage - abs(pm_amperage);


    if (difference_amper > range_difference_amperage && real_voltage >= discharge_voltage){
        dis.increment();
        return;
    }

    if (difference_amper < -range_difference_amperage || real_voltage <= discharge_voltage){
        dis.decrement();
        return;
    }

    dis.fix_it_duty();
}

void Stifler_voltage_manager::off(){
    set_pu_voltage(0.0);
    relay.pu.off();
    relay.end.off();
    cc.pwm_duty = 0;
    cv.pwm_duty = 0;
    dis.pwm_duty = 0;
    dis.fixed_duty = pwm_duty::max_pu;
    cc.fixed_duty = pwm_duty::max_pu;
    cv.fixed_duty = pwm_duty::max_pu;

}

String Stifler_voltage_manager::get_duty_cc(){
    return String(cc.pwm_duty);
}

String Stifler_voltage_manager::get_duty_cv(){
    return String(cv.pwm_duty);
}

void Stifler_voltage_manager::set_duty_cc(int duty){
    cc.pwm_duty = duty;
    ledcWrite(ch_pwm::cc, cc.pwm_duty);
}

void Stifler_voltage_manager::CC::increment(){
    if (pwm_duty < fixed_duty && pwm_duty < pwm_duty::max_pu){
        pwm_duty++;
        ledcWrite(ch, pwm_duty);
    }
}

void Stifler_voltage_manager::CC::decrement(){
    if (pwm_duty > 0){
        pwm_duty--;
        ledcWrite(ch, pwm_duty);
    }
}

void Stifler_voltage_manager::CC::fix_it_duty(){
    fixed_duty = pwm_duty;
}