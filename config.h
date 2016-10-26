#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define REDPIN           3
#define GREENPIN         5
#define BLUEPIN          6

#define LED_MATRIX_PIN_1 0
#define LED_MATRIX_PIN_2 1
#define LED_MATRIX_PIN_3 2

#define WIFI_SSID        "BryggenResearch"
#define WIFI_PASS        "bras2016"

#define LISTENPORT       4000

#define LISTEN_TIMEOUT_US 5000
#define LISTEN_TIMEOUT_NO_DATA_THRESHOLD (15 * (1000000 / LISTEN_TIMEOUT_US))

#endif
