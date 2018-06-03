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
#include <cstdlib>
#include "converter.h"

#define PRIMARY_LOCATION "/.sfl2txt.conf"
#define SECONDARY_LOCATION "/.config/sfl2txt.conf"



using namespace std;
/*
sample_rate=22050
clone_filename=yes
convert_from=sfl
convert_to=txt
; comment
[section]
*/

bool parse_config_file (options *data) {

	string buffer;
	double start;
	double end;
	string home_dir(getenv("HOME"));
	string config_file;
	
	ifstream inf;
	config_file += home_dir;
	config_file += PRIMARY_LOCATION;
	inf.open (config_file.data());

	if(inf.fail()) {
		config_file += home_dir;
		config_file += SECONDARY_LOCATION;
		inf.open (config_file.data());
		if(inf.fail()) {
			return (false);
		}
	}

	string tmp_s;
	while (getline(inf,buffer)) {
		// There are no parameters with ; or [ inside anyway.
		if  (buffer.find(";") == string::npos && buffer.find("[") ==  string::npos ) {

			stringstream ss (buffer);
			getline( ss, tmp_s, '=' );
			if (tmp_s == "sample_rate") {
				getline( ss, tmp_s, '=' );
				sscanf(tmp_s.c_str(), "%d", &data->sample_rate);
			}
			if (tmp_s == "clone_filename") {
				getline( ss, tmp_s, '=' );
				if (tmp_s == "yes") {
					data->clone_filename = true;
				} else {
					data->clone_filename = false;
				}
			}
			if (tmp_s == "convert_to") {
				getline( ss, tmp_s, '=' );
				data->convert_to = tmp_s;
			}
			if (tmp_s == "convert_from") {
				getline( ss, tmp_s, '=' );
				data->convert_from = tmp_s;
			}

		}

        }

	inf.close();

	return (true);
}