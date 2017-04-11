#ifndef _ESP8266MQTTMESH_H_
#define _ESP8266MQTTMESH_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FS.h>
#include <functional>

class ESP8266MQTTMesh {
private:
    const String *networks;
    const char   *network_password;
    const char   *mesh_password;
    const String *base_ssid;
    const char   *mqtt_server;
    const int    mqtt_port;
    const int    mesh_port;

    WiFiClient espClient;
    WiFiServer espServer;
    PubSubClient mqttClient;
    String mySSID = "";
    long lastMsg = 0;
    char msg[50];
    int value = 0;
    bool meshConnect = false;
    unsigned long lastReconnect = 0;
    bool AP_ready = false;
    std::function<void(String topic, String msg)> callback;

    bool connected() { return (WiFi.status() == WL_CONNECTED); }
    void die() { while(1) {} }

    bool match_bssid(String bssid);
    void connect();
    void connect_mqtt();
    void setup_AP();
    int read_subdomain(String fileName);
    void assign_subdomain();
    void send_bssids(IPAddress ip);
    void handle_client_connection(WiFiClient client);
    void parse_message(String topic, String msg);
    void mqtt_callback(char* topic, byte* payload, unsigned int length);
    void send_message(IPAddress ip, String msg);
    void broadcast_message(String msg);
public:
    ESP8266MQTTMesh(const String *networks, const char *network_password, const char *mesh_password,
                    const String *base_ssid, const char *mqtt_server, int mqtt_port, int mesh_port);
    void setCallback(std::function<void(String topic, String msg)> _callback);
    void setup();
    void loop();
    void publish(String subtopic, String msg);
    static String getValue(String data, char separator, int index);
};
#endif //_ESP8266MQTTMESH_H_