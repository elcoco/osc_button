#include <Arduino.h>

#include <Ethernet.h>
//#include <ETH.h>
#include <SPI.h>
//
//#include <ArduinoOSCEther.h>
#include <SPI.h>    
#include <ETH.h>            // Standard arduino library doesn't work with W5500 ethernet chip
#include <OSCMessage.h>


#include "pins_arduino.h"   // Find custom pin definitions in here
                            //

// Ethernet stuff
const IPAddress ip(192, 168, 2, 201);
const IPAddress gateway (192, 168, 2, 1);
const IPAddress subnet (255, 255, 255, 0);

const IPAddress dest_ip(192, 168, 2, 200);
const int dest_port = 55555;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };


void setup() 
{
    SPI.begin(ETH_CLK, ETH_MISO, ETH_MOSI);
    delay(1000);

    Ethernet.init(ETH_CS); 

    //Start the Ethernet connection
    Ethernet.begin(mac, ip);

    //Hardware check
    printf("Checking Ethernet hardware...\n");

    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        printf("ERROR: No Ethernet hardware detected!\n");
        return;
    }
    else {
        printf("Ethernet hardware detected!\n");
    }

    //Check if cable is connected

}

void wait_for_link()
{
    while (Ethernet.linkStatus() == LinkOFF) {
        printf("Waiting for link\n");
        delay(500);
    }
}

void loop() 
{
    wait_for_link();
    printf("Link is ON. Cable is connected.\n");

    //printf("ip = %s\n", Ethernet.localIP());
    delay(1000);
}

