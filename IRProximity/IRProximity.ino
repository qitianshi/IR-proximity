// IRProximity.ino

// NUS EE1111B Final project
// Qi Tianshi A0231151Y

// Source code for IR proximity sensor.


#include "calculate.h"
#include "input.h"
#include "output.h"

void setup() {

    // Sets up serial monitor.
    Serial.begin(9600);
    Serial.println();

    // Sets up PWM output for the IR emitter.
    pinMode(3, OUTPUT);
    analogWrite(3, map(15, 0, 100, 0, 255));               // Maps duty cycle (0-100) to digital range (0-255).

    // Sets up pins for LED output.
    for (int i = 0; i < SIZEOF_ledPins; ++i) { pinMode(ledPins[i], OUTPUT); }

    // Sets up the analog pin with a higher sample frequency than the default 9.6 kHz.
    ADCSRA &= ~(bit(ADPS0) | bit(ADPS1) | bit(ADPS2));     // Clears the prescaler.
    ADCSRA |= bit(ADPS2);                                  // Sets the prescaler to 16 (bits ADPS2,ADPS1,ADPS0 = '0b100').

}

void loop() {

    // Reads and extracts data from IR receiver.
    IRData extractedData = extractFromReceiver(A0, 408);

    // Calculates and outputs proximity.
    const int proximity = calculateProximity(extractedData);
    binaryWrite(proximity == -1 ? 0 : proximity);

    // Serial monitor output for debugging.
    Serial.print("Prox ");
    Serial.print(proximity);
    Serial.print("\t\tBase ");
    Serial.print(extractedData.baseline, 4);
    Serial.print("\tAvg ");
    Serial.print(extractedData.average, 4);
    Serial.print("\tAmp ");
    Serial.print(extractedData.amplitude, 4);
    Serial.println();

    delay(1000);

}
