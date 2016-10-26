#ifndef WIFI_H_INCLUDED 
#define WIFI_H_INCLUDED

#include <WiFi.h>
#include "statemachine.h"

void wifi_connect();
IPAddress wifi_localIP();

void serv_start();
void serv_start_polltimer();
void serv_stop_polltimer();
bool serv_getdata(t_data_recv *);

#endif
