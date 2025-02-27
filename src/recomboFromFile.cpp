#include "recomboFromFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

recomboFromFile::recomboFromFile(int Min_arc, int Max_arc, char* Infile, char* Outfile, int N_components, char Read_mode, int Sampling_mode, int Block_file_mode, bool Supress_output, bool Info_mode, int Seed, int Sequence_type, int Recombo_type){
	//set operating variables
	supress_output = Supress_output;
	min_arc = Min_arc;
	max_arc = Max_arc;
	read_mode = Read_mode;
	sequence_type = Sequence_type;
	recombo_type = Recombo_type;
	sampling_mode = Sampling_mode;
	block_file_mode = Block_file_mode;
	seed = Seed;

    if (seed != 0) {
        std::srand(seed);
	}
	else{
		seed = time(NULL);
		std::cout << "seed set to current system time: " << seed << std::endl;
	}
	//initialize infile_name and current_block_file_number, for use with block_file_mode == 1
	infile_name = new string(Infile);
	current_block_file_number = 0;
	//allocate uninitialized ifstream
	in = new ifstream;
	if (block_file_mode == 1){
		//increment infile_name
		inc_filename();
		//allocate and open output file
		out = new ofstream(Outfile, ios::out | ios::binary);
	}
	else{
		//open infile and allocate/open outfile
		in->open(Infile, ios::in | ios::binary);
		out = new ofstream(Outfile, ios::out | ios::binary);
	}
	//open info file if in info mode
	if (info_mode){
		stringstream ss;
		ss << Outfile << ".info";
		info_file = new ofstream(ss.str().c_str(), ios::out);
	}
	//open n_sites file (EXPERIMENTAL)
	stringstream tt;
	tt << Outfile << "_sites.b";
	sites_file = new ofstream(tt.str().c_str(), ios::out);

	n_components = N_components;
	if (!in->good())
		{std::cout << "Error reading input file" << std::endl; exit(3);}
	if (!out->good())
		{std::cout << "Error creating output file" << std::endl; exit(3);}
}

bool recomboFromFile::inc_filename(){
	//close current file if open
	in->close();
	//increment block file counter
	current_block_file_number++;
	//put the new filename together with a string stream
	stringstream ss;
	ss << *infile_name << '%' << current_block_file_number << ".b";
	//try to open new filename
	in->open(ss.str().c_str(), ios::out | ios::binary);
	//check if successful, return based on result
	if (in->good()){
		return true;
	}
	else{
		return false;
	}
}

string recomboFromFile::get_current_filename(){
	//put the new filename together with a string stream
	stringstream ss;
	ss << *infile_name << '%' << current_block_file_number << ".b";
	return ss.str();
}

bool recomboFromFile::read_comp_knots(ifstream* in){
	if (read_mode == 'b'){
		return initialComp0.readFromCube(*in);
	}
	else if (read_mode == 't'){
		return initialComp0.readFromText(*in);
	}
	else return false;
}

bool recomboFromFile::read_comp_links(ifstream* in){
	if (read_mode == 'b'){
		return (initialComp0.readFromCube(*in) && initialComp1.readFromCube(*in));
	}
	else if (read_mode == 't'){
		return (initialComp0.readFromText(*in) && initialComp1.readFromText(*in));
	}
	else return false;
}

void recomboFromFile::do_recombo(){
	if (sampling_mode == 0){
		sampling_mode = -1;
	}
	if (n_components == 1){
		std::cout << "doing recombo knots" << std::endl; 
		do_recombo_knots();}
	else if (n_components == 2){
		std::cout << "doing recombo links" << std::endl;
		do_recombo_links();}
	else{
	std::cout << std::endl << "error: Links with > 2 components are not currently supported by this software" << endl;
	exit(2);
	}
}

void recomboFromFile::writeSitesFile(clkConformationBfacf3* clk, int site_choice) {
	std::vector<threevector<int> >sitelist;
	sitelist = knot->getChosenSite(site_choice);

	if (knot->dirIsParal(site_choice) && n_components == 1) {
		for (int j = 0; j < 2; ++j) {
			recomboSites.addVertexBack(sitelist[j]);
		}
		for (int j = 3; j > 1; --j) {
			recomboSites.addVertexBack(sitelist[j]);
		}
	} else {
		for (int j = 0; j < 4; ++j) {
			recomboSites.addVertexBack(sitelist[j]);
		}
	}
	recomboSites.writeAsCube(*sites_file);
	recomboSites.clear();
}

