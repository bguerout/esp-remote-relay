// Inspired by https://blog.yavilevich.com/2020/09/convention-for-compile-time-configuration-of-platformio-projects/?utm_source=pocket_mylist
#pragma once

#ifndef JOUSSEHOME_WIFI_SSID
#error JOUSSEHOME_WIFI_SSID env variable is not set or vault is encrypted
#endif

#ifndef JOUSSEHOME_WIFI_PASSWORD
#error JOUSSEHOME_WIFI_PASSWORD env variable is not set or vault is encrypted
#endif

#ifndef JOUSSEHOME_RELAY_PIN
#error JOUSSEHOME_RELAY_PIN env variable is not set or vault is encrypted
#endif

#ifndef JOUSSEHOME_TELEGRAM_BOT_TOKEN
#error JOUSSEHOME_TELEGRAM_BOT_TOKEN env variable is not set or vault is encrypted
#endif
