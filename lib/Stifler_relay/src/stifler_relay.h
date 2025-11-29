#pragma once
#include <charger_general.h>

class Stifler_relay{
    public:
        Stifler_relay();
        //Инициализируем PWM
        void begin();
        //Необходимо постоянно вызывать в цикле
        void loop();
        class end{
            public:
            void on();
            void off();
            bool state;
        };

        class pu{
            public:
            void on();
            void off();
            bool state;
        };

        class minus{
            public:
            void on();
            void off();
            bool state;
        };

        class plus{
            public:
            void on();
            void off();
            bool state;
        };

        end end;
        pu pu;
        minus minus;
        plus plus;
    private:        
        
};