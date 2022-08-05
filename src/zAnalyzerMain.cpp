#include "zAnalyzer.h"
#include <string.h>
#include <stdlib.h>

void print_usage() {
	std::cout << "Usage:\n"
	          << "\tzAnalyzer input_file [operators/options]\n"
	          << "zAnalyzer takes an input file of cubic lattice conformations "
		      << "written in Rob Scharein's CUBE format and outputs a table of average length / Z "
		      << "vaue associations with the provided tolerance\n\n"
	          << "zAnalyzer operators:\n"
	          << "-l\ttarget length\n"
	          << "-tol\ttolerated error in avg length of the returned z-value\n"
	          << "-c\tbest guess for the number of BFACF steps needed between samples for independence\n"
	          << "-w\tbest guess for an appropriate warmup\n"
	          << "-q\tbest guess for an initial q value" << std::endl;
}

int main(int argc, char* argv[]) {
	char* infile = NULL, *outfile = NULL;
	int target_length = 0, tol = 0, warmup = 0, c = 0, q = 0;

	if (argc < 9) {
		print_usage();
		return 0;
	}

	infile = argv[1];

	for (int i=2; i < argc; i += 2) {
		if (!strcmp(argv[i], "-l")) {
			target_length = std::atoi(argv[i+1]);
		} else if (!strcmp(argv[i], "-tol")) {
			tol = std::atoi(argv[i+1]);
		} else if (!strcmp(argv[i], "-q")) {
			q = std::atoi(argv[i + 1]);
		} else if (!strcmp(argv[i], "-c")) {
			c = std::atoi(argv[i + 1]);
		} else if (!strcmp(argv[i], "-w")) {
			warmup = std::atoi(argv[i + 1]);
		} else {
			std::cout << "unrecognized operator/option. Terminating program..." << std::endl;
			return 0;
		}
	}
	
	Analyzer analyzer(infile, warmup, c, q);
	analyzer.optimize(target_length, tol);
	
	return 0;
}
