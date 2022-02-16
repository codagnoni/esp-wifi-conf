ESP-8622 Simple Wifi configuration module
===========================================

# Dependency

This project depends on [ESP8266 support](https://github.com/esp8266/Arduino) for Arduino environment.


# How it works

At the very first boot (or whenever the EEPROM is cleared) it sets up a hot spot named _ESP8266\_SETUP_ that you can connect with an external device.

1. Booting and read Wifi config from EEPROM;
2. If Wifi config is not found, starts an AP with SSID "ESP8266_SETUP":
    1. Connect to this AP with your external device;
    2. Wifi Settings page will automatically open the Captive Portal;
    3. Select the SSID you want ESP to connect to, and set the password;
    4. Chosen SSID and password will be written to EEPROM, then ESP will reboot automatically.
3. If Wifi config is found:
    1. Setup will start Wifi as STA (client) and connect to configured SSID;
    2. It will get an IP address from DHCP;
    3. Now the ESP will be part of you LAN.
