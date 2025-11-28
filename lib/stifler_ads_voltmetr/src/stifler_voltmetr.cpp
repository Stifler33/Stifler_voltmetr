#include "stifler_voltmetr.h"

INA219 power_monitor(0.01, 32.0);

bool Stifler_voltmetr::begin(uint8_t ch_voltage, uint8_t ch_polarity, uint8_t i2c_address, TwoWire *wire){
    ch_v = ch_voltage;    
    ch_p = ch_polarity;    
    // The ADC input range (or gain) can be changed via the following
    // functions, but be careful never to exceed VDD +0.3V max, or to
    // exceed the upper and lower limits if you adjust the input range!
    //                                                                ADS1015  ADS1115
    //                                                                -------  -------
    // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
    // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
    // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
    // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
    // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
    // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
    setGain(GAIN_FOUR);
    ready = Adafruit_ADS1X15::begin(i2c_address, wire);
    return ready;
}

void Stifler_voltmetr::set_delta(int max_mv, int max_adc_value){
    delta = float(max_mv) / float(max_adc_value);
}

float Stifler_voltmetr::read_voltage(){
    int raw_voltage = readADC_SingleEnded(ch_v);
    float result = raw_voltage * delta;    
    return result * 0.001;
}

bool Stifler_voltmetr::read_voltage(float *value){
    if (readADC_SingleEnded(ch_p) > max_d / 2){
        int raw_voltage = readADC_SingleEnded(ch_v);
        float result = raw_voltage * delta;    
        *value = float(result * 0.001);
        return true;
    }else{
        return false;
    }
}

bool Stifler_voltmetr::is_ready(){
    return ready;
}

bool Stifler_voltmetr::pm_voltage_amperage(float *for_value_voltage, float* for_value_amperage){
    bool ready = power_monitor.begin();
    if (ready){
        *for_value_voltage = power_monitor.getVoltage();
        *for_value_amperage = power_monitor.getCurrent();
        return ready;
    }
    return ready;
}