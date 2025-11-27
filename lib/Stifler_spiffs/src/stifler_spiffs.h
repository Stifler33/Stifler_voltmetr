#pragma once
#include <SPIFFS.h>
#include "FS.h"
#include <ArduinoJson.h>
#include <map>

class Voltage_map{
    public:
        Voltage_map();
        /**
         * Инициализирует память Spiffs 
         * @return возвращает true если инициализация прошла успешно
         */
        bool init();

        /**
         * Сохраняет новое значение напряжения по указанному значению скважности
         * @param value_duty значение скважности которое будет соответствовать напряжению
         * @param value_voltage значение напряжения которое нужно сохранить
         * @return true если сохранение успешно, иначе false
         */
        bool read_new_value(int value_duty, float value_voltage);

        /**
         * Получаем значение скважности для интересующего напряжения
         * @param voltage напряжение для которого ищем значение скважности
         * @return значение скважности. Если подходящего напряжения не нашлось вернет 0
         */
        int get_duty(float voltage);

        void print_size();
        void print_map();
    private:
        /**
         * Документ где хранится карта напряжений
         */
        JsonDocument json_voltage;

        /**
         * Проверяем есть ли файл, если файл есть загружаем его в обьект json
         * 
         * если файла нет создаем новый и заполняем.
         */
        bool check_file();

        /**
         * Создает новый файл для карты напряжений и заполняет его нулевыми значениями
         */
        bool create_file();

        /**
         * Карта напряжений в формате std::map
         */        
        File file;
        bool is_init;
};