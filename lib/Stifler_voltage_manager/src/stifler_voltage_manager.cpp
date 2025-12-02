#include <stifler_voltage_manager.h>

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

    is_running_charge = false;
    is_running_discharge = false;
    pre_running = false;
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
    cv.write_duty(map_v.get_duty(voltage));
    return true;
}

void Stifler_voltage_manager::loop(){
    relay.loop();

    is_ready = voltmetr.values(
        &real_voltage,
        &pm_amperage,
        &pm_voltage,
        &power,
        &mAh
    );
    if (!is_ready){
        off();
    }

    // if (!is_running_charge && !is_running_discharge){
    //     off();
    // }
}

bool Stifler_voltage_manager::charge(float desired_voltage, float desired_amperage){
    if (!is_ready && !is_running_discharge){
        return false;
    }

    if (charge_voltage != desired_voltage){
        charge_voltage = desired_voltage;        
    } 

    if (desired_amperage != charge_amperage){
        charge_amperage = desired_amperage;
        cc.fixed_duty = pwm_duty::max_pu;
    }

    if (!pre_running){
        dis.write_duty(0);
        set_pu_voltage(charge_voltage);        
        relay.pu.on();
        cc.write_duty(10);
        pre_running = (charge_voltage - pm_voltage) < difference_min_pre_voltage && 
        pm_amperage < min_amperage_charge && pm_amperage > -min_amperage_charge;
        return false;
    }

    if (!is_running_charge){
        dis.write_duty(0);
        set_pu_voltage(charge_voltage);
        relay.end.on();
        relay.pu.on();
    }

    is_running_charge = true;

    bool is_voltage = (charge_voltage - real_voltage) < difference_min_voltage;
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
        is_running_charge = false;
        return true;
    }
   
    correct_amperage_charge();
    return false;
}

bool Stifler_voltage_manager::discharge(float desired_voltage, float desired_amperage){
    if (!is_ready && !is_running_charge){
        return false;
    }

    if (desired_voltage != discharge_voltage){
        discharge_voltage = desired_voltage;
    }

    if (desired_amperage != discharge_amperage){
        discharge_amperage = desired_amperage;
        dis.fixed_duty = pwm_duty::max_pu;
    }

    if (!is_running_discharge){
        cc.write_duty(0);
        cv.write_duty(0);
        relay.pu.off();
        relay.end.on();
    }

    is_running_discharge = true;    

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
        is_running_discharge = false;
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
    is_running_charge = false;
    is_running_discharge = false; 
    pre_running = false;
    relay.pu.off();
    relay.end.off();
    relay.plus.off();
    relay.minus.off();
    cc.write_duty();
    cv.write_duty();
    dis.write_duty();
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

void Stifler_voltage_manager::CC::write_duty(int duty){
    if (duty < pwm_duty::max_pu && duty >= 0){
        pwm_duty = duty;
        ledcWrite(ch, pwm_duty);
    }    
}
