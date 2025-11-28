#pragma once
#include <stifler_spiffs.h>

class Stifler_voltage_manager{
    public:
        bool begin();
    private:
        Voltage_map map_v;
        
        uint8_t rl_minus = 12;
        uint8_t rl_plus = 4;
        uint8_t rl_end = 17;
        uint8_t rl_pu = 13;
        uint8_t pm_sda = 18;
        uint8_t pm_scl = 5;
        uint8_t dis_bat = 23;
        uint8_t cc = 25;
        uint8_t cv = 26;

        int pwm_freq = 20000;

        class ch_pwm{
            public:
                const static uint8_t rl_end = 0;
                const static uint8_t rl_pu = 1;
                const static uint8_t rl_plus = 2;
                const static uint8_t rl_minus = 3;
                const static uint8_t cc = 4;
                const static uint8_t cv = 5;
                const static uint8_t dis = 6;
        };
        
        class res_pwm{
            public:
                const static uint8_t cc_cv = 10;
                const static uint8_t relay = 8;
        };

        class pwm_duty{
            public:
                const static uint32_t relay = 120;            
                const static uint32_t max = 255;
                const static uint32_t min = 0;
        };
};