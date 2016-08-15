#include "mbed.h"         // this tells us to load mbed related functions
#include "tones.h"        // list of all the tones and their frequencies
#include "FXOS8700CQ.h"   // library for the accelerometer

InterruptIn btn1(BUTTON1);     // we create a variable 'btn1', use it as an in port
InterruptIn btn2(BUTTON2);     // we create a variable 'btn2', use it as an in port

PwmOut buzzer(D3);             // our buzzer is a PWM output (pulse-width modulation)

Timeout tone_timeout;          // TimeOut = a interrupt to call a function after a specified delay

// Set up the accelerometer (this is specific to the onboard one)
InterruptIn accel_interrupt_pin(PTC13);
FXOS8700CQ accel(PTE25, PTE24, FXOS8700CQ_SLAVE_ADDR1);

static void silence() {
    buzzer.write(0.0f); // silence!
}

// this is our function that plays a tone.
// Takes in a tone frequency, and after duration (in ms.) we stop playing again
static void play_tone(int tone) {
    buzzer.period_us(1000000 / (tone));
    buzzer.write(0.10f); // 10% duty cycle, otherwise it's too loud
}

static void play_note1() {
    play_tone(NOTE_C4);
}
static void play_note2() {
    play_tone(NOTE_D4);
}

// YOUR CODE HERE

// this code runs when the microcontroller starts up
int main() {
    // play note when we fall
    btn1.fall(play_note1);
    btn2.fall(play_note2);

    // silence when we rise
    btn1.rise(silence);
    btn2.rise(silence);

    // accelerometer setup
    accel_interrupt_pin.fall(play_note3);
    accel_interrupt_pin.mode(PullUp);

    accel.config_int();      // enabled interrupts from accelerometer
    accel.config_feature();  // turn on motion detection
    accel.enable();          // enable accelerometer

}