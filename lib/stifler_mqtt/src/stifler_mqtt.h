#pragma once

#include <PubSubClient.h>
#include <GTimer.h>
#include <Preferences.h>
#include <map>
#include <functional>

#define NAME_CONFIG_MQTT "config_mqtt"
extern std::map<String, const char*> pub_topics;
extern std::map<String, const char*> sub_topics;

/**
 * сохраняем топик для отправки в него данных
 * @param name_topic короткое имя топика
 * @param topic полное имя топика
 */
void add_pub_topic(String name_topic, const char* topic);

/**
 * сохраняем топики из которых мы получаем разные данные или команды
 * @param name_toic короткое имя топика
 * @param topic полное имя топика 
 * @param handler функция формата void(String) которая выполниется при поступлении данных в этот топик
 */
void add_sub_topic(String name_topic, const char* topic, std::function<void(String)> handler=nullptr);
// void add_handler(const char* topic, std::function<void()> handler);

void init_brocker();
bool loop_mqtt();

/**
 * Отправляем данные на сервер по имени топика который сохранили функцией add_pub_topic
 * @param name_topic имя топика
 * @param payload сообщение в текстовом формате
 */
void public_data(String name_topic, const char* payload);

void unsubscribe(const char *topic);