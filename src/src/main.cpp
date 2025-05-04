#include <Arduino.h>

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <OSCMessage.h>

#include "esp32-hal-gpio.h"
#include "pins_arduino.h"   // Find custom pin definitions in here

#include "config.h"

/* ethernet.h     ->  https://docs.arduino.cc/libraries/ethernet/#Ethernet%20Class
 * example        ->  https://github.com/Johannf78/ESP32-W5500/blob/main/code/ESP32withW5500Try1.ino
 * board          ->  https://www.waveshare.com/wiki/ESP32-S3-ETH#Other_resource_link
 * schematics     ->  https://files.waveshare.com/wiki/ESP32-S3-ETH/ESP32-S3-ETH-Schematic.pdf
 * OSC example    ->  https://github.com/CNMAT/OSC/blob/master/examples/UDPSendMessage/UDPSendMessage.ino
 * OSC API        ->  https://github.com/CNMAT/OSC/blob/master/API.md
 * OSC debugging  ->  https://github.com/72nd/osc-utility
 */

EthernetUDP Udp;

// timer for led blinking
hw_timer_t *my_timer;

void isr_on_timer();
void wait_for_link();
void btn_init(struct Button *btn);
int  btn_check(struct Button *btn);
void buttons_debug();
void led_blink(struct Led *led, uint8_t n);
void osc_send_msg(struct Button *btn, int8_t msg);


void IRAM_ATTR isr_on_timer()
{
    // Do some blinking on leds with blink_enabled set
    for (struct Button **btn=buttons ; *btn; btn++) {
        if ((*btn)->led->blink_enabled)
              digitalWrite((*btn)->led->pin, !digitalRead((*btn)->led->pin));
    }
}

void wait_for_link()
{
    uint8_t was_connected = 1;

    while (Ethernet.linkStatus() == LinkOFF) {

        // Use ISR to blink leds
        for (struct Button **btn=buttons ; *btn; btn++)
            (*btn)->led->blink_enabled = 1;

        printf("Waiting for link\n");
        delay(500);
        was_connected = 0;
    }
    if (!was_connected)
        printf("Link is UP\n");

    // End in LED off state
    for (struct Button **btn=buttons ; *btn; btn++) {
        (*btn)->led->blink_enabled = 0;
        digitalWrite((*btn)->led->pin, LOW);
    }
}

void btn_init(struct Button *btn)
{
    pinMode(btn->pin, INPUT_PULLUP);
    btn->prev_state = 0;

    pinMode(btn->led->pin, OUTPUT);
    btn->led->blink_enabled = 0;
}

int btn_check(struct Button *btn)
{
    uint8_t state = digitalRead(btn->pin);

    // only trigger when previous state was not pressed
    if (state == LOW && btn->prev_state)
        asm("nop");
    else if (state == HIGH && btn->prev_state) {
        btn->prev_state = 0;
    }
    else if (state == LOW) {
        btn->prev_state = 1;
        return 1;
    }
    return 0;
}

void led_blink(struct Led *led, uint8_t n)
{
    for (uint8_t i=0 ; i<n ; i++) {
        digitalWrite(led->pin, 1);
        delay(10);
        digitalWrite(led->pin, 0);
        delay(BLINK_DUTYCYCLE_MS);
    }
}

void osc_send_msg(struct Button *btn, int8_t msg)
{
    printf("%s: triggered\n", btn->addr);
    OSCMessage osc_msg(btn->addr);
    osc_msg.add(msg);
    Udp.beginPacket(btn->ip, btn->port);
    osc_msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    osc_msg.empty(); // free space occupied by message
}

void buttons_debug()
{
    /* Print message at boot */
    printf("Local IP: %s\n", local_ip.toString().c_str());

    printf("Configured buttons:\n");
    for (struct Button **btn=buttons ; *btn; btn++)
        printf("  %s\t=> %s:%d\n", (*btn)->addr, (*btn)->ip.toString().c_str(), (*btn)->port);
}

void setup() 
{

    SPI.begin(ETH_CLK, ETH_MISO, ETH_MOSI);
    delay(1000);

    Ethernet.init(ETH_CS); 
    Ethernet.begin(mac, local_ip);
    Udp.begin(local_port);


    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        printf("ERROR: No Ethernet hardware detected!\n");
        while (1);
    }

    buttons_debug();
    wait_for_link();

    for (struct Button **btn=buttons ; *btn; btn++)
        btn_init(*btn);

    // Checks in ISR function if led blinking is enabled
    my_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(my_timer, &isr_on_timer, true);
    timerAlarmWrite(my_timer, BLINK_DUTYCYCLE_MS * 1000, true);
    timerAlarmEnable(my_timer);

}

void loop() 
{
    wait_for_link();
    for (struct Button **btn=buttons ; *btn; btn++) {
        if (btn_check(*btn)) {
            osc_send_msg(*btn, 1);
            led_blink((*btn)->led, 1);
        }
    }

    delay(10);
}
