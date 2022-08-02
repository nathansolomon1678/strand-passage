#include "zAnalyzer.h"

Analyzer::Analyzer(char* filename, double Init_lower, double Init_upper, int warmup, int C_steps, int Q) {
	add_initial_conformation_from_file(filename);
	init_lower = Init_lower;
	init_upper = Init_upper;
	w = warmup;
	c_steps = C_steps;
	q = Q;
}

bool Analyzer::add_initial_conformation(istream& is) {
	if (!initialComp0.readFromCoords(is)) {
        return false;
    }
	n_components = 1;
    if (initialComp1.readFromCoords(is)) {
        n_components = 2;
    }
    if (n_components == 2) {
        knot = new clkConformationBfacf3(initialComp0, initialComp1);
    } else if (n_components == 1) {
        knot = new clkConformationBfacf3(initialComp0);
    }
    knot->setZ(0);
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

bool Analyzer::z_from_length(double target_in, double mean_tol){
	target = target_in;
    std::cout << "\nInitializing Search..." << std::endl;
	initialize_search(mean_tol);
	// While the target is outside the guess window
	while (guess.center - guess.std_dev > target + mean_tol || guess.center + guess.std_dev < target - mean_tol) {
        std::cout << "=========================================================================\n"
                  << "Current Z-vals: " << min.z << "(" << min.center << ")  "
                  << guess.z << "(" << guess.center << ")  " << max.z << "(" << max.center << ")\n"
                  << "=========================================================================" << std::endl;
		if (target < guess.center) {
			max = guess;
			guess.z = exp((log(min.z) + log(guess.z)) / 2.);
			length_from_z(&guess);
			check_overlap();
		} else if (target > guess.center){
			min = guess;
			guess.z = exp((log(max.z) + log(guess.z)) / 2.);
			length_from_z(&guess);
			check_overlap();
		}
	}
    std::cout << "\nZ: " << guess.z << " Q: " << q << "Avg Length: " << guess.center << endl;
	return true;
}

void Analyzer::get_knot_length(double& mean, double& std_dev, double z, double q, int num_samples, int steps_between_samples, int num_warmup_steps) {
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
    int total = 0;
	for (int i = 0; i < num_samples; ++i) {
		total += knot_lengths[i];
	}
	mean = (double) total / num_samples;

    double variance = 0;
	for (int i = 0; i < num_samples; i++){
		variance += pow(mean - knot_lengths[i], 2) / num_samples;
	}
	std_dev = sqrt(variance / num_samples);

    std::cout << "With z=" << z << ", the mean knot length is " << mean
              << " and the standard deviation of the knot lengths is " << std_dev << std::endl;
}

bool Analyzer::length_from_z(search_data* in) {
    get_knot_length(in->center, in->std_dev, in->z, q, in->n, c_steps, w);
}

bool Analyzer::initialize_search(double mean_tol){
    // RESET
	min.center = min.std_dev = 0;
	guess.center = guess.std_dev = 0;
	max.center = max.std_dev;
	min.n = guess.n = max.n = 10000;
	min.std_dev_tol = guess.std_dev_tol = 25;

	//double init_lower = .1, init_upper = CRITICAL_Z;
	search_data temp;
	min.z = init_lower;
	max.z = init_upper;
	guess.z = exp((log(max.z) + log(min.z)) / 2);
    std::cout << "Warming up with " << w << " steps" << std::endl;
	length_from_z(&min);
	knot->stepQ(w, q, max.z);
	length_from_z(&max);
	while (max.center < target){
		q += 1;
	    length_from_z(&guess);
	}
	
	check_overlap();
	return true;
}

bool Analyzer::check_overlap() {
	search_data temp;
    std::cout << "Checking overlap..." << std::endl;
	while (guess.center + guess.std_dev >= max.center - max.std_dev){
		guess.std_dev_tol /= 2;
		length_from_z(&guess);
		if (guess.center + guess.std_dev >= max.center - max.std_dev) {
		    max.std_dev_tol /= 2;
		    length_from_z(&max);
        }
		while (min.center + min.std_dev >= guess.center - guess.std_dev) {
			min.std_dev_tol /= 2;
			length_from_z(&min);
			if (min.center + min.std_dev >= guess.center - guess.std_dev) {
			    guess.std_dev_tol /= 2;
			    length_from_z(&guess);
            }
			if (max.center < min.center) {
				temp = min;
				min = max;
				max = temp;
			}
			if (min.center > guess.center) {
				temp = min;
				min = guess;
				guess = temp;
			}
			if (max.center < guess.center) {
				temp = guess;
				guess = max;
				max = temp;
			}
			// if the target goes out of range of the search, purturbs the z values until it comes back into range
			while (min.center > target){
				min.z -= .0025;
				length_from_z(&min);
			}
			while (max.center < target){
				max.z += .0025;
				length_from_z(&max);
			}
		}
		return true;
	}
}
