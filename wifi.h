#ifndef WIFI_H_INCLUDED 
#define WIFI_H_INCLUDED

#include <WiFi.h>
#include "statemachine.h"

#define OPCODE_DISCOVER { 0xde, 0xad, 0xbe, 0xef }

typedef struct __attribute__((packed)) {
  uint8_t opcode[4];
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t num;
} t_data_recv;

void wifi_connect();
int wifi_status();
IPAddress wifi_localIP();
bool serv_start();
void serv_start_polltimer();
void serv_stop_polltimer();
bool serv_getdata(t_data_recv *);
bool serv_discover();

#endif
