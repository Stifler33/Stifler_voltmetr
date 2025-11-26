#pragma once
#include <Arduino.h>
#include <GTimer.h>
#include <stifler_voltmetr.h>
#include <stifler_mqtt.h>
#include <Preferences.h>
#include <map>

extern std::map<int, float> calibration_values;

extern bool flag_map_volt;
extern int counter_measure;
const int max_counter_measure = 1024;

void init_output();
/**
 * rl_end
 * @param type "on" or "off"
 */
void switch_end_relay(String type);

void switch_pu_relay(String type);

void switch_plus_relay(String type);

void switch_minus_relay(String type);

void loop_relay();

void CC(String value);

void CV(String value);

void discharge(String value);

void map_volt(String value);

bool wait_voltage(float new_voltage);

void save_calibration_value(int value_duty, float value_voltage);

void get_calibration();

void check_flash_calib();