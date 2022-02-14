
#include "myWiFiClient.h"
#include <Arduino.h>
#include <EEPROM.h>

MyWiFiClient *wifi_client;
boolean configurationMode = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup..");
  EEPROM.begin(512);
  wifi_client = new MyWiFiClient();

  if (wifi_client->isConfigured()) {
    configurationMode = false;
    wifi_client->setupWifi();
  } else {
    configurationMode = true;
    wifi_client->setupWifiAP();
  }
}

void loop() {
  if (configurationMode) {
    wifi_client->captivePortalProcessRequest();
    wifi_client->captivePortalHandleClient();
  }
}