// output.h

// NUS EE1111B Final project
// Qi Tianshi A0231151Y

// Reads raw input from IR sensors.


#include "irdata.h"

/// Converts a voltage reading from an analog range to its true
/// voltage.
/// @param analogValue The value to be converted. Must be between
///                    0 and 1024.
/// @param voltageHigh The upper bound voltage value, corresponding to
///                    analog 1024.
/// @return The voltage corresponding to the given analog value.
float analogToVoltage(float analogValue, float voltageHigh) {
    return (analogValue / 1024.0) * voltageHigh;
}

/// Extracts the baseline, average, and amplitude voltage from raw IR
/// receiver data.
/// @param pin The input pin. Must be an analog pin.
/// @param numOfSamples The number of samples to be collected.
/// @return Extracted IR sensor data.
IRData extractFromReceiver(int pin, int numOfSamples) {

    int rawInput[numOfSamples];

    // Reads raw sensor input. Doing this in its own loop improves the
    // consistency of the average voltage reading.
    for (int i = 0; i < numOfSamples; ++i) {
        rawInput[i] = analogRead(pin);
    }

    int maximum = -1;              // For finding baseline.
    long sum = 0;                  // For finding average.
    for (int i = 0; i < numOfSamples; ++i) {

        // Tracks the sum of all voltages to calculate average.
        sum += rawInput[i];

        // `baseline` takes the largest of all readings.
        if (rawInput[i] > maximum) { maximum = rawInput[i]; }

    }

    // Calculates average reading (in analog).
    const float averageLevel = sum / float(numOfSamples);

    // Calculates the amplitude voltage (average of readings less than
    // average of all readings).
    long long amplitudeSum = 0;
    int amplitudeCount = 0;
    for (int i = 0; i < numOfSamples; ++i) {
        if (rawInput[i] < averageLevel) {

            amplitudeSum += rawInput[i];
            amplitudeCount ++;

        }
    }

    return {
        analogToVoltage(float(maximum), 5.0),                           // Baseline
        analogToVoltage(averageLevel, 5.0),                             // Average
        analogToVoltage(amplitudeSum / float(amplitudeCount), 5.0)      // Amplitude
    };

}
