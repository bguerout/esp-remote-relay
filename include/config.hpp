// Inspired by https://blog.yavilevich.com/2020/09/convention-for-compile-time-configuration-of-platformio-projects/?utm_source=pocket_mylist
#pragma once

#ifndef THERMOSTAT_WIFI_SSID
#error THERMOSTAT_WIFI_SSID env variable is not set
#endif

#ifndef THERMOSTAT_WIFI_PASSWORD
#error THERMOSTAT_WIFI_PASSWORD env variable is not set
#endif

#ifndef THERMOSTAT_RELAY_PIN
#error THERMOSTAT_RELAY_PIN env variable is not set
#endif
