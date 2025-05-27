#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Available GPIO's = 21, 17, 16, 18, 15, 3, 2, 1, 0, 44, 43, 20, 19, 48, 47, 46, 45, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33

// Time between blinks when triggered
#define BLINK_DUTYCYCLE_MS 500

#define MAX_OSC_MESSAGES 2  // maximum amount of messages to send per button press


struct Led {
    uint8_t pin;
    uint8_t blink_enabled;  // When 1, led blinks using timer
};

struct Msg {
    const char *addr;    // OSC address
    const IPAddress ip;  // remote ip
    const int port;      // remote port
};

struct Button {
    uint8_t pin;
    uint8_t prev_state;
    struct Led *led;
    struct Msg msgs[MAX_OSC_MESSAGES+1]; // array, delimited by NULL
};


#ifdef TARGET_BOX1
  #define LOCAL_IP 10, 0, 0, 101
  #define REMOTE_PORT 53000
  #define MAC_ADDR 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED

  #define OSC_NEXT_TARGET_IP 10, 0, 0, 100
  #define OSC_NEXT_TARGET_PORT 53000
  #define OSC_STOP_TARGET_IP 10, 0, 0, 255
  #define OSC_STOP_TARGET_PORT 53000
#endif
#ifdef TARGET_BOX2
  #define LOCAL_IP 10, 0, 0, 201
  #define REMOTE_PORT 53000
  #define MAC_ADDR 0xD0, 0x84, 0x51, 0xD7, 0x1A, 0xCB

  #define OSC_NEXT_TARGET_IP 10, 0, 0, 200
  #define OSC_NEXT_TARGET_PORT 53000
  #define OSC_STOP_TARGET_IP 10, 0, 0, 255
  #define OSC_STOP_TARGET_PORT 53000
#endif

struct Msg next_playhead_move {
    .addr = "/playhead/next",
    .ip = IPAddress(OSC_NEXT_TARGET_IP),
    .port = OSC_NEXT_TARGET_PORT
};

struct Msg next_playhead_start {
    .addr = "/cue/playhead/start",
    .ip = IPAddress(OSC_NEXT_TARGET_IP),
    .port = OSC_NEXT_TARGET_PORT
};

struct Msg stop_playhead_move {
    .addr = "/playhead/1",
    .ip = IPAddress(OSC_STOP_TARGET_IP),
    .port = OSC_STOP_TARGET_PORT
};

struct Msg stop_playhead_start {
    .addr = "/cue/playhead/start",
    .ip = IPAddress(OSC_STOP_TARGET_IP),
    .port = OSC_STOP_TARGET_PORT
};


// Static IP/port of this device
const IPAddress local_ip(LOCAL_IP);
const int local_port = 8888;
byte mac[6] = {MAC_ADDR};

struct Led led_start = { .pin = 2 };
struct Led led_stop  = { .pin = 3 };

struct Button btn_start = { .pin  = 0,
                            .led  = &led_start,
                            .msgs = { next_playhead_move,
                                      next_playhead_start,
                                      NULL }
                          };

struct Button btn_stop = { .pin  = 1,
                           .led  = &led_stop,
                           .msgs = { stop_playhead_move,
                                     stop_playhead_start,
                                     NULL }
                         };

struct Button *buttons[] = {&btn_start, &btn_stop, NULL};

#endif // !CONFIG_H
