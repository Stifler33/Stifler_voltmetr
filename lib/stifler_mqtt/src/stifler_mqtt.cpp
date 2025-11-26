#include "stifler_mqtt.h"
#include <WiFi.h>


WiFiClient espClient;
PubSubClient client(espClient);
Preferences flash;
std::map<String, const char*> pub_topics;
std::map<String, const char*> sub_topics;
std::map<const char*, std::function<void(String)>> handlers;
GTimer<millis> tmr_mqtt_loop(1000, true, GTMode::Interval);

struct Subscribe_data
{
    String login;
    String password;
    String address;
    uint16_t port;
    // String topics[];
}sub_data;

void get_settings(){
    if (flash.begin(NAME_CONFIG_MQTT, true)){
        sub_data.address = flash.getString("address", "");
        sub_data.login = flash.getString("user", "");
        sub_data.password = flash.getString("password", "");
        sub_data.port = flash.getInt("port", 1883);
        flash.end();
    }else{
        Serial.println("open flash error");
    } 
}

void callback(char* topic, byte* payload, unsigned int length) {  
  String message;
  for (int i = 0; i < length; i++) {    
    message += (char)payload[i];
  }  

  for (const auto&pair: handlers){    
    if (String(pair.first) == String(topic)){
        if (pair.second != nullptr){
            pair.second(message);
        }        
    }
  }
}

void init_brocker(){
    get_settings();
    client.setServer(sub_data.address.c_str(), sub_data.port);
    client.setCallback(callback);
}

void subscribe_topics(){
    for (const auto& pair : sub_topics){        
        client.subscribe(pair.second);
    }
}

void reconnect() {  
  if (!client.connected()) {    
    // Попытка подключения
    if (client.connect("my_id", sub_data.login.c_str(), sub_data.password.c_str())) {         
      // Отправляем первое сообщение о статусе      
      client.publish(pub_topics["connect"], "ON");
      subscribe_topics();
    }
  }
}

bool loop_mqtt(){
    if (tmr_mqtt_loop && !client.connected()){
        reconnect();
    }
    return client.loop();
}

void add_pub_topic(String name_topic, const char* topic){
    pub_topics[name_topic] = topic;
}

void add_sub_topic(String name_topic, const char* topic, std::function<void(String)> handler){
    sub_topics[name_topic] = topic;
    handlers[topic] = handler;
}

void public_data(String name_topic, const char* payload){
    if (pub_topics.count(name_topic)){
        client.publish(pub_topics[name_topic], payload, true);
    }
}

void unsubscribe(const char *topic){
    client.unsubscribe(topic);
}