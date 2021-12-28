// irdata.h

// NUS EE1111B Final project
// Qi Tianshi A0231151Y

// Struct for storing data from the IR sensor.


/// Baseline, average, and amplitude voltages from IR receiver.
struct IRData {

    /// The voltage measured when the emitter is off. Defined as the
    /// highest voltage reading per measurement.
    const float baseline;

    /// The average of all voltage measurements.
    const float average;

    /// The voltage measured when the emitter is on. Defined as the
    /// average of readings less than the average of all readings.
    const float amplitude;

};
