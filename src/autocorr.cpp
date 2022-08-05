#include "autocorr.h"

double square(double num) { return num * num; }

double autocorrelation(std::vector<double> signal, int lag) {
    // Step 1: compute the mean and variance of the signal
    double total = 0.;
    for (int i = 0; i < signal.size(); ++i) {
        total += signal[i];
    }
    const double mean = total / signal.size();

    double total_squared_error = 0.;
    for (int i = 0; i < signal.size(); ++i) {
        total_squared_error += square(signal[i] - mean);
    }
    const double variance = total_squared_error / signal.size();

    // Step 2: compute the autocovariance
    double total_squared_difference = 0.;
    for (int i = 0; i < signal.size() - lag; ++i) {
        total_squared_difference += (signal[i] - mean) * (signal[i + lag] - mean);
    }
    const double autocovariance = total_squared_difference / signal.size();
    
    // Normalize the autocovariance to get the Pearson's correlation coefficient
    // between the signal with no offset and the signal with on offset of `lag`
    return autocovariance / variance;
}