void recomboFromFile::do_recombo_knots(){
	int attempts = 0, count = 0, sites = 0, choice = 0, length_counter = 0;
	long int total_attempts = 0, total_count = 0;
	vector<int> lengths;

	TOP:
	while (read_comp_knots(in) && sampling_mode != 0){
		sites = 0;
		if (!supress_output){
			std::cout << '\r' << "test Attempting Recombination: " << ++attempts << " Performed: " << count;
		}
		knot = new clkConformationBfacf3(initialComp0);
		//need to create new countRecomboSites function that is ideal for use with mmc
		int length = knot->getComponent(0).size();
		lengths.push_back(length);
		length_counter++;
		
        sites = knot->countRecomboSites(min_arc, max_arc);
		if(sites > 0){
			list<clkConformationAsList> components;
            //choice is the chosen site to perform recombo. Want to output this file
			choice = std::rand(0, sites); //sites, NOT sites-1

			//write sites file
            writeSitesFile(knot, choice);

			if (knot->performRecombination(*out, sequence_type, recombo_type, 1, choice)){
                knot->getComponents(components);
                list<clkConformationAsList>::const_iterator i;
                out->write("KnotPlot 1.0", 12);
                out->put((char) 12);
                out->put((char) 10);
                for (i = components.begin(); i != components.end(); i++) {
                    out->write("comp", 4);
                    i->writeAsCube(*out);
                }
                out->write("endf", 4);
                out->put((char) 10);
            }
			count++;
			if (sampling_mode > 0){
				sampling_mode--;
			}
		}
		delete knot;
	}
	//report on current file
	std::cout << "\nPerformed "<<count<<" recombinations on "<<attempts<< " conformations in file "<< get_current_filename() << std::endl;
	double sum = 0;
	for (int i=0; i < lengths.size(); i++){
        sum += lengths[i];
	}
	std::cout << "Length " << min_arc + max_arc << " conformations so far: " << length_counter << std::endl;
	std::cout << "Average length so far: " << sum / lengths.size() << std::endl;
	if (block_file_mode == 1){
		//copy current file statistics into totals
		total_attempts += attempts;
		total_count += count;
		attempts = 0;
		count = 0;
		//if bfm == 1, see if next block file exists
		if (inc_filename()){
			//next block file exists and is now open, return to TOP to process next file
			goto TOP;
		}
		else{
			//next block file does not exist, report statistics for all blocks
			std::cout << "\nPerformed " << total_count << " recombinations on " << total_attempts << " conformations." << std::endl;
		}
	}
}

void recomboFromFile::do_recombo_knots_all(){ //experimental
	int attempts = 0, count = 0, sites = 0, choice = 0, length_counter = 0;
	long int total_attempts = 0, total_count = 0;
	vector<int> lengths;
TOP:
	while (read_comp_knots(in) && sampling_mode != 0){
		sites = 0;
		if (!supress_output){
			std::cout << '\r' << "test Attempting Recombination: " << ++attempts << " Performed: " << count;
		}
		//knot.
		knot = new clkConformationBfacf3(initialComp0);
		//need to create new countRecomboSites function that is ideal for use with mmc
		/*int length = knot->getComponent(0).size();
		lengths.push_back(length);
		if (length == (min_arc + max_arc)){
			length_counter++;
			sites = knot->countRecomboSites(min_arc, max_arc);
		}*/
		sites = knot->countRecomboSites(min_arc, max_arc);
		//write sites to sites_file
		/*stringstream ss;
		ss << sites << '\n';
		sites_file->write(ss.str().c_str(),5);*/
		std::cout << sites << '\n';
		for (int i=0; i < sites; i++){
			list<clkConformationAsList> components;
			knot->performRecombination(*out, sequence_type, recombo_type, 1, i);
			knot->getComponents(components);
			list<clkConformationAsList>::const_iterator j;
			for (j = components.begin(); j != components.end(); j++)
			{
				j->writeAsCube(*out);
			}
			count++;
			knot->undoRecombination(*out, sequence_type, recombo_type, 0);
		}
		delete knot;
	}
	//report on current file
	std::cout << "\nPerformed " << count << " recombinations on " << attempts << " conformations in file " << get_current_filename() << std::endl;
	double sum = 0;
	for (int i = 0; i < lengths.size(); i++){
		sum += lengths[i];
	}
	//std::cout << "Length " << min_arc + max_arc << " conformations so far: " << length_counter << std::endl;
	//std::cout << "Average length so far: " << sum / lengths.size() << std::endl;
	if (block_file_mode == 1){
		//copy current file statistics into totals
		total_attempts += attempts;
		total_count += count;
		attempts = 0;
		count = 0;
		//if bfm == 1, see if next block file exists
		if (inc_filename()){
			//next block file exists and is now open, return to TOP to process next file
			goto TOP;
		}
		else{
			//next block file does not exist, report statistics for all blocks
			std::cout << "\nPerformed " << total_count << " recombinations on " << total_attempts << " conformations." << std::endl;
		}
	}
}


