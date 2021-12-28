// calculate.h

// NUS EE1111B Final project
// Qi Tianshi A0231151Y

// Converts IR readings to proximities.


#include "calib_data.h"
#include "irdata.h"

/// Calculates the proximity based on IR sensor data.
/// @param data Extracted IR sensor data.
/// @return Calculated proximity (in centimeters).
int calculateProximity(IRData data) {

    // Refuses to sense if baseline is too low (sensor will be
    // saturated; inaccurate readings).
    if (data.baseline < 2.4) {
        Serial.print("Alpha null\t");
        return -1;
    }

    // Algorithm for transforming amplitude to account for baseline.
    float transformedAmplitude = pow(1 / (data.baseline - data.amplitude), 0.25) - 0.15 / data.baseline;

    // Edge case for close proximities.
    if (transformedAmplitude < altFormulaThreshold) {
        Serial.print('T');
        transformedAmplitude = pow(1 / (data.baseline - data.amplitude), 0.25) - 0.5 / data.baseline;
    }

    Serial.print("Alpha ");
    Serial.print(transformedAmplitude, 4);
    Serial.print('\t');

    // Traverses down proximity look-up table to find correct proximity.
    for (int i = SIZEOF_proximityTable - 1; i >= 0; --i) {
        if (transformedAmplitude > proximityTable[i]) {

            // Since the array stores the upper bound for each proximity.
            return i + 1;

        }
    }

}
