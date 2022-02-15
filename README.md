ESP-8622 Simple Wifi configuration module
===========================================

# Dependency

This project depends on [ESP8266 support](https://github.com/esp8266/Arduino) on Arduino environment.


# How it works

At the very first boot (or whenever the EEPROM is cleared) it sets up a hot spot named _ESP8266\_SETUP_ that you can connect with an external device.

After connected, open http://192.168.1.1 at any browser and you'll be served with a captive portal that will present the list of WiFi networks available for the ESP.

Select the network of your desire, enter the password and hit the submit button. From now on, the ESP will always connect to that specific SSID.

