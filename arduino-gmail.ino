/*******************************************************************************
 * 
 * Arduino-Gmail
 * ----------------
 * 
 * An example project for INFO361 / Advanced Topics in Human-Computer Interaction
 * University of Bergen, 2016
 * 
 ******************************************************************************/

#include "TimerOne.h"
#include "led.h"
#include "disp.h"
#include "config.h"
#include "util.h"
#include "wifi.h"
#include "statemachine.h"

/************************************ STATES **********************************/
t_state s_fatal_err, s_init, s_init_timer, s_init_wifi, s_init_udp, s_listen, s_receive, s_discover;

void s_fatal_err_enter() {
  LED_displayErr();
  delay(5000);
  resetDevice();
}

void s_init_enter() {
  MCUSR = 0; // Reset watchdog flags
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);

  LED_displayAll();
  setColor(255,255,255);
  delay(1000);
  LED_clearAll();
  setColor(0,0,0);
  sm_emit(SIGSUCC);
}

void s_init_timer_enter() {
  Timer1.initialize(); 
  sm_emit(SIGSUCC);
}

void s_init_wifi_enter() {
  LED_startWifiAnimation();
  wifi_connect(); // This blocks until connection is established
  sm_emit(SIGSUCC);
}

void s_init_wifi_exit() {
  LED_stopWifiAnimation();
}

void s_init_udp_enter() {
  LED_printIP(wifi_localIP());
  if (!serv_start()) {
    sm_emit(SIGERR);
  } else {
    sm_emit(SIGSUCC);
  }
}

void s_listen_enter() {
  serv_start_polltimer();
}

void s_listen_exit() {
  serv_stop_polltimer();
}

void s_listen_signal(t_sig *s) {
  t_data_recv buf;
  switch (s->type) {

    case sig_timeout:
      if (serv_getdata(&buf)) {
        setColor(buf.r, buf.g, buf.b);
        LED_printDigit(buf.num);
      } else if (wifi_status() != WL_CONNECTED) {
        sm_emit(SIGCONNLOST);
      } 
    
    default: 
      break;
  }
}

void s_discover_enter() {
    LED_startDiscoverAnimation();
    serv_discover();
    sm_emit(SIGSUCC);
}

void s_discover_exit() {
    LED_stopDiscoverAnimation();
}

void s_receive_enter() {
  t_data_recv buf;
  if (serv_getdata(&buf)) {
    setColor(buf.r, buf.g, buf.b);
    LED_printDigit(buf.num);
  } 

  sm_emit(SIGSUCC);
}

/********************************** END STATES ********************************/

void setup() {
  s_fatal_err   = { .onEnter       = s_fatal_err_enter, 
                    .onExit        = NULL, 
                    .onSignal      = NULL,
                    .transitions   = { 
                        .success   = NULL, 
                        .err       = NULL,
                        .timeout   = NULL,
                        .conn_lost = NULL}};

  s_init        = { .onEnter       = s_init_enter, 
                    .onExit        = NULL, 
                    .onSignal      = NULL,
                    .transitions   = { 
                        .success   = &s_init_timer, 
                        .err       = &s_fatal_err,
                        .timeout   = NULL,
                        .conn_lost = NULL}};

  s_init_timer  = { .onEnter       = s_init_timer_enter, 
                    .onExit        = NULL, 
                    .onSignal      = NULL,
                    .transitions   = { 
                        .success   = &s_init_wifi, 
                        .err       = &s_fatal_err,
                        .timeout   = NULL,
                        .conn_lost = NULL}};

  s_init_wifi   = { .onEnter       = s_init_wifi_enter, 
                    .onExit        = s_init_wifi_exit, 
                    .onSignal      = NULL,
                    .transitions   = { 
                        .success   = &s_init_udp, 
                        .err       = &s_fatal_err,
                        .timeout   = NULL,
                        .conn_lost = NULL}};

  s_init_udp    = { .onEnter       = s_init_udp_enter, 
                    .onExit        = NULL, 
                    .onSignal      = NULL,
                    .transitions   = { 
                        .success   = &s_discover, 
                        .err       = &s_fatal_err,
                        .timeout   = NULL,
                        .conn_lost = &s_init_udp}};

  s_discover    = { .onEnter       = s_discover_enter, 
                    .onExit        = s_discover_exit, 
                    .onSignal      = NULL,
                    .transitions   = { 
                        .success   = &s_listen, 
                        .err       = NULL,
                        .timeout   = NULL,
                        .conn_lost = NULL}};

  s_listen      = { .onEnter       = s_listen_enter,   
                    .onExit        = s_listen_exit, 
                    .onSignal      = s_listen_signal,
                    .transitions   = { 
                        .success   = NULL, 
                        .err       = &s_fatal_err,
                        .timeout   = NULL,
                        .conn_lost = NULL}};

sm_init(&s_init);   

}

void loop() {
  sm_process_signals();
}
