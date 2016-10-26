#ifndef WIFI_H_INCLUDED 
#define WIFI_H_INCLUDED

#include <WiFi.h>
#include "statemachine.h"

#define OPCODE_DISCOVER { 0xde, 0xad, 0xbe, 0xef }

void wifi_connect();
IPAddress wifi_localIP();

void serv_start();
void serv_start_polltimer();
void serv_stop_polltimer();
bool serv_getdata(t_data_recv *);
bool serv_discover();

#endif
