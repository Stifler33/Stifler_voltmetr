#pragma once
#include <charger_general.h>
#include <stifler_spiffs.h>
#include <stifler_voltmetr.h>
#include <stifler_relay.h>
/**
 * Класс для реализации работы алгоритмов зарядки, разрядки
 * 
 * измирение соответствующих параметров
 * 
 * управление необходимыми реле
 */
class Stifler_voltage_manager{
    public:
        Stifler_voltage_manager();
        bool begin();        

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

        bool discharge(float desired_voltage, float desired_amperage);

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
        // мощность real_voltage * pm_amperage
        float power;
        // емкость
        float mAh;
        //управление реле
        Stifler_relay relay;
        void set_duty_cc(int duty);
        String get_duty_cc();
        String get_duty_cv();
    private:
        // карта напряжений для БП
        Voltage_map map_v;
        // вольтметр на ads1115
        Stifler_voltmetr voltmetr;
        // дельта для измеряемого напряжения. для ads1115
        int delta_voltage;
        bool pre_running;
        float discharge_voltage;
        float discharge_amperage;
        float charge_voltage;
        float charge_amperage;
        // если идет зарядка true иначе false
        bool is_running_charge;
        // если идет разрядка true иначе false
        bool is_running_discharge;
        //корректировка тока зарядки
        void correct_amperage_charge();
        // корректировка тока разрядки
        void correct_amperage_dischage();
        // устанавливаем напряжение на блоке питания согласно карте напряжений
        bool set_pu_voltage(float voltage);
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

            void write_duty(int duty=0);
        };

        class CV: public CC{
            public:
            CV(){
                ch = ch_pwm::cv;
            }
        };

        class DIS: public CC{
            public:
            DIS(){
                ch = ch_pwm::dis;
            }
        };
        CC cc;
        CV cv;
        DIS dis;
        
};