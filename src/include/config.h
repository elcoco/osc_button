#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Available GPIO's = 21, 17, 16, 18, 15, 3, 2, 1, 0, 44, 43, 20, 19, 48, 47, 46, 45, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33

// Time between blinks when triggered
#define BLINK_DUTYCYCLE_MS 500


struct Msg {
    const char *addr;    // OSC address
    const IPAddress ip;  // remote ip
    const int port;      // remote port
};

struct Led {
    uint8_t pin;
    struct Msg msg;
};

struct Button {
    uint8_t pin;
    uint8_t prev_state;
    struct Msg msg;
};


#ifdef TARGET_BOX1
  #define LOCAL_IP 10, 0, 0, 101
  #define LOCAL_PORT 8888
  #define MAC_ADDR 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED

  struct Msg msg_btn_start {
      .addr = "/button1/start",
      .ip = IPAddress(10, 0, 0, 255),
      .port = 52000
  };

  struct Msg msg_btn_stop {
      .addr = "/button1/stop",
      .ip = IPAddress(10, 0, 0, 255),
      .port = 52000
  };

  struct Msg msg_led_ready {
      .addr = "/led1/state",
      .ip = IPAddress(10, 0, 0, 255),
      .port = 52000
  };
#endif

#ifdef TARGET_BOX2
  #define LOCAL_IP 10, 0, 0, 201
  #define LOCAL_PORT 8888
  #define MAC_ADDR 0xD0, 0x84, 0x51, 0xD7, 0x1A, 0xCB

  struct Msg msg_btn_start {
      .addr = "/button2/start",
      .ip = IPAddress(10, 0, 0, 255),
      .port = 52000
  };

  struct Msg msg_btn_stop {
      .addr = "/button2/stop",
      .ip = IPAddress(10, 0, 0, 255),
      .port = 52000
  };

  struct Msg msg_led_ready {
      .addr = "/led2/state",
      .ip = IPAddress(10, 0, 0, 255),
      .port = 52000
  };
#endif


// Static IP/port of this device
const IPAddress local_ip(LOCAL_IP);
const int local_port = LOCAL_PORT;
byte mac[6] = {MAC_ADDR};

struct Led led_ready = { .pin = 2,
                         .msg = msg_led_ready
                       };

struct Button btn_start = { .pin = 0,
                            .msg = msg_btn_start,
                          };

struct Button btn_stop = { .pin = 1,
                           .msg = msg_btn_stop,
                         };

// Buttons only send data
struct Button *buttons[] = {&btn_start, &btn_stop, NULL};

// Leds only receive data
struct Led *leds[] = {&led_ready, NULL};

#endif // !CONFIG_H
