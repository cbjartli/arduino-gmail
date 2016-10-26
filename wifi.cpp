#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "wifi.h"
#include "config.h"
#include <TimerOne.h>
#include "statemachine.h"


int status = WL_IDLE_STATUS;
WiFiUDP Udp;

void wifi_connect() {
  while (status != WL_CONNECTED) {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS); // Connect to WPA/WPA2 network
    delay(1000); // wait 1 second for connection:
  }
}

IPAddress wifi_localIP() {
    return WiFi.localIP();
}

void serv_start() {
  Udp.begin(LISTENPORT);
}

void ISR_poll_timeout() { 
    sm_emit(SIGTOUT);
}

void serv_start_polltimer() {
  Timer1.setPeriod(10000);
  Timer1.attachInterrupt(ISR_poll_timeout); 
}

void serv_stop_polltimer() {
  Timer1.detachInterrupt();
}

bool serv_getdata(t_data_recv *buf) {
  int len = Udp.read((char *)buf, sizeof(t_data_recv));

  return (len > 0);
}

