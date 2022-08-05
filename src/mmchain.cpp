#include "mmchain.h"
#include <iostream>

using namespace std;

void mmchain::initialize(char* in, char* Outfile_name, double zmin, double zmax, int Q, double sr, int s, int N, int C, long int W, int M, char mode, int Seed,
	int Min_arc, int Max_arc, int Target_recombo_length, int bfs, int T, bool Supress_output, int Sequence_type, int Recombo_type){
	string infile;
	min_arc = Min_arc;
	max_arc = Max_arc;
	target_recombo_length = Target_recombo_length;
	infile.append(in);
	set_mmc(zmin, zmax, q, sr, s, mode, n, c, m, w);
	z_m = zmax; 
	z_1 = zmin;
	q = Q;
	target_swap_ratio = sr;
	swap_interval = s;
	sample_mode = mode;
	n = N;
	time_limit = T;
	c = C;
	m = M;
	w = W;
	seed = Seed;
	if (seed)
		srand(seed);
	else{
		seed = time(NULL); //set seed to current system time if unspecified
	};
	add_initial_conformation_from_file(infile);
	block_file_size = bfs;
	block_file_index = 0;
	outfile_name = Outfile_name;
	current_block_file_number = 0;
	supress_output = Supress_output;
	sample_attempts = 0;
	sequence_type = Sequence_type;
	recombo_type = Recombo_type;
}

void mmchain::create_config_file(){
	ofstream config("config.txt", ios::out);
	config << "initial_file= " << endl << "zmin= " << endl << "zmax= " << endl << "q= " << endl
		<< "target_swap_ratio= " << endl << "swap_interval= " << endl << "n_samples= " << endl 
		<< "steps_between_samples= " << endl << "initial_n_chains= 10" << endl
		<<"warmup= " << endl << "sample_mode= b"; 
	config.close();
}

bool mmchain::add_initial_conformation(istream& is){
	chain tempChain;
	int i=0;
	if (!initialComp0.readFromCoords(is))
      return false;
	 n_components = 1;
   if (initialComp1.readFromCoords(is))
      n_components = 2;

   if (n_components == 2){
	   for(i=0; i<m; i++){
		   tempChain.member_knot = new clkConformationBfacf3(initialComp0, initialComp1);
		   tempChain.member_knot->setSeed(time(NULL)); //what is this doing? PRESERVED(->setSeed(m+rand());)
		   chains.push_back(tempChain);
	   }}
   else if (n_components == 1){
		for (i=0;i<m;i++){
			tempChain.member_knot = new clkConformationBfacf3(initialComp0);
			tempChain.member_knot->setSeed(m+rand()); //what is this actually doing though?
			chains.push_back(tempChain);
		}}

   return true;
}

bool mmchain::add_initial_conformation_from_file(string& filename){
	ifstream in;
	in.open(filename.c_str(), ios::in);
	if (!in){
		cout << "ERROR: UNABLE TO OPEN INITIAL FILE" << endl;
		return false;
	}
	if (add_initial_conformation(in))
		return true;
	else
		return false;
}

void mmchain::set_mmc(double Z_1, double Z_m, int Q, double Target_swap_ratio, int Swap_interval, char Sample_mode, int num_samples, int iterations, int M, long int W){
	z_m = Z_m;
	z_1 = Z_1;
	q = Q;
	target_swap_ratio = Target_swap_ratio;
	swap_interval = Swap_interval;
	sample_mode = Sample_mode;
	n = num_samples;
	c = iterations;
	m = M;  
	w = W;
}

void mmchain::stamp_log(string buff){
	
	cout << "filename= " << outfile_name << endl;
	cout << "start_time= " << buff << endl;
	cout <<"zmin= "<< z_1 << endl;
	cout <<"zmax= "<< z_m << endl;
	cout <<"q= "<< q << endl;
	cout <<"target_swap_ratio= " << target_swap_ratio << endl;
	cout <<"swap_interval= "<<  swap_interval << endl;
	cout <<"n_samples= " << n << endl;
	cout <<"steps_between_samples= " << c << endl;
	cout << "initial_n_chains= " << m << endl;
	cout << "warmup= " << w << endl;
	cout << "sample_mode= " << sample_mode << endl;
	cout << "seed= " << seed << endl;
	cout << "block_file_size= " << block_file_size << endl;
	cout << "time_limit= " << time_limit << endl;
	cout << endl;
}

void mmchain::run_mmc() {
    // TODO: reference original code from github
}

