#pragma once
#include <Arduino.h>
#include <GTimer.h>

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

