#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Available GPIO's = 21, 17, 16, 18, 15, 3, 2, 1, 0, 44, 43, 20, 19, 48, 47, 46, 45, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33

// Time between blinks when triggered
#define BLINK_DUTYCYCLE_MS 500

// Override device specific settings in platformio.ini using build flags
    #ifndef ADDR_BASE
      #define ADDR_BASE "box0"
    #endif
    
    #ifndef REMOTE_PORT
      #define REMOTE_PORT 53000
    #endif
    
    #ifndef LOCAL_IP
      #define LOCAL_IP 10, 0, 0, 100
    #endif
    
    #ifndef MAC_ADDR
      #define MAC_ADDR 0xD0, 0x84, 0x51, 0xD7, 0x1A, 0xCB
    #endif
// End

struct Led {
    uint8_t pin;
    uint8_t blink_enabled;  // When 1, led blinks using timer
};

struct Button {
    uint8_t pin;
    const char *addr;    // OSC address
    uint8_t prev_state;
    struct Led *led;
    const IPAddress ip;  // remote ip
    const int port;      // remote port
};

// Static IP/port of this device
const IPAddress local_ip(LOCAL_IP);
const int local_port = 8888;
byte mac[6] = {MAC_ADDR};

struct Led led_start = { .pin = 2 };
struct Led led_stop  = { .pin = 3 };

struct Button btn_start = { .pin  = 0,
                            .addr = "/" ADDR_BASE "/start",
                            .led  = &led_start,
                            .ip = IPAddress(10, 0, 0, 255),  // 255 == everyone on network
                            .port = REMOTE_PORT };

struct Button btn_stop = { .pin  = 1,
                            .addr = "/" ADDR_BASE "/stop",
                            .led  = &led_stop,
                            .ip = IPAddress(10, 0, 0, 255),  // 255 == everyone on network
                            .port = REMOTE_PORT };

struct Button *buttons[] = {&btn_start, &btn_stop, NULL};

#endif // !CONFIG_H
