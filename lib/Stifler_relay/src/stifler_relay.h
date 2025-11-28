#pragma once
#include <charger_general.h>

class Stifler_relay{
    public:
        Stifler_relay();
        void begin();
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