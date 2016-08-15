#include "mbed.h"
#include <string>
#include <sstream>
#include "security.h"
#include "easy-connect.h"
#include "simple-mbed-client.h"

using namespace std;

DigitalOut statusLed(LED1);
PwmOut buzzer(D3);                  // our buzzer is a PWM output (pulse-width modulation)

Ticker statusTicker;

static int BPM = 35;

SimpleMbedClient client;            // we define that we want to use mbed Client

// Declaring two cloud variables (both strings), first argument is the URL, second default value
SimpleResourceString notes = client.define_resource("buzzer/0/notes", "262:277");
SimpleResourceString duration = client.define_resource("buzzer/0/duration", "4:4");

// this is our function that plays a tone.
// Takes in a tone frequency, and after duration (in ms.) we stop playing again
static void playTone(int tone, int duration) {
    buzzer.period_us(1000000 / tone);  // period in us
    buzzer.write(0.10f);               // 10% duty cycle, otherwise it's too loud
    wait_us(1000 * duration / 2);      // play for half the length
    buzzer.write(0.0f);
    wait_us(1000 * duration / 2);      // silence for half the length
}

// Play song function, similar to what we had in the previous program
static void play_song(vector<int>* melody, vector<int>* duration) {

    printf("play_song was called with %d notes\n\r", melody->size());

    for (size_t i = 0; i < melody->size(); i++) {
        int tone = melody->at(0);
        // BPM is quarter notes per minute, so length in milliseconds is:
        int length = static_cast<int>(static_cast<float>(1000 / duration->at(0)) * (60000.0f / static_cast<float>(BPM * 1000)));

        // printf("tone %d, length %d, duration %d\r\n", tone, length, duration->at(0));

        if (melody->at(i) != 0) {
            playTone(melody->at(i), length);
        }
        else {
            buzzer = 0.0f;
            wait_ms(length);
        }
    }
}

static void blinkStatus() {
    statusLed = !statusLed;
}

static void onRegistered() {
    statusTicker.detach();

    statusLed = BUILT_IN_LED_ON;
}

// YOUR CODE HERE

int main() {
    statusTicker.attach(&blinkStatus, 1.0f);

    // besides cloud variables, we also have cloud functions.
    // first argument is the URL, second is the function we want to invoke.
    client.define_function("buzzer/0/play", &play_song_cloud);

    // connect to the network
    NetworkInterface* network = easy_connect(true);
    if (!network) {
        return 1;
    }

    // declare device properties, DeviceType is the type shown in mbed Device Connector
    struct MbedClientOptions options = client.get_default_options();
    options.DeviceType = "instrument";

    // connect to mbed Device Connector
    bool setup = client.setup(options, network);
    if (!setup) {
        printf("Connecting to mbed Device Connector failed...\n");
        return 1;
    }

    client.on_registered(&onRegistered);

    // every 25s. we send a keep alive
    while (1) {
        wait_ms(25000);
        client.keep_alive();
    }
}
