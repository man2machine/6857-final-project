
// Example of initializing and using the random number generator.


#include <iostream>
#include <chrono>

unsigned long micros()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

#include <Piduino.h>

#include <Crypto.h>
#include <RNG.h>
#include <TransistorNoiseSource.h>
//#include <RingOscillatorNoiseSource.h>

// Change "MyApp 1.0" to some other tag for your application
// so that different applications will generate different results
// even if the input noise or seed data is otherwise identical.
#define RNG_APP_TAG "MyApp 1.0"

// Noise source to seed the random number generator.
TransistorNoiseSource noise(A1);
//RingOscillatorNoiseSource noise;

bool calibrating = false;
byte data[32];
unsigned long startTime;
size_t length = 48; // First block should wait for the pool to fill up.

void setup() {
    
    std::cout << "start" << std::endl;

    // Initialize the random number generator.
    RNG.begin(RNG_APP_TAG);

    // Add the noise source to the list of sources known to RNG.
    RNG.addNoiseSource(noise);

    startTime = millis();
}

void printHex(const byte *data, unsigned len)
{
    static char const hexchars[] = "0123456789ABCDEF";
    unsigned long time = millis() - startTime;
    std::cout << time / 1000;
    std::cout << '.';
    std::cout << (time / 100) % 10;
    std::cout << ": ";
    while (len > 0) {
        int b = *data++;
        std::cout << hexchars[(b >> 4) & 0x0F];
        std::cout << hexchars[b & 0x0F];
        --len;
    }
    std::cout << std::endl;
}

void loop() {
    // Track changes to the calibration state on the noise source.
    bool newCalibrating = noise.calibrating();
    if (newCalibrating != calibrating) {
        calibrating = newCalibrating;
        if (calibrating)
            std::cout << "calibrating" << std::endl;
    }

    // Perform regular housekeeping on the random number generator.
    RNG.loop();

    // Generate output whenever 32 bytes of entropy have been accumulated.
    // The first time through, we wait for 48 bytes for a full entropy pool.
    if (RNG.available(length)) {
        RNG.rand(data, sizeof(data));
        printHex(data, sizeof(data));
        length = 32;
    }
}
