#pragma once
#include <Arduino.h>
#include <stifler_spiffs.h>
#include <stifler_voltmetr.h>
#include <stifler_relay.h>
#include <GTimer.h>

const static int pwm_freq = 20000;
const static float difference_min_voltage = 0.01;
const static float difference_min_amperage = 0.01;
/**
 * каналы ШИМ для разных целей
 */
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

/**
 * разрешение ШИМ для разных целей
 */
class res_pwm{
    public:
        const static uint8_t cc_cv = 10;
        const static uint8_t relay = 8;
};

/**
 * фиксированная скважность ШИМ для разных целей
 */
class pwm_duty{
    public:
        const static uint32_t relay = 120;            
        const static uint32_t max = 255;
        const static uint32_t max_pu = 1024;
        const static uint32_t min = 0;
};

class pin{
    public:
        const static uint8_t load = 6;
        class rl{
            public:
            const static uint8_t pu = 13;
            const static uint8_t minus = 12;
            const static uint8_t plus = 4;
            const static uint8_t end = 17;
        };

        class sensors{
            public:
            const static uint8_t temp = 27;
            const static uint8_t pm_sda = 18;
            const static uint8_t pm_scl = 5;

        };

        class pu{
            public:
            const static uint8_t cc = 25;
            const static uint8_t cv = 25;
        };
};