void mmchain::swap(){ 
	int i;
	clkConformationBfacf3* temp;
	probs* p_temp;
	if (n_components == 1){ //tested
		i = rand() % (m-1); //generate random integer from 0 to m-1
			if((chains[i].member_knot->getComponent(0).size() < chains[i+1].member_knot->getComponent(0).size()) 
				|| (((double) rand() / (RAND_MAX)) < pow((double)(chains[i+1].z)/(chains[i].z), (chains[i].member_knot->getComponent(0).size() - chains[i+1].member_knot->getComponent(0).size())))){
					//perform swap
				p_temp = chains[i].member_knot->probMap;
				chains[i].member_knot->probMap = chains[i+1].member_knot->probMap;
				chains[i+1].member_knot->probMap = p_temp;
				temp = chains[i].member_knot;
				chains[i].member_knot = chains[i+1].member_knot;
				chains[i+1].member_knot = temp;
				intervals[i].attempted_swaps++;
				intervals[i].sucessful_swaps++;
			}
			else{ 
				intervals[i].attempted_swaps++;
			}

		}
	if (n_components == 2){ //rewritten for new swap condition, untested
		for (i = 0; i < m-1;i++){
			i = rand() % (m-1); //generate random integer from 0 to m-1
			//test code
			//int shouldBeLarger = chains[i].member_knot->getComponent(0).size() + chains[i].member_knot->getComponent(1).size();
			//int shouldBeSmaller = chains[i + 1].member_knot->getComponent(0).size() + chains[i + 1].member_knot->getComponent(1).size();
			if((chains[i].member_knot->getComponent(0).size() + chains[i].member_knot->getComponent(1).size()) 
				< (chains[i+1].member_knot->getComponent(0).size() + chains[i+1].member_knot->getComponent(1).size())
				|| (((double)rand() / (RAND_MAX)) < pow((double)(chains[i + 1].z) / (chains[i].z),
				((chains[i].member_knot->getComponent(0).size() + chains[i].member_knot->getComponent(1).size()) 
				- (chains[i+1].member_knot->getComponent(0).size() + chains[i+1].member_knot->getComponent(1).size()))))){
				//swap pre-computed bfacf probabilities
				p_temp = chains[i].member_knot->probMap;
				chains[i].member_knot->probMap = chains[i+1].member_knot->probMap;
				chains[i+1].member_knot->probMap = p_temp;
					//perform swap
				temp = chains[i].member_knot;
				chains[i].member_knot = chains[i+1].member_knot;
				chains[i+1].member_knot = temp;
				intervals[i].attempted_swaps++;
				intervals[i].sucessful_swaps++;
			}
			else 
				intervals[i].attempted_swaps++;
		}
	}
}

chain* mmchain::alloc_new_chain(double z){
	// allocate and configure a single new chain from the file
	chain* tempChain = new chain();

	if (n_components == 2){
		tempChain->member_knot = new clkConformationBfacf3(initialComp0,  initialComp1);
	}
	else{
		tempChain->member_knot = new clkConformationBfacf3(initialComp0);
	}
	tempChain->member_knot->setZ(z);
	tempChain->z = tempChain->member_knot->getZ();
	//warmup new chain
	tempChain->member_knot->stepQ(w, q, tempChain->z); 
	return tempChain; 
}

void mmchain::insert_new_chain(int i){
	cout << "	Adding new chain at location: " << i+1 << endl;
	//allocate new chain/interval and adjust all other chain data according
	chain* tempChain = alloc_new_chain(exp((log(chains[i].z) + log(chains[i+1].z))/2));

	//insert new chain at position i+1, pushing all other chains back by one
	m += 1;
		chains.push_back(*tempChain); 
	for (int j = m-2; j > i; j--){
		chains[j+1] = chains[j];
	}
	chains[i+1] = *tempChain;
	//create new interval, push into intervals, update delta_bs using update_intervals()
	interval_data tempInterval;
	intervals.push_back(tempInterval); 
	for (int j = m-3; j > i; j--){
		intervals[j+1] = intervals[j];
	}
	intervals[i+1] = tempInterval;
	update_intervals(); 
}

void mmchain::update_intervals(){ //possibly obsolete
	for (int i = 0; i < m-1; i++){
		intervals[i].delta_b = (log(chains[i].z) - log(chains[i+1].z));
	}
}

bool mmchain::test_intervals(){
	int test = 1; //true by default
	for (int i = 0; i < m-1 ; i++){
		//compute confidence intervals
		double p = intervals[i].sucessful_swaps/intervals[i].attempted_swaps;
		double z = 1.96; //95% confidence
		double cw = z*sqrt((1/intervals[i].attempted_swaps)*p*(1-p));
		cout << "Interval " << i << ": " << p - cw << ' ';
		if ((p - cw) < target_swap_ratio){
			insert_new_chain(i);
			i++;
			test = 0; //false when new chain needs to be allocated
		}
		else{
			cout << endl;
		}
		/*
		if (target_swap_ratio > p + cw){ //test confidence intervals
			//add new chain
			insert_new_chain(i);
			i++;
			test = 0; //false when new chain needs to be allocated
		}
		else if (target_swap_ratio < p - cw){
			system("PAUSE");
			//unhandled exception
		}*/
	}
	return test;
}

