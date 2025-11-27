#include "stifler_spiffs.h"

Voltage_map::Voltage_map(){    
    
}

bool Voltage_map::check_file(){
    File file = SPIFFS.open("/map_voltage.json", "r");
    if (!file){
        return false;
    }

    DeserializationError error = deserializeJson(map_volt, file);
    if (error){
        return false;
    }
    return true;
}

bool Voltage_map::init(){
    bool begin = SPIFFS.begin(true);
    if (!begin){return false;}

    bool is_file = check_file();
    if (!is_file){return false;}

    return true;
}

void Voltage_map::print_size(){
    Serial.printf("Общий объем SPIFFS: %u байт\r\n", SPIFFS.totalBytes());
    Serial.printf("Использовано: %u байт\r\n", SPIFFS.usedBytes());
}