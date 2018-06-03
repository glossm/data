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
#include <vector>
#include "converter.h"

using namespace std;


int read_from_sfl (map <DWORD, entry*> *dataset, string infile) {

	map <DWORD, entry*>::iterator itr;

	riff_header* rfh = new riff_header;
	cue_header* cuh = new cue_header;

	istream *inf;
	ifstream *inff;

	if (infile != "") {
		inff = new ifstream(infile.data(), ios::in | ios::binary);
		inf = inff;
	} else {
		inf = &cin;
	}

	if(inf->fail()) {
		cerr << "# Error: Input file " << infile << " does not exist." << endl;
		return (1);
	}

	DWORD num;

	inf->read((char*) (rfh), sizeof(*rfh));
	
	inf->read((char*) (cuh), sizeof(*cuh));

	for (int i = 0; i < cuh->length; i++) {
		cue_entry* cue = new cue_entry;
		inf->read((char*) (cue), sizeof(*cue));
		(*dataset)[cue->id] = new entry;
		(*dataset)[cue->id]->start = cue->order;
		delete cue;
	}

	list_header* lsh = new list_header;
	inf->read((char*) (lsh), sizeof(*lsh));


	DWORD left = lsh->size - 4;

	DWORD current_type;
	
	while (left > 0){
	
		inf->read((char*) (&current_type), sizeof(current_type));

		if (current_type == REGION_SIGNATURE) {

			region_entry* rgn1 = new region_entry;
			inf->read((char*) (rgn1), sizeof(*rgn1));
	
			left -= sizeof (*rgn1) + 4;
			
			(*dataset)[rgn1->id]->length = rgn1->length;

			delete rgn1;

		} else if  (current_type == LABEL_SIGNATURE) {


			label_entry* lbl = new label_entry;
			inf->read((char*) (lbl), sizeof(*lbl));
			// Beware for zero-padding
			if (lbl->size % 2) {
				lbl->size++;
			}
			char* contents = new char [lbl->size - 4];
			inf->read((char*) (contents), (lbl->size - 4));
			
			left -= sizeof (*lbl) + lbl->size;
			(*dataset)[lbl->id]->label = contents;
			delete[] contents;
			delete lbl;

		} else {
			cerr << "# Error: encountered an unknown chunk type, aborting." << endl;
			return (1);
		}

	}

	delete rfh;
	delete cuh;
	delete lsh;

	if (inf != &cin) {
		inff->close();
		delete(inff);
	}

	return (0);
}