void recomboFromFile::do_recombo_links(){
	int attempts = 0, count = 0, sites = 0, choice = 0, length_counter = 0;
	long int total_attempts = 0, total_count = 0;
	vector<int> lengths;
	TOP:
	while (read_comp_links(in) && sampling_mode != 0){
	    sites = 0;
		if (!supress_output){
			std::cout << '\r' << "test Attempting Recombination: " << ++attempts << " Performed: " << count;
		}
		knot = new clkConformationBfacf3(initialComp0, initialComp1);
		//need to create new countRecomboSites function that is ideal for use with mmc
		int short_length = min(knot->getComponent(0).size(),knot->getComponent(1).size());
		int long_length = max(knot->getComponent(0).size(), knot->getComponent(1).size());
		lengths.push_back(short_length + long_length);
		if ((short_length >= min_arc) && (long_length <= max_arc)){
		    length_counter++;
            sites = knot->countRecomboSites(min_arc, max_arc);
            }
	//}
		//sites = knot->countRecomboSites(knot->getComponent(0).size()/2-4, knot->getComponent(0).size()/2+4);

		if(sites > 0){
			list<clkConformationAsList> components;
			choice = std::rand(0, sites-1);

			//write sites file
			writeSitesFile(knot, choice);

			if (knot->performRecombination(*out, sequence_type, recombo_type, 2, choice)) {
                knot->getComponents(components);
                list<clkConformationAsList>::const_iterator i;
                out->write("KnotPlot 1.0", 12);
                out->put((char) 12);
                out->put((char) 10);
                for (i = components.begin(); i != components.end(); i++) {
                    out->write("comp", 4);
                    i->writeAsCube(*out);
                }
                out->write("endf", 4);
                out->put((char) 10);
            }
			count++;
			if (sampling_mode > 0){
				sampling_mode--;
			}
		}
		delete knot;
	}
	//report on current file
	std::cout << "\nPerformed " << count << " recombinations on " << attempts << " conformations in file " << get_current_filename() << std::endl;
	double sum = 0;
	for (int i=0; i < lengths.size(); i++){
        sum += lengths[i];
	}
	std::cout << "Length " << min_arc + max_arc << " conformations so far: " << length_counter << std::endl;
	std::cout << "Average length so far: " << sum / lengths.size() << std::endl;
	if (block_file_mode == 1){
		//copy current file statistics into totals
		total_attempts += attempts;
		total_count += count;
		attempts = 0;
		count = 0;
		//if bfm == 1, see if next block file exists
		if (inc_filename()){
			//next block file exists and is now open, return to TOP to process next file
			goto TOP;
		}
		else{
			//next block file does not exist, report statistics for all blocks
			std::cout << "\nPerformed " << total_count << " recombinations on " << total_attempts << " conformations." << std::endl;
		}
	}
}

recomboFromFile::~recomboFromFile(){
	delete in;
	delete out;
	delete infile_name;
	if (info_mode){
		delete info_file;
	}
	delete sites_file;
}
