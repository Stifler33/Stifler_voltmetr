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

        /**
         * Зарядка. Данную функцию вызываем в основом цикле для зарядки акб.
         * 
         * передаем ей напряжение и вольтаж которыми хотим заряжать. 
         * 
         * напряжение выставляется согласно сохраненной таблице, ток корректируется каждый вызов функции
         * @param desired_voltage желаемое напряжение зарядки
         * @param desired_amperage желаемый ток зарядки
         * @return true если зарядка окончена false если еще идет 
         */
        bool charge(float desired_voltage, float desired_amperage);

        //цикл в котором обновляем показания с датчиков
        void loop();

        // выключение всего и сброс переменных
        void off();
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
        void set_duty_cc(int duty);
        String get_duty_cc();
        String get_duty_cv();
    private:
        Voltage_map map_v;
        Stifler_voltmetr voltmetr;        
        int delta_voltage;
        float discharge_voltage;
        float discharge_amperage;
        float charge_voltage;
        float charge_amperage;                        
        void correct_amperage(float desired_amperage);
        /**
         * Храним значения и функции для управления сигналами CC CV
         */
        class CC{
            public:
            uint8_t ch;
            CC(){
                ch = ch_pwm::cc;
            }
            /**
             * Текущее значение ШИМ
             */
            uint32_t pwm_duty;

            /**
             * Значение ШИМ выше которого мы не поднимаемся
             */
            uint32_t fixed_duty;

            /**
             * Увеличиваем скважность ШИМ на 1, но не больше duty_pwm::max_pu
             */
            void increment();

            /**
             * уменьшаем скважность ШИМ на 1, но не меньше 0
             */
            void decrement();

            /**
             * фиксируем текущее значение скважности
             */
            void fix_it_duty();
        };

        class CV: public CC{
            public:
            CV(){
                ch = ch_pwm::cv;
            }
        };
        CC cc;
        CV cv;
        
};