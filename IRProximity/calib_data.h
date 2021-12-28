// calib_data.h

// NUS EE1111B Final project
// Qi Tianshi A0231151Y

// Calibration data used for proximity calculation.


#define SIZEOF_proximityTable 15

/// The look-up table mapping proximities to the upper bound of alpha.
///
/// The array stores the upper bound of alpha for each proximity. Use
/// the index to find the corresponding proximity.
const float proximityTable[] = {
    -1,
    -1,
    0.678742744705276, // 2cm
    0.810700052790296, // 3cm
    0.934213003998942, // 4cm
    1.01797674798544,  // 5cm
    1.08700805748239,  // 6cm
    1.15859353918255,  // 7cm
    1.22453695613106,  // 8cm
    1.27878485513828,  // 9cm
    1.33123229213597,  // 10cm
    1.37641306843758,  // 11cm
    1.42489351423301,  // 12cm
    1.46139431968389,  // 13cm
    1.49447031018143   // 14cm
};

/// The threshold value of alpha, below which the alternative proximity
/// calculation function is used to account for sensor saturation.
const float altFormulaThreshold = 0.874164227230965;
