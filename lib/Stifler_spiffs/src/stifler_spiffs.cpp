#include "stifler_spiffs.h"

#define PATH_FILE "/map_voltage.json"
#define DUTY 1024

Voltage_map::Voltage_map(){}

bool Voltage_map::check_file(){
    file = SPIFFS.open(PATH_FILE, "r");
    if (!file){                
        return false;        
    }

    if (!SPIFFS.exists(PATH_FILE)) {
        file.close();
        if (!create_file()){
            return false;
        }    
        file = SPIFFS.open(PATH_FILE, "r");   
    }

    DeserializationError error = deserializeJson(json_voltage, file);    
    file.close();

    if (error){        
        return false;
    }
    return true;
}

bool Voltage_map::create_file(){
    file = SPIFFS.open(PATH_FILE, "w");
    if (!file){        
        return false;
    }
    
    for (int i = 0; i < DUTY; i++){
        json_voltage[String(i)] = 0.0f;
    }

    if (serializeJson(json_voltage, file) == 0){    
        return false;
    }

    file.close();
    return true;    
}

bool Voltage_map::init(){
    bool begin = SPIFFS.begin(true);
    if (!begin){
        is_init = false;
        return false;
    }

    bool is_file = check_file();    
    if (!is_file){
        is_init = false;
        return false;
    }
    
    is_init = true;
    print_map();
    return true;
}

void Voltage_map::print_size(){
    Serial.printf("Общий объем SPIFFS: %u байт\r\n", SPIFFS.totalBytes());
    Serial.printf("Использовано: %u байт\r\n", SPIFFS.usedBytes());
}

bool Voltage_map::read_new_value(int value_duty, float value_voltage){
    if (!is_init){return false;}
    if (value_duty > DUTY || value_duty < 0 || value_voltage < 0){
        return false;
    }

    if (json_voltage[String(value_duty)].is<float>()){
        json_voltage[String(value_duty)] = value_voltage;
    }else{
        return false;
    }

    file = SPIFFS.open(PATH_FILE, "w");
    if (file){
        if (serializeJson(json_voltage, file) == 0){
            file.close();
            return false;
        }
        file.close();
        return true;
    }
    return false;
}

void Voltage_map::print_map(){
    if (!is_init){
        Serial.println("error is_init");
    }

    for (int i = 0; i < DUTY; i++){
        Serial.print(i);
        Serial.print(" ");
        Serial.println(float(json_voltage[String(i)]));
    }
}

int Voltage_map::get_duty(float voltage){
    if (!is_init){return 0;}
    if (voltage < 0){return 0;}

    for (int i = 0; i < DUTY; i++){
        String i_str = String(i);
        float v_in_map = 0.0;

        if (json_voltage[i_str].is<float>()){
            if (float(json_voltage[i_str]) > voltage){
                return i;
            }            
        }else{
            return 0;
        }        
    }
    return 0;
}