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

int dump_entries (map <DWORD, entry*> *dataset) {

	map <DWORD, entry*>::iterator itr;
	double begin;
	double end;
	
	for(itr = dataset->begin();itr!=dataset->end();itr++) {
		begin = itr->second->start  * TICK_LENGTH;
		end =  itr->second->start  * TICK_LENGTH + itr->second->length  * TICK_LENGTH;
		printf ("%u\t%u\t%u\t%s\n",  itr->first, itr->second->start,  itr->second->length, itr->second->label.c_str());
	}
	
	return (0);
}
