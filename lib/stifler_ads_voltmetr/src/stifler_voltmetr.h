#include <Adafruit_ADS1X15.h>
#include <GyverINA.h>
#include <GTimer.h>
#include <charger_general.h>

class Stifler_voltmetr: public Adafruit_ADS1X15{
    public:
        /**
        * Устанавливаем значение дельты для вычисления напряжения
        * @param max_mv максимальное ожидаемое напряжение
        * @param max_adc_value максимальное ожидаемое цифровое значение напряжения
        */
        void set_delta(int max_mv = 30000, int max_adc_value = 32767);

        /**
         * Читаем напряжение с INA219
         * @param value передаем переменную для записи в нее считанного напряжения
         * @return true если ina в сети и напряжение считано
         */        
        bool pm_voltage_amperage(float *for_value_voltage, float* for_value_amperage);
        
        bool values(
            float *real_voltage=0, 
            float *amperage=0, 
            float *pm_voltage=0, 
            float *power=0,
            float *mAh=0
        );

        /**
         * Получить напряжение
         */
        float read_voltage();

        /**
         * Получаем значение емкости акб
         * @param sec промежуток времени за который происходит измирение
         */
        float get_total_AH(int sec=1);

        /**
         * Записать измеренное напряжение в переменную.
         * 
         * Возвращает true если акб подключен правильно, иначе false
         * 
         * @param value переменная для хранения измеренного напряжения
         */
        bool read_voltage(float *value);

        /**
         * Задаем номер канала на котором будем считывать напряжение
         * 
         * адрес датчика и шина i2c на которой он находиться
         * 
         * @param channel канал который читаем
         */
        bool begin(uint8_t ch_voltage, uint8_t ch_polarity, uint8_t i2c_address = 72U, TwoWire *wire = &Wire);

        /**
         * @return true если ads1115 в сети
         */
        bool is_ready();

    private:
        /*Переменная для хранения дельты*/
        float delta;
        /*Канал на котором отслеживаем напряжение*/
        uint8_t ch_v;
        /*Канал для проверки полярности подключенного напряжения*/
        uint8_t ch_p;
        /*Максимальное сырое значение напряжения на канале*/
        int max_d = 32767;
        /*хранит состояние подключения*/
        bool ready;
};