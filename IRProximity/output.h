// output.h

// NUS EE1111B Final project
// Qi Tianshi A0231151Y

// Outputs measured proximity to LEDs (in binary).


#define SIZEOF_ledPins 5

/// The digital pins for LED output.
const int ledPins[] = {12, 11, 10, 9, 8};

/// Outputs a number in binary through LEDs.
/// @param value The value to be output.
void binaryWrite(byte value) {

    // Setting param type to byte automatically converts int to binary.

    // For each bit, outputs HIGH or LOW depending on 1 or 0.
    for (int i = 0; i < SIZEOF_ledPins; ++i) {
        digitalWrite(ledPins[i], bitRead(value, i) == 1 ? HIGH : LOW);
    }

}
