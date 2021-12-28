// OptimizedProximity.ino

// NUS EE1111B Final project
// Qi Tianshi A0231151Y

// Source code for IR proximity sensor.


/// The digital pins for LED output.
const int ledPins[] = {12, 11, 10, 9, 8};

/// The look-up table mapping proximities to the upper bound of alpha.
///
/// The array stores the upper bound of alpha for each proximity. Use the index to find the
/// corresponding proximity.
const float proximityTable[] = {-1,
                                -1,
                                0.678742744705276,  // 2cm
                                0.810700052790296,  // 3cm
                                0.934213003998942,  // 4cm
                                1.01797674798544,   // 5cm
                                1.08700805748239,   // 6cm
                                1.15859353918255,   // 7cm
                                1.22453695613106,   // 8cm
                                1.27878485513828,   // 9cm
                                1.33123229213597,   // 10cm
                                1.37641306843758,   // 11cm
                                1.42489351423301,   // 12cm
                                1.46139431968389,   // 13cm
                                1.49447031018143};  // 14cm

/// The threshold value of alpha, below which the alternative proximity calculation function
/// is used to account for sensor saturation.
const float altFormulaThreshold = 0.874164227230965;

// Compiler macros for array sizes to avoid unnecessary computation and memory usage.
#define SIZEOF_ledPins 5
#define SIZEOF_proximityTable 15

/// Baseline, average, and amplitude voltages from IR receiver.
struct IRData {

  /// The voltage measured when the emitter is off. Defined as the highest voltage reading per measurement.
  const float baseline;

  /// The average of all voltage measurements.
  const float average;

  /// The voltage measured when the emitter is on. Defined as the average of readings less than the average of all readings.
  const float amplitude;

};


/// Converts a voltage reading from an analog range to its true voltage.
/// @param analogValue The value to be converted. Must be between 0 and 1024.
/// @param voltageHigh The upper bound voltage value, corresponding to 1024.
/// @return The voltage corresponding to the given analog value.
float analogToVoltage(float analogValue, float voltageHigh) {
  return (analogValue / 1024.0) * voltageHigh;
}

/// Extracts the baseline, average, and amplitude voltage from raw IR receiver data.
/// @param pin The input pin. Must be an analog pin.
/// @param numOfSamples The number of samples to be collected.
/// @return Extracted IR sensor data.
IRData extractFromReceiver(int pin, int numOfSamples) {

  int rawInput[numOfSamples];

  // Reads raw sensor input. Doing this in its own loop improves the consistency of the average voltage reading.
  for (int i = 0; i < numOfSamples; ++i) { rawInput[i] = analogRead(pin); }

  int maximum = -1;              // Will take the maximum of all readings (for finding baseline).
  long sum = 0;                  // Sum of all readings (for finding average).
  for (int i = 0; i < numOfSamples; ++i) {

    // Tracks the sum of all voltages to calculate average.
    sum += rawInput[i];

    // `baseline` takes the largest of all readings.
    if (rawInput[i] > maximum) { maximum = rawInput[i]; }

  }

  // Calculates average reading (in analog).
  const float averageLevel = sum / float(numOfSamples);

  // Calculates the amplitude voltage (average of readings less than average of all readings).
  long long amplitudeSum = 0;
  int amplitudeCount = 0;
  for (int i = 0; i < numOfSamples; ++i) {
    if (rawInput[i] < averageLevel) {

      amplitudeSum += rawInput[i];
      amplitudeCount ++;

    }
  }

  return {analogToVoltage(float(maximum), 5.0),                           // Baseline
          analogToVoltage(averageLevel, 5.0),                             // Average
          analogToVoltage(amplitudeSum / float(amplitudeCount), 5.0)};    // Amplitude

}

/// Calculates the proximity based on IR sensor data.
/// @param data Extracted IR sensor data.
/// @return Calculated proximity (in centimeters).
int calculateProximity(IRData data) {

  // Refuses to sense if baseline is too low (sensor will be saturated; inaccurate readings).
  if (data.baseline < 2.4) {
    Serial.print("Alpha null\t");
    return -1;
  }

  // Algorithm for transforming amplitude to account for baseline.
  float transformedAmplitude = pow(1 / (data.baseline - data.amplitude), 0.25) - 0.15 / data.baseline;
  if (transformedAmplitude < altFormulaThreshold) {               // Edge case for close proximities.
    Serial.print('T');
    transformedAmplitude = pow(1 / (data.baseline - data.amplitude), 0.25) - 0.5 / data.baseline;
  }

  Serial.print("Alpha ");
  Serial.print(transformedAmplitude, 4);
  Serial.print('\t');

  // Traverses down proximity look-up table to find correct proximity.
  for(int i = SIZEOF_proximityTable - 1; i >= 0; --i) {
    if (transformedAmplitude > proximityTable[i]) {
      return i + 1;                                       // Since the array stores the upper bound for each proximity.
    }
  }

}

/// Outputs a number in binary through LEDs.
/// @param value The value to be output.
void binaryWrite(byte value) {                       // Setting param type to byte automatically converts int value to binary.
  for (int i = 0; i < SIZEOF_ledPins; ++i) {
    digitalWrite(ledPins[i], bitRead(value, i) == 1 ? HIGH : LOW);    // For each bit, output HIGH or LOW depending on 1 or 0.
  }
}

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
