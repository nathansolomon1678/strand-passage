#ifndef Z_ANALYZER
#define Z_ANALYZER

#include <iostream>
#include <list>
#include <fstream>

#include "autocorr.h"
#include "clk.h"
#include "clkCigar.h"
#include "clkConformationAsList.h"
#include "clkConformationBfacf3.h"
#include "conformationAsList.h"
#include "genericConformation.h"

#define CRITICAL_Z 0.2134

class Analyzer {
	clkConformationBfacf3* knot;
	int target_length;
    double z_guess;

    int q;
    char* knot_filename;
    int num_warmup_steps;
    int steps_between_samples;
    int num_samples;

  public:
	bool add_initial_conformation(istream&);
	bool add_initial_conformation_from_file(char* filename);
    void get_knot_length(double& mean, double& std_dev, double& autocorr_coefficient, double z, double q, int num_samples, int steps_between_samples, int num_warmup_steps);
	void optimize(double target_length, double tolerance);

	Analyzer(char* filename, int warmup, int init_q);
};

#endif  // Z_ANALYZER
