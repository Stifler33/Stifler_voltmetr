#pragma once
#include <charger_general.h>
#include <stifler_spiffs.h>
#include <stifler_voltmetr.h>
#include <stifler_relay.h>

class Stifler_voltage_manager{
    public:
        Stifler_voltage_manager();
        bool begin();
        bool set_pu_voltage(float voltage);
        bool charge(float desired_voltage, float desired_amperage);
        //цикл в котором обновляем показания с датчиков
        void loop();
        //напряжение с акб (ads1115)
        float real_voltage;
        //напряжение с монитора напряжения ina219        
        float pm_voltage;
        //ампераж с монитора напряжения ina219
        float pm_amperage;
        //true если все показания читаются
        bool is_ready;
        //управление реле
        Stifler_relay relay;
    private:
        Voltage_map map_v;
        Stifler_voltmetr voltmetr;        
        int delta_voltage;
        float discharge_voltage;
        float discharge_amperage;
        float charge_voltage;
        float charge_amperage;
        float min_amperage_charge;
        int duty_pwm_cc;
        int duty_pwm_cv;        
        void correct_amperage(float desired_amperage);
        
};