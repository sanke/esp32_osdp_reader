# ESP32 OSDP access control reader 

CP - OSDP control panel
PD - OSDP peripheral device

This is my little side project related to access control, basically this is a lib for arduino IDE
for ESP32 microcontroller (I used esp32-wroom) that enables it to act as an OSDP PD with access control reader capabilities. It's not a full implementation of OSDP protocol in any way
its just a concept that works with HID EP1501 (aka mercury ep1501) with basic functionality i.e. partial support for led, beeper, card identification. 


I haven't found any library that enables OSDP protocol for arduino IDE out of the box so I wrote one my self (used some structure definitions from https://github.com/goToMain/libosdp). 
Probably _libosdp_ could have been compiled against esp32 platform, but it seemed too heavy for my purpose
and I wanted to get familiar with OSDP protocol itself

## How it works

ESP32 microcontroller which acts as OSDP PD waits for identification card packet (basically raw card number) via bluetooth LE interface. After it receives one, appropriate status to OSDP CP is sent. CP checks access rights of a given card number and replies
with grant or deny. Based on OSDP reader configuration and hardware that you enabled you should see blinking led or hear beeping sound.


## Usage 

Won't dive into too much details, I assume user is familiar with OSDP and arduino IDE. 
For hardware I used _esp32-wroom_ along with rs485 module for communication with mrc ep1501 and some basic circuits for tri-state led and buzzer. Firmware uploading/compiling was done using arduino IDE. To use this code place contents of repo under `...\Documents\Arduino\libraries\osdp_reader`.
And your basic sketch should look like this (don't forget to assign appropriate pins values, Serial/UART interfaces):


```c++
#include <osdp_reader.h>

#define TX_MODE_PIN 5 // rs485 module DE, RE
//#define R_LED_PIN 33
//#define G_LED_PIN 25
//#define B_LED_PIN 26
//#define BUZ_PIN 14
#define RX2_PIN 16 // rs485 module RO 
#define TX2_PIN 17 // rs485 module DI

osdp_reader reader(Serial, Serial2, TX_MODE_PIN);

void setup() {

  Serial.begin(115200); // for debugging purposes
  Serial2.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN); // Serial2 is used for communicating with OSDP CP via rs485 
  reader.init();
}

void loop() {
  reader.loop();
}
```
NOTE: OSDP PD address is hardcoded to 0x01

Also there are _osdp_reader_ constructors that accept led, buzzer pin configuration




