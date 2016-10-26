#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "wifi.h"
#include "config.h"
#include <TimerOne.h>
#include "statemachine.h"
#include "util.h"

const uint8_t opc_discover[4] = OPCODE_DISCOVER;

bool opcode_equals(const uint8_t *a, const uint8_t *b) {
    bool rc = true;

    for (int i = 0; i < MEMBER_SIZE(t_data_recv, opcode); ++i, ++a, ++b)
    {
        rc = rc && (*a == *b);
    }

    return rc;
}


WiFiUDP Udp;
IPAddress serverIP;

void wifi_connect() {
  WiFi.disconnect();

  while (WiFi.begin(WIFI_SSID, WIFI_PASS) != WL_CONNECTED) {
    delay(1000); // wait 1 second for connection:
  }
}

IPAddress wifi_localIP() {
    return WiFi.localIP();
}

int wifi_status() {
    return WiFi.status();
}

bool serv_start() {
  Udp.stop();
  return Udp.begin(LISTENPORT);
}

void ISR_poll_timeout() { 
    sm_emit(SIGTIMEOUT);
}

void serv_start_polltimer() {
  Timer1.setPeriod(LISTEN_TIMEOUT_US);
  Timer1.attachInterrupt(ISR_poll_timeout); 
}

void serv_stop_polltimer() {
  Timer1.detachInterrupt();
}

bool serv_getdata(t_data_recv *buf) {
  char rxbuf[UDP_TX_PACKET_MAX_SIZE];
  int packetSize = Udp.parsePacket();

  if (packetSize == 0) {
      return false;
  }

  Udp.read(rxbuf, sizeof(rxbuf));

  if (packetSize != sizeof(t_data_recv)) {
      return false;
  }

  memcpy(buf, rxbuf, sizeof(t_data_recv));

  return true;
}

bool serv_discover() {
  t_data_recv buf;
  memset(&buf, 0, sizeof buf);

  if (serv_getdata(&buf) && opcode_equals(buf.opcode, opc_discover)) {
    serverIP = Udp.remoteIP();
    return true;   
  } else {
    return false;
  }

}

