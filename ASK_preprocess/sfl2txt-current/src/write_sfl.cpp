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

int write_to_sfl (map <DWORD, entry*> *dataset, string outfile) {

	DWORD list_size = 0;
	DWORD cue_size = 0;
	DWORD riff_size = 0;
	DWORD labels = 0;

	map <DWORD, entry*>::iterator itr;

	vector <region_entry*> regions;
	vector <cue_entry*> cue_table;

	for(itr = dataset->begin();itr!=dataset->end();itr++) {

		if (itr->second->length > 0) {
			region_entry* rgn = new region_entry;
			rgn->size = 20;
			rgn->id = itr->first;
			rgn->length = itr->second->length;
			rgn->type2 = REGION_CHUNK_ID;
			rgn->wtf1 = 0;
			rgn->wtf2 = 0;
			regions.push_back (rgn);
			list_size += 28; // 4 type, 4 size, 20 length
		} 
			
			DWORD entry_length = 8 + 4 + itr->second->label.size() + 1;
			if (entry_length % 2) entry_length++;
			list_size+= entry_length;
			labels++;

			cue_entry* cue = new cue_entry;
			cue->id = itr->first;
			cue->order = itr->second->start;
			cue->riff_id = DATA_CHUNK_ID;
			cue->chunk_start = 0;
			cue->block_start = 0;
			cue-> sample_offset = itr->second->start;
			cue_table.push_back(cue);

	}


	list_size =  list_size + 4;
	cue_size = 24 * labels + 4;
	riff_size = list_size+ cue_size + 5*4;


	ostream *outf;
	ofstream *outff;

	if (outfile != "") {
		outff = new ofstream(outfile.data(), ios::out | ios::binary | ios::trunc);
		outf = outff;
	} else {
		outf = &cout;
	}

	riff_header* rfh = new riff_header;
	rfh->chunk_id = RIFF_CHUNK_ID;
	rfh->size = riff_size;
	rfh->riff_type = SFPL_CHUNK_ID;

	outf->write((char*) (rfh), sizeof(*rfh));

	delete rfh;	

	cue_header* cuh = new cue_header;
	cuh->chunk_id = CUE_CHUNK_ID;
	cuh->size = cue_size;
	cuh->length = labels;

	outf->write((char*) (cuh), sizeof(*cuh));

 	delete cuh;

	for (vector<cue_entry*>::iterator it = cue_table.begin(); it!=cue_table.end(); ++it) {
		outf->write((char*) (*it), sizeof *(*it));
	}


	list_header* lhd = new list_header;
	lhd->list_id = LIST_CHUNK_ID;
	lhd->size = list_size;
	lhd->content_type = ADTL_ID;

	outf->write((char*) (lhd), sizeof(*lhd));
	
 	delete lhd;

	DWORD ltxt = LTXT_ID;
	DWORD labl = LABL_ID;

	for (vector<region_entry*>::iterator it = regions.begin(); it!=regions.end(); ++it) {
		
		outf->write((char*) (&ltxt), sizeof (ltxt));
		outf->write((char*) (*it), sizeof *(*it));
	}

	
	DWORD field_size;

	for(itr = dataset->begin();itr!=dataset->end();itr++) {
		field_size =  itr->second->label.size() + 5;
		outf->write((char*) (&labl), sizeof (ltxt));
		outf->write((char*) (&field_size), sizeof (field_size));
		outf->write((char*) (&(itr->first)), sizeof (itr->first));
		outf->write(itr->second->label.data(), itr->second->label.size()+1);

		if (field_size % 2) {
			const char zero = '\0';
			outf->write(&zero, sizeof (char));
		}
	}

	if (outf != &cout) {
		outff->close();
		delete(outff);
	}


}

