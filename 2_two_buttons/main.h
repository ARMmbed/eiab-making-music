#include "mbed.h"       // this tells us to load mbed related functions

DigitalOut led1(LED1);         // we create a variable 'led1', use it as an out port
DigitalOut led2(LED2);         // we create a variable 'led2', use it as an out port

InterruptIn btn1(BUTTON1);     // we create a variable 'btn1', use it as an in port
InterruptIn btn2(BUTTON2);     // we create a variable 'btn2', use it as an in port

// YOUR CODE HERE

// this code runs when the microcontroller starts up
int main() {
    led1 = led2 = BUILT_IN_LED_OFF; // turn off led1 and led2 on startup

    btn1.fall(toggle_one);
    btn2.fall(toggle_two);

    // spin in a main loop. Wait for interrupts.
    while(1) {}
}
