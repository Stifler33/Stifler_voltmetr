#include <Arduino.h>
#include "esp_adc_cal.h"
#include "driver/adc.h"

// Параметры для GPIO 27 (ADC2, канал 7)
#define DEFAULT_VREF    1100        // Значение Vref по умолчанию (1100 мВ = 1.1 В)
#define ADC_UNIT        ADC_UNIT_2  // Используем ADC2
#define ADC_CHANNEL     adc2_channel_t::ADC2_CHANNEL_7 // GPIO 27 соответствует каналу 7 на ADC2
#define ADC_ATTEN       ADC_ATTEN_DB_11 // Ослабление 11 дБ
#define ADC_WIDTH       ADC_WIDTH_BIT_12 // Разрешение 12 бит

// Структура для хранения характеристик калибровки
static esp_adc_cal_characteristics_t adc_chars; 

void setup() {
  Serial.begin(115200);

  // 1. Настройка пина ADC (Сначала устанавливаем ширину для ADC2)
  // В ESP-IDF API нет отдельной функции adc2_config_width, она задается глобально или неявно.
  // Мы настраиваем только ослабление.
  adc2_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);

  // 2. Характеризация ADC (Калибровка)
  // Проверяем, какие данные калибровки доступны в eFuse чипа
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
      ADC_UNIT, 
      ADC_ATTEN, 
      ADC_WIDTH, 
      DEFAULT_VREF, 
      &adc_chars
  );

  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    Serial.println("Калибровка выполнена с использованием Vref из eFuse.");
  } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
    Serial.println("Калибровка выполнена с использованием двухточечных данных из eFuse.");
  } else {
    Serial.println("Калибровка по eFuse недоступна. Используется Vref по умолчанию.");
  }
}

void loop() {
  int raw_adc = 0;
  
  // 1. Считывание "сырого" значения с ADC2
  // Функция adc2_get_raw возвращает ESP_OK при успешном считывании
  esp_err_t status = adc2_get_raw((adc2_channel_t)ADC_CHANNEL, ADC_WIDTH, &raw_adc);

  if (status == ESP_OK) {
      // 2. Преобразование сырого значения в напряжение (в мВ) с компенсацией нелинейности
      uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(raw_adc, &adc_chars);

      Serial.print("Сырое ADC (GPIO 27): ");
      Serial.print(raw_adc);
      
      Serial.print(" -> Скорректированное напряжение: ");
      Serial.print(voltage_mv * 9.74);
      Serial.println(" мВ");
      
  } else if (status == ESP_ERR_TIMEOUT) {
      Serial.println("Ошибка: Считывание ADC2 не удалось (возможно, из-за использования Wi-Fi/Bluetooth)");
  } else {
      Serial.println("Ошибка: Считывание ADC2 не удалось по другой причине.");
  }

  delay(1000);
}