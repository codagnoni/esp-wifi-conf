#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

//** SoftAP settings
const IPAddress apIP(192, 168, 1, 1);

class MyWiFiClient {
    private:
        WiFiClient* _client;
        String _ssidList;
        DNSServer _dnsServer;
        ESP8266WebServer* webServer;
        const char* apSSID = "ESP8266_SETUP";
        String ssid = "";
        String pass = "";

    public:
        MyWiFiClient();
        ~MyWiFiClient();
        WiFiClient* getWiFiClient();
        void setupWifi();
        void setupWifiAP();
        boolean isConfigured();
        void startCaptivePortal();
        void captivePortalProcessRequest();
        void captivePortalHandleClient();
        void setSSID(String ssid) {
          this->ssid = ssid;
        }
        void setPass(String pass) {
          this->pass = pass;
        }
};

#endif // WIFI_H
