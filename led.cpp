#include "TM1638.h"
#include "disp.h"
#include "config.h"
#include "TimerOne.h"
#include "led.h"

TM1638 LED_Matrix(LED_MATRIX_PIN_1, LED_MATRIX_PIN_2, LED_MATRIX_PIN_3);

void LED_displayAll() {
  LED_Matrix.displayAll();
}

void LED_clearAll() {
  LED_Matrix.clear();
}

void LED_displayErr() {
  LED_Matrix.displayPic(picDatErr);
}

void ISR_WifiAnimation() { 
  static uint8_t count = 0;
  LED_Matrix.displayPic(picDatWifi[count % 4]);
  ++count;
}

void LED_startWifiAnimation() {
  Timer1.setPeriod(500000);
  Timer1.attachInterrupt(ISR_WifiAnimation); 
}

void LED_stopWifiAnimation() {
  Timer1.detachInterrupt();
  LED_Matrix.clear();
}

void ISR_DiscoverAnimation() { 
  static uint8_t count = 0;
  LED_Matrix.displayPic(picDatDiscover[count % 8]);
  ++count;
}

void LED_startDiscoverAnimation() {
  Timer1.setPeriod(200000);
  Timer1.attachInterrupt(ISR_DiscoverAnimation); 
}

void LED_stopDiscoverAnimation() {
  Timer1.detachInterrupt();
  LED_Matrix.clear();
}

void LED_printIP(IPAddress ip) {

    for (int i = 0; i < 4; i++) {
      uint8_t c = ip[i] % 10;
      uint8_t b = (ip[i] / 10) % 10;
      uint8_t a = (ip[i] / 100) % 10;
      
      if (a) { LED_Matrix.displayPic(picDatNum[a]); delay(250); LED_Matrix.clear(); delay(50);}
      if (b) { LED_Matrix.displayPic(picDatNum[b]); delay(250); LED_Matrix.clear(); delay(50);}
      LED_Matrix.displayPic(picDatNum[c]); delay(250); LED_Matrix.clear(); delay(50);
      if (i < 3) { LED_Matrix.displayPic(picDatDot); delay(250); LED_Matrix.clear(); delay(50);}
    }
}

void LED_printDigit(uint8_t num) {
  if (num > 10) {
    LED_Matrix.displayPic(picDatTenPlus);
  } else if (num == 10) {
    LED_Matrix.displayPic(picDatTen);
  } else {
    LED_Matrix.displayPic(picDatNum[num]);
  }
}
