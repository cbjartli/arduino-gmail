#include <avr/wdt.h>
#include <stdbool.h>
#include "util.h"
#include "config.h"

void resetDevice() {
  wdt_enable(WDTO_15MS); // Turn on watchdog...
  while (true) { };      // ...and wait for it to reset device.
}

void setColor(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(REDPIN, 255-r);
  analogWrite(GREENPIN, 255-g);
  analogWrite(BLUEPIN, 255-b);
}
