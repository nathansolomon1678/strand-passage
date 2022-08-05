#include "zAnalyzer.h"

Analyzer::Analyzer(char* filename, int warmup, int C_steps, int Q) {
    knot_filename = filename;
	num_warmup_steps = warmup;
	steps_between_samples = C_steps;
	q = Q;
    z_guess = .18;
    num_samples = 1000;
}

bool Analyzer::add_initial_conformation(istream& is) {
    clkConformationAsList initial_conformation;
	if (!initial_conformation.readFromCoords(is)) {
        return false;
    }
    knot = new clkConformationBfacf3(initial_conformation);
    return true;
}

bool Analyzer::add_initial_conformation_from_file(char* filename){
	ifstream in;
	in.open(filename);
	if (!in) {
        std::cout << "ERROR: UNABLE TO OPEN FILE" << std::endl;
		return false;
	}
	return add_initial_conformation(in);
}

void Analyzer::get_knot_length(double& mean, double& std_dev, double z, double q, int num_samples, int steps_between_samples, int num_warmup_steps) {
	add_initial_conformation_from_file(knot_filename);
    std::vector<int> knot_lengths;
    // Telling the clkConformationBfacf3 object (*knot) what the z value is now,
    // instead of only specifying the z value in stepQ, allows it to precompute
    // transition probabilities, so stepQ can run slightly faster
    knot->setZ(z);
    knot->stepQ(num_warmup_steps, q, z);
    // Begin taking samples and saving the results in knot_lengths
    for (int i = 0; i < num_samples; ++i) {
        knot->stepQ(steps_between_samples, q, z);
        knot_lengths.push_back(knot->getComponent(0).size());
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
	for (int i = 0; i < num_samples; i++){
		total_squared_error += pow(mean - (double) knot_lengths[i], 2);
	}
	std_dev = sqrt(total_squared_error) / num_samples;

    std::cout << "With z=" << z << " and " << num_samples << " samples, the mean knot length is " << mean
              << " +/- " << 2. * std_dev << std::endl;
}

void Analyzer::optimize(double target_length, double tolerance) {
    double mean_length, std_dev_length;
    get_knot_length(mean_length, std_dev_length, z_guess, q, num_samples, steps_between_samples, num_warmup_steps);

	while (mean_length - 2. * std_dev_length < target_length - tolerance
        || mean_length + 2. * std_dev_length > target_length + tolerance) {
        // So long as the length is not yet close enough to the target length,
        // continue trying to optimize
        if (mean_length <= target_length && tolerance <= 2. * std_dev_length) {
            num_samples = (int) num_samples * pow(2. * std_dev_length / tolerance, 2);
        }
        z_guess += z_guess * (1. - z_guess / CRITICAL_Z) * (2. / (1. + mean_length / target_length) - 1.);
        get_knot_length(mean_length, std_dev_length, z_guess, q, num_samples, steps_between_samples, num_warmup_steps);
	}
}