void mmchain::calibrate_chains(){
	cout << endl << "Starting calibration..." << endl;
	//Phase 1: Increase M as needed
	do{ 
		//reset interval data
		for (int i = 0; i < m-1; i++){
			intervals[i].attempted_swaps = 0;
			intervals[i].sucessful_swaps = 0;
		}
		//run chains for the same # of steps as a warmup
		cout << "Testing chains... " << endl;
		for(int i = 0; i < w; i++){ 
			for(int j = 0; j < m; j++){
				chains[j].member_knot->stepQ(swap_interval, q, chains[j].z);
			}
			if (m!=1){
				swap();
			}
		}
	}  while (!test_intervals()); //Compute and Check confidence intervals, add more chains as needed
}

void mmchain::display_statistics(){
    // TODO: print out autocorrelation info for each chain
    std::cout << std::endl;

	for (int i = 0; i < m; i++){
		float mean = 0;
		int var = 0;
		//expected value
		for (int j = 0; j < chains[i].data.size(); j++){
			mean += chains[i].data[j];
		}
		mean /= chains[i].data.size();
		//var
		for (int j = 0; j < chains[i].data.size(); j++){
			var += pow(chains[i].data[j] - mean, 2);
		}
		var /= (chains[i].data.size() - 1);
		//display results
        std::cout << chains[i].z << ' ' << mean << ' ' << var << std::endl;
	}
}

void mmchain::writeSitesFile(clkConformationBfacf3* clk, int site_choice){
	std::vector<threevector<int> >sitelist;
	sitelist = clk->getChosenSite(site_choice);

	//reverse the order of 3rd and 4th coordinates so it pass bad_increment test under 'p'
	if ((clk->dirIsParal(site_choice) && n_components == 1) || (clk->dirIsParal(site_choice) && n_components == 2)){
		recomboSites.addVertexBack(sitelist[0]); //newly added Diwen 07/11/18
		recomboSites.addVertexBack(sitelist[1]);
		recomboSites.addVertexBack(sitelist[3]);
		recomboSites.addVertexBack(sitelist[2]);
	}
	else {
		for (int j = 0; j < 4; ++j) {
			recomboSites.addVertexBack(sitelist[j]);
		}
	}

	recomboSites.writeAsCube(out3);
	recomboSites.clear();
}

void mmchain::write_to_block_file(clkConformationBfacf3* clk){
    std::cout << clk->getComponent(0).size() << std::endl;
	if ((block_file_index < block_file_size) && (block_file_index != 0)){
		//write conformation to file
		if (n_components == 1){
			clkConformationAsList toPrint(clk->getComponent(0));
			toPrint.writeAsCube(out);
		}
		else if (n_components == 2){
			clkConformationAsList toPrint(clk->getComponent(0));
			clkConformationAsList toPrint2(clk->getComponent(1));
			toPrint.writeAsCube(out);
			toPrint2.writeAsCube(out);
		}
		else{
			cout << endl << "ERROR: write_to_block_file(): n_components=" << n_components << ", only 1 or 2 component links supported" << endl;
			exit(1);
		}
		block_file_index++;
	}
	else
	{
		//close current file if open
		out.close();
		//increment file name
		current_block_file_number++;
		//reset block file index
		block_file_index = 0;
		stringstream ss;
		ss << outfile_name << 'n' << current_block_file_number << ".b";
		//open new file
		out.open(ss.str().c_str(), ios::out | ios::binary);

		//if in block mean sampling mode, keep .info file synchronized 
		info_file.close();
		if (sample_mode == 'm'){
			stringstream ss;
			ss << outfile_name << 'n' << current_block_file_number << ".info";
			//open new file
			info_file.open(ss.str().c_str(), ios::out);
		}

		//write conformation to file
		if (n_components == 1){
			clkConformationAsList toPrint(clk->getComponent(0));
			toPrint.writeAsCube(out);
		}
		else if (n_components == 2){
			clkConformationAsList toPrint(clk->getComponent(0));
			clkConformationAsList toPrint2(clk->getComponent(1));
			toPrint.writeAsCube(out);
			toPrint2.writeAsCube(out);
		}
		else{
			cout << endl << "ERROR: write_to_block_file(): n_components=" << n_components << ", only 1 or 2 component links supported" << endl;
			exit(1);
		}
		block_file_index++;
	}
}

int mmchain::count_recombo_sites(clkConformationBfacf3* clk, int& Para_site, int& Anti_site){
	int length = 0, sites = 0, recombination = 0;
	sites = clk->countRecomboSites(min_arc, max_arc, sequence_type, recombo_type, total_para_site, total_anti_site, Para_site, Anti_site);
	if (n_components == 1){
		length = clk->getComponent(0).size();
	}
	else if (n_components == 2){
		length = clk->getComponent(0).size() + clk->getComponent(1).size();
	}
	if (target_recombo_length > 0) {
		if (length == target_recombo_length){
			recombination = sites;
		}
	}
	else{
		recombination = sites;
	}
	return recombination;
}
