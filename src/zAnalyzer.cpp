#include "zAnalyzer.h"

Analyzer::Analyzer(char* filename, int warmup, int Q) {
    knot_filename = filename;
	num_warmup_steps = warmup;
	steps_between_samples = 1000;
	q = Q;
    z_guess = .18;
    num_samples = 1000;
}

/////////////////bool Analyzer::add_initial_conformation_from_file(char* filename) {
/////////////////    std::ifstream in;
/////////////////	in.open(filename);
/////////////////	if (!in) {
/////////////////        std::cout << "ERROR: UNABLE TO OPEN FILE" << std::endl;
/////////////////		return false;
/////////////////	}
/////////////////    clkConformationAsList initial_conformation;
/////////////////	if (!initial_conformation.readFromCoords(in)) {
/////////////////        return false;
/////////////////    }
/////////////////    knot = new CLK(initial_conformation);
/////////////////    return true;
/////////////////}

void Analyzer::get_knot_length(double& mean, double& std_dev, double& autocorr_coefficient,
                               double z, double q, int num_samples, int steps_between_samples, int num_warmup_steps) {
	///////////////////////add_initial_conformation_from_file(knot_filename);
    knot = new CLK();
    std::vector<int> knot_lengths;
    // Telling the CLK object (*knot) what the z value is now,
    // instead of only specifying the z value in bfacf_moves, allows it to precompute
    // transition probabilities, so bfacf_moves can run slightly faster
    knot->set_z(z);
    knot->set_q(q);
    knot->bfacf_moves(num_warmup_steps);
    // Begin taking samples and saving the results in knot_lengths
    for (int i = 0; i < num_samples; ++i) {
        knot->bfacf_moves(steps_between_samples);
        knot_lengths.push_back(knot->get_length());
    }
    
    // Compute the mean and standard deviation
    // Note that this is the standard deviation of the sample means,
    // which is the standard deviation of the knot lengths divided by
    // the square root of the number of samples
    int total = 0;
	for (int i = 0; i < num_samples; ++i) {
		total += knot_lengths[i];
	}
	mean = (double) total / (double) num_samples;

    double total_squared_error = 0;
	for (int i = 0; i < num_samples; i++) {
		total_squared_error += pow(mean - (double) knot_lengths[i], 2);
	}
	std_dev = sqrt(total_squared_error) / num_samples;
    //
    // Convert the knot lengths time series from `std::vector<int>` to `std::vector<double>`
    std::vector<double> signal(knot_lengths.begin(), knot_lengths.end());
    autocorr_coefficient = autocorrelation(signal, 1);

    std::cout << "\nz = " << z
              << "\nNumber of samples: " << num_samples
              << "\nSteps between samples: " << steps_between_samples
              << "\nMean knot length: " << mean << " +/- " << 2. * std_dev
              << "\nAutocorrelation coefficient (with lag = 1 sample): " << autocorr_coefficient
              << std::endl;
}

void Analyzer::optimize(double target_length, double tolerance) {
    double mean_length, std_dev_length, autocorr_coefficient;
    get_knot_length(mean_length, std_dev_length, autocorr_coefficient, z_guess, q, num_samples, steps_between_samples, num_warmup_steps);

	while (mean_length - 2. * std_dev_length < target_length - tolerance
        || mean_length + 2. * std_dev_length > target_length + tolerance) {
        // So long as the length is not yet close enough to the target length,
        // continue trying to optimize
        if (mean_length <= target_length && tolerance <= 2. * std_dev_length) {
            num_samples = (int) num_samples * pow(2. * std_dev_length / tolerance, 2);
        }
        if (autocorr_coefficient > .1) {
            steps_between_samples = (int) steps_between_samples * (1. + autocorr_coefficient);
        }
        z_guess += z_guess * (1. - z_guess / CRITICAL_Z) * (2. / (1. + mean_length / target_length) - 1.);
        get_knot_length(mean_length, std_dev_length, autocorr_coefficient, z_guess, q, num_samples, steps_between_samples, num_warmup_steps);
	}
}
