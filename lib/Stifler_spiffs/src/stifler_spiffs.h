#include <SPIFFS.h>
#include "FS.h"
#include <ArduinoJson.h>

class Voltage_map{
    public:
        Voltage_map();
        /**
         * Инициализирует память Spiffs 
         * @return возвращает true если инициализация прошла успешно
         */
        bool init();
        void print_size();
    private:
        JsonDocument map_volt;
        bool check_file();
};