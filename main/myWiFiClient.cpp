#include "myWiFiClient.h"

MyWiFiClient::MyWiFiClient() { this->_client = NULL; }

MyWiFiClient::~MyWiFiClient() {}

void MyWiFiClient::setupWifi() {
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(this->ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(this->ssid, this->pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}

boolean MyWiFiClient::isConfigured() {
  Serial.println("Reading EEPROM...");
  String ssid = "";
  String pass = "";
  if (EEPROM.read(0) != 0) {
    for (int i = 0; i < 32; ++i) {
      ssid += char(EEPROM.read(i));
    }
    Serial.print("SSID: ");
    Serial.println(ssid);
    for (int i = 32; i < 96; ++i) {
      pass += char(EEPROM.read(i));
    }
    Serial.print("Password: ");
    Serial.println(pass);
    setSSID(ssid);
    setPass(pass);
    return true;
  } else {
    Serial.println("Config not found.");
    return false;
  }
}

void MyWiFiClient::setupWifiAP() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(100);
  Serial.println("");
  for (int i = 0; i < n; ++i) {
    this->_ssidList += "<option value=\"";
    this->_ssidList += WiFi.SSID(i);
    this->_ssidList += "\">";
    this->_ssidList += WiFi.SSID(i);
    this->_ssidList += "</option>";
  }
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID);
  _dnsServer.start(53, "*", apIP);
  webServer = new ESP8266WebServer(80);
  this->startCaptivePortal();
  Serial.print("Starting Access Point at \"");
  Serial.print(apSSID);
  Serial.println("\"");
}

String makePage(String title, String contents) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}

String urlDecode(String input) {
  String s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}

void MyWiFiClient::startCaptivePortal() {
  Serial.print("Starting Web Server at ");
  Serial.println(WiFi.softAPIP());
  webServer->on("/settings", [this]() {
    String s = "<h1>Wi-Fi Settings</h1><p>Please enter your password by "
               "selecting the SSID.</p>";
    s += "<form method=\"get\" action=\"setap\"><label>SSID: </label><select "
         "name=\"ssid\">";
    s += _ssidList;
    s += "</select><br>Password: <input name=\"pass\" length=64 "
         "type=\"password\"><input type=\"submit\"></form>";
    webServer->send(200, "text/html", makePage("Wi-Fi Settings", s));
  });
  webServer->on("/setap", [this]() {
    for (int i = 0; i < 96; ++i) {
      EEPROM.write(i, 0);
    }
    String ssid = urlDecode(webServer->arg("ssid"));
    Serial.print("SSID: ");
    Serial.println(ssid);
    String pass = urlDecode(webServer->arg("pass"));
    Serial.print("Password: ");
    Serial.println(pass);
    Serial.println("Writing SSID to EEPROM...");
    for (int i = 0; i < ssid.length(); ++i) {
      EEPROM.write(i, ssid[i]);
    }
    Serial.println("Writing Password to EEPROM...");
    for (int i = 0; i < pass.length(); ++i) {
      EEPROM.write(32 + i, pass[i]);
    }
    EEPROM.commit();
    Serial.println("Write EEPROM done!");
    String s = "<h1>Setup complete.</h1><p>device will be connected to \"";
    s += ssid;
    s += "\" after the restart.";
    webServer->send(200, "text/html", makePage("Wi-Fi Settings", s));

    ESP.restart();
  });
  webServer->onNotFound([this]() {
    String s =
        "<h1>AP mode</h1><p><a href=\"/settings\">Wi-Fi Settings</a></p>";
    webServer->send(200, "text/html", makePage("AP mode", s));
  });

  webServer->begin();
}

void MyWiFiClient::captivePortalProcessRequest() {
  this->_dnsServer.processNextRequest();
}


void MyWiFiClient::captivePortalHandleClient() {
  this->webServer->handleClient();
}
