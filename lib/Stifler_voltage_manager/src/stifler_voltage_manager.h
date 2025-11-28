#pragma once
#include <charger_general.h>

class Stifler_voltage_manager{
    public:
        Stifler_voltage_manager();
        bool begin();
        bool set_pu_voltage(float voltage);
        bool charge(float desired_voltage, float desired_amperage);
        void loop();
        float real_voltage;
        float pm_voltage;
        float pm_amperage;
        bool is_ready;
    private:
        Voltage_map map_v;
        Stifler_voltmetr voltmetr;
        Stifler_relay relay;
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