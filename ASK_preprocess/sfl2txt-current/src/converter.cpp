// 	This file is part of SFL to TXT converter.
// 
//     SFL to TXT converter is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
// 
//     SFL to TXT converter is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with SFL to TXT converter .  If not, see <http://www.gnu.org/licenses/>.
//


#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include "converter.h"

using namespace std;

double TICK_LENGTH;

int main (int argc, char* argv[]) {

	char *cvalue = NULL;
	int c;

	string infile = "";
	string outfile = "";
	string in_file_type = "";
	string out_file_type = "";

	options config;
	config.sample_rate = 0;
	config.clone_filename = false;
	config.convert_from = "";
	config.convert_to = "";

	string::size_type idx;
 	TICK_LENGTH = (double) 1 / DEFAULT_SAMPLE_RATE;
	
	if (parse_config_file (&config)) {
		cerr <<  "# Found config file with presets: -r " <<  config.sample_rate 
		<< " -f " << config.convert_from 
		<< " -t " << config.convert_to;
		if (config.clone_filename)
			cerr << " -s";
		cerr << std::endl;
	};

	while ((c = getopt (argc, argv, "i:o:f:t:r:hvs")) != -1){
		stringstream ss;
		switch (c) {
			case 'i':
				cvalue = optarg;
				ss << cvalue;
				ss >> infile;

				idx = infile.rfind('.');
				if (idx!=string::npos) {
					in_file_type = infile.substr(idx+1);
				}

				break;
			case 'o':
				cvalue = optarg;
				ss << cvalue;
				ss >> outfile;

				idx = outfile.rfind('.');
				if (idx!=string::npos) {
					out_file_type = outfile.substr(idx+1);
				}

				break;
			case 'r':
				cvalue = optarg;
				ss << cvalue;
				ss >> config.sample_rate;
				TICK_LENGTH = (double) 1 / config.sample_rate;
				break;
			case 'f':
				cvalue = optarg;
				ss << cvalue;
				ss >> config.convert_from;
				break;
			case 't':
				cvalue = optarg;
				ss << cvalue;
				ss >> config.convert_to;
				break;
			case 'h':
				display_help_message();
				return(0);
				break;
			case 's':
				config.clone_filename = !config.clone_filename;
				break;
			case 'v':
				cerr << "# This is sfl2txt converter version " << THIS_VERSION << endl;
				return (0);
				break;
			default:
				break;
		}
	}


	// Set the filenames and types as appropriate

	if (config.convert_from.empty() && in_file_type.empty())
		config.convert_from = "sfl";
	if (config.convert_from.empty())
		config.convert_from = in_file_type;

	if (config.convert_to.empty() && out_file_type.empty())
		config.convert_to = "txt";
	if (config.convert_to.empty())
		config.convert_to = out_file_type;

	if (config.clone_filename) {
		idx = infile.rfind('.');
		if (idx!=string::npos) {
			outfile = infile.substr(0, idx) + "." + config.convert_to;
		} else {
			outfile = infile + "." + config.convert_to;
		}
	}

	if (config.convert_to == config.convert_from) {
		cerr << "# Error: To convert file to its own type use 'cp' or 'mv'" << endl;
		return (1);
	}

	// Display short banner to inform the user

	cerr << "# If in doubt, run 'sfl2txt -h' for brief help on use and options" << endl;
	cerr << "# Converting " << infile << "(" << config.convert_from <<  ") to " << outfile << "(" << config.convert_to << ") ";
	
	if (config.sample_rate) {
		cerr << "using frequency of " << config.sample_rate << " Hz:" << endl;
	} else {
		cerr << "using default frequency (" << DEFAULT_SAMPLE_RATE << " Hz):" << endl;
	}


	
	// Out there and back

	map <DWORD, entry*> markup;
	
	int flag = 0;

	if (config.convert_from == "txt" || config.convert_from == "TXT") {
		flag = read_from_txt (&markup, infile.data());
	} else if (config.convert_from == "sfl" || config.convert_from == "SFL") {
		flag = read_from_sfl (&markup, infile.data());
	} else if (config.convert_from == "srt" || config.convert_from == "SRT") {
		flag = read_from_srt (&markup, infile.data());
	} else {
		cerr << "# Error: Cannot import from '" << config.convert_from << "', unknown file type." << endl;
		return (1);
	}

	if (flag != 0) {
		cerr << "# Error opening infile, exiting" << endl;
		return (1);
	}

	if (config.convert_to == "txt" || config.convert_to == "TXT") {
		save_to_txt (&markup, outfile.data());
	} else if (config.convert_to == "srt" || config.convert_to == "SRT") {
		save_to_srt (&markup, outfile.data());
	} else if (config.convert_to == "sfl" || config.convert_to == "SFL") {
		write_to_sfl (&markup, outfile.data());
	} else {
		cerr << "# Error: Cannot export to '" << config.convert_to << "', unknown file type." << endl;
		return (1);
	}

//	dump_entries (&markup);

	// I hate it when valgrind reports memory leaks
	map <DWORD, entry*>::iterator itr;
	for(itr = markup.begin();itr!=markup.end();itr++) {
		delete (itr->second);
	}

	cerr << "# Conversion complete." << endl;

	return 0;

}

void display_help_message() {

	cerr << "# sfl2txt converter" << endl;
	cerr << "# Usage:" << endl;
	cerr << "\t sfl2txt [options...]" << endl;
	cerr << "# Options:" << endl;
	cerr << "\t -i <infile>  sets the input file, defaults to standard input;" << endl;
	cerr << "\t -o <outfile>  sets the output file, defaults to standard output;" << endl;
	cerr << "\t -s  generate output filename from input filename, replacing extension;" << endl;
	cerr << "\t -r <sample_rate>  sets the sample rate, defaults to "<<  DEFAULT_SAMPLE_RATE << " Hz;" << endl;
	cerr << "\t -f <source_format>  sets the type of input file, defaults to 'sfl';" << endl;
	cerr << "\t -t <target_format>  sets the type of output file, defaults to 'txt';" << endl;
	cerr << "\t -h  prints this message;" << endl;
	cerr << "\t -v  outputs the software's version;" << endl;
	cerr << "\t * Supported filetypes are: sfl, txt, srt (both lowercase and all-caps)" << endl;
	cerr << "\t ** For more details, refer to the sfl2txt manpage." << endl;

}

