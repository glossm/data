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
#include <cstring>
#include <map>
#include <sstream>
#include <iomanip>
#include "converter.h"

using namespace std;

int read_from_txt (map <DWORD, entry*> *dataset, string infile) {

	string buffer;
	double start;
	double end;
	string label;

	istream *inf;
	ifstream *inff;

	if (infile != "") {
		inff = new ifstream(infile.data());
		inf = inff;
	} else {
		inf = &cin;
	}

	if(inf->fail()) {
		cerr << "# Error: Input file " << infile << " does not exist." << endl;
		return (1);
	}

	DWORD id = 1;
	float tmp;
	string tmp_s;

	while (getline(*inf,buffer)) {
		entry* cur_entry = new entry;
		stringstream ss (buffer);

		getline( ss, tmp_s, '\t' );
		sscanf(tmp_s.c_str(), "%f", &tmp);
		cur_entry->start = int( tmp / TICK_LENGTH);

		getline( ss, tmp_s, '\t' );
		sscanf(tmp_s.c_str(), "%f", &tmp);
		cur_entry->length =  int( tmp / TICK_LENGTH) - cur_entry->start;

		getline( ss, tmp_s, '\t' );
		cur_entry->label = tmp_s;
		(*dataset)[id] = cur_entry;
		id++;
        }

	if (inf != &cin) {
		inff->close();
		delete(inff);
	}


	return (0);

}


int read_from_srt (map <DWORD, entry*> *dataset, string infile) {

	string buffer;
	double start;
	double end;
	int h_start;
	int m_start;
	int s_start;
	int ss_start;
	int h_end;
	int m_end;
	int s_end;
	int ss_end;

	istream *inf;
	ifstream *inff;

	if (infile != "") {
		inff = new ifstream(infile.data());
		inf = inff;
	} else {
		inf = &cin;
	}

	if(inf->fail()) {
		cerr << "# Error: Input file " << infile << " does not exist." << endl;
		return (1);
	}

	DWORD id;
	string label = "";

	while (getline(*inf,buffer)) {

		stringstream ss (buffer);
		entry* cur_entry = new entry;

		ss >> id;

		getline(*inf,buffer);
		sscanf(buffer.c_str(), "%u:%u:%u,%u --> %u:%u:%u,%u", &h_start, &m_start, &s_start, &ss_start, &h_end, &m_end, &s_end, &ss_end);

		start = h_start * 3600 + m_start * 60 + s_start + (double) ss_start / 1000;
		end = h_end * 3600 + m_end * 60 + s_end + (double) ss_end / 1000;

		cur_entry->start = int( start / TICK_LENGTH );
		cur_entry->length =  int( end / TICK_LENGTH ) - cur_entry->start;


		// label
		getline(*inf,buffer);

		// this is goddamn srt, so it uses \r\n
		while (buffer != "\r") {
			buffer.erase(buffer.length() - 1, 1);
			if (label.empty()) {
				label += buffer;
			} else {
				label += " " + buffer;
			}
			getline (*inf, buffer);
		}

		cur_entry->label = label;
		label = "";

		(*dataset)[id] = cur_entry;

        }

	if (inf != &cin) {
		inff->close();
		delete(inff);
	}


	return (0);

}
