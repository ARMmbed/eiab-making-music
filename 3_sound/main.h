#include "mbed.h"       // this tells us to load mbed related functions
#include "tones.h"                   // list of all the tones and their frequencies

InterruptIn btn1(BUTTON1);           // we create a variable 'btn1', use it as an in port
InterruptIn btn2(BUTTON2);           // we create a variable 'btn2', use it as an in port

PwmOut buzzer(D3);                   // our buzzer is a PWM output (pulse-width modulation)

Timeout tone_timeout;                // TimeOut = a interrupt to call a function after a specified delay

static void silence() {
    buzzer.write(0.0f); // silence!
}

// this is our function that plays a tone.
// Takes in a tone frequency, and after duration (in ms.) we stop playing again
static void play_tone(int tone, int duration) {
    buzzer.period_us(1000000 / (tone));
    buzzer.write(0.10f); // 10% duty cycle, otherwise it's too loud

    // we wait for duration ms. and then call the silence function
    tone_timeout.attach_us(&silence, duration * 1000); // setup tone_timeout to call silence after duration ms
}

// YOUR CODE HERE

// this code runs when the microcontroller starts up
int main() {
    btn1.fall(play_note1);
    btn2.fall(play_note2);
}
