#pragma once

#ifndef WLAN_SSID
#define WLAN_SSID     "SSID"
#endif
#ifndef WLAN_PASSWORD
#define WLAN_PASSWORD "Password"
#endif

// Fallback AP (если STA не поднялась)
#ifndef AP_PASSWORD
#define AP_PASSWORD   "12345678"
#endif

// Таймаут подключения к STA (мс)
#ifndef WIFI_CONNECT_TIMEOUT_MS
#define WIFI_CONNECT_TIMEOUT_MS 15000
#endif


#ifndef RELAY_PIN
#define RELAY_PIN 26
#endif
