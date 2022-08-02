#include "zAnalyzer.h"
#include <string.h>
#include <stdlib.h>

void print_usage(){
	std::cout << "Usage:" << std::endl;
	std::cout << "\tzAnalyzer input_file [operators/options]" << std::endl;
	std::cout << "zAnalyzer takes an input file of cubic lattice conformations " << 
		"written in Rob Scharein's CUBE format and outputs a table of average length / Z " << 
		"vaue associations with the provided tolerance" << std::endl << endl;
	std::cout << "zAnalyzer operators:" << std::endl;
	std::cout << "-l\ttarget length" << std::endl;
	std::cout << "-tol\ttolerated error in avg length of the returned z-value" << std::endl;
	std::cout << "-zmin\tbest guess for Z value that is a lower bound for the target Z value" << std::endl;
	std::cout << "-zmax\tbest guess for Z valye that is an upper bound for the target Z value" << std::endl;
	std::cout << "-c\tbest guess for the number of BFACF steps needed between samples for independence" << std::endl;
	std::cout << "-w\tbest guess for an appropriate warmup" << std::endl;
	std::cout << "-q\tbest guess for an initial q value" << std::endl;
	std::cout << "-s\tset seed to a specific value (default is current system time)" << std::endl;
}

int main(int argc, char* argv[]){
	char* infile = NULL, *outfile = NULL;
	int target_length = 0, tol = 0, warmup = 0, c = 0, q = 0, seed = std::time(NULL);
	double z_min = 0, z_max = 0;

	if (argc < 9){
		print_usage();
		return 0;
	}

	infile = argv[1];

	for (int i=2; i < argc; i++){
		if (!strcmp(argv[i], "-l")){
			target_length = std::atoi(argv[i+1]);
			i++;
		} else if (!strcmp(argv[i], "-tol")){
			tol = std::atoi(argv[i+1]);
			i++;
		} else if (!strcmp(argv[i], "-zmax")){
			z_max = std::atof(argv[i + 1]);
			i++;
		} else if (!strcmp(argv[i], "-zmin")){
			z_min = std::atof(argv[i + 1]);
			i++;
		} else if (!strcmp(argv[i], "-q")){
			q = std::atoi(argv[i + 1]);
			i++;
		} else if (!strcmp(argv[i], "-c")){
			c = std::atoi(argv[i + 1]);
			i++;
		} else if (!strcmp(argv[i], "-w")){
			warmup = std::atoi(argv[i + 1]);
			i++;
		} else if (!strcmp(argv[i], "-s")){
			seed = std::atoi(argv[i + 1]);
			i++;
		} else {
			std::cout << "unrecognized operator/option. Terminating program...";
			return 0;
		}
	}

	if (z_min > z_max){
		std::cout << "zmin must be strictly greater than zmax. terminating program..." << std::endl;
		return 0;
	}
	
    std::srand(seed);
	Analyzer analyzer(infile, z_min, z_max, warmup, c, q);
	analyzer.z_from_length(target_length, tol);
	
	return 0;
}
