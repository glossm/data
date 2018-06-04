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

string srt_time_format (double time) {

	// Seconds to HH:MM:SS:sss

	int hour=time/3600;
	time=time - hour * 3600;
	int min=time/60;
	time = time - min * 60;
	int sec = time;
	time = time - sec;
	int hsec = time * 1000;

	char out [13];
	sprintf (out, "%02u:%02u:%02u,%-3.3u", hour, min, sec, hsec);

	stringstream ss;
	ss << out;
	return ss.str();
}

int save_to_txt (map <DWORD, entry*> *dataset, string outfile) {

	// Audacity uses a simple format for region labels:
	// start (seconds,double) end(seconds, double) label
	// field separator \t

	map <DWORD, entry*>::iterator itr;

	ostream *outf;
	ofstream *outff;

	if (outfile != "") {
		outff = new ofstream(outfile.data());
		outf = outff;
	} else {
		outf = &cout;
	}

	char buffer [128];
	double begin;
	double end;

	for(itr = dataset->begin();itr!=dataset->end();itr++) {
		begin = itr->second->start  * TICK_LENGTH;
		end =  itr->second->start  * TICK_LENGTH + itr->second->length  * TICK_LENGTH;
		sprintf (buffer, "%-3.6f\t%-3.6f\t", begin, end);
		*outf << buffer << itr->second->label << endl;
	}

	
	if (outf != &cout) {
		outff->close();
		delete(outff);
	}
	
	return (0);
}

int save_to_srt (map <DWORD, entry*> *dataset, string outfile) {

	map <DWORD, entry*>::iterator itr;

	ostream *outf;
	ofstream *outff;

	if (outfile != "") {
		outff = new ofstream(outfile.data());
		outf = outff;
	} else {
		outf = &cout;
	}

	int i = 1;
	string begin;
	string end;

	for(itr = dataset->begin();itr!=dataset->end();itr++) {
		begin =  srt_time_format ( itr->second->start * TICK_LENGTH );
		end =  srt_time_format ( itr->second->start  * TICK_LENGTH + itr->second->length  * TICK_LENGTH );
		*outf << i << "\r\n";
		*outf << begin << " --> " << end << "\r\n";
		*outf << itr->second->label << "\r\n";
		*outf << "\r\n";
		i++;
	}

	if (outf != &cout) {
		outff->close();
		delete(outff);
	}
	
	return (0);
}

