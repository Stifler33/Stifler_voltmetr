#pragma once
#include <Arduino.h>
#include <GTimer.h>

/**
 * частота ШИМ
 */
const static int pwm_freq = 20000;

/**
 * Допустимая разница(погрешность) между реальным и желаемым напряжением
 */
extern float difference_min_voltage;

/**
 * минимальный ток при зарядке
 * 
 * если ток меньше этого значения значит зарядка окончена
 */
extern float min_amperage_charge;

extern float difference_min_pre_voltage;

/**
 * допустимая разница между реальным током и желаемым
 * 
 * применяется в функции коррекции ампеража Stifler_voltage_manager::correct_amperage
 */
const float range_difference_amperage = 0.05;

/**
 * задержка для измирения емкости акб
 * 
 * значение в миллисекундах
 */
const uint32_t delay_measure_ah = 1000;

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
        const static uint32_t relay = 130;            
        const static uint32_t max = 255;
        const static uint32_t max_pu = 1024;
        const static uint32_t min = 0;
};

class pin{
    public:
        const static uint8_t load = 23;
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
            const static uint8_t cv = 26;
        };
};