#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

#include <WiFi.h>

void LED_displayAll();
void LED_clearAll();
void LED_displayErr();
void LED_startWifiAnimation();
void LED_stopWifiAnimation();
void LED_printIP(IPAddress);
void LED_printDigit(uint8_t);
void LED_startDiscoverAnimation();
void LED_stopDiscoverAnimation();

#endif
