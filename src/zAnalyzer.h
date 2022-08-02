#ifndef Z_ANALYZER
#define Z_ANALYZER

#include <iostream>
#include <list>
#include <fstream>

#include "pseudorandom.h"
#include "autocorr.h"
#include "clk.h"
#include "clkCigar.h"
#include "clkConformationAsList.h"
#include "clkConformationBfacf3.h"
#include "conformationAsList.h"
#include "genericConformation.h"

#define CRITICAL_Z 0.2134

class search_data {
  public:
	double center, std_dev, std_dev_tol, z;
	int n;

	search_data(): center(0), std_dev(0), std_dev_tol(20), z(0), n(10000) {}
};

class Analyzer {
	search_data min, max, guess;
	double init_lower, init_upper;
	int target, q, w, n_components, c_steps;
	clkConformationBfacf3* knot;
	clkConformationAsList initialComp0;
	clkConformationAsList initialComp1;

  public:
	bool add_initial_conformation(istream&);
	bool add_initial_conformation_from_file(char* filename);
    void get_knot_length(double& mean, double& std_dev, double z, double q, int num_samples, int steps_between_samples, int num_warmup_steps);
	bool z_from_length(double target, double mean_tol);
	bool length_from_z(search_data* in);
	bool initialize_search(double mean_tol);
	bool check_overlap();
	void write_log(ostream);
	void read_table(istream);
	bool write_table(ofstream);

	Analyzer(char* filename, double Init_lower, double Init_upper, int warmup, int c, int init_q);
};

#endif  // Z_ANALYZER
