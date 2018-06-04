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


#ifndef H_CONVERTER_1283625045
#define H_CONVERTER_1283625045

#include <string>
#include <unistd.h>

using namespace std;

#define THIS_VERSION "0.7.0"

typedef unsigned int DWORD;

extern double TICK_LENGTH;

#define DEFAULT_SAMPLE_RATE 44100

#define LABEL_SIGNATURE 0x6C62616C
#define REGION_SIGNATURE 0x7478746C

#define RIFF_CHUNK_ID 0x46464952
#define SFPL_CHUNK_ID 0x4c504653
#define DATA_CHUNK_ID 0x61746164
#define CUE_CHUNK_ID 0x20657563
#define LIST_CHUNK_ID 0x5453494c
#define REGION_CHUNK_ID 0x206e6772
#define LABL_ID 0x6c62616c
#define LTXT_ID 0x7478746c
#define ADTL_ID 0x6c746461


struct riff_header {
	DWORD chunk_id;
	DWORD size;
	DWORD riff_type;
};

struct cue_header {
	DWORD chunk_id;
	DWORD size;
	DWORD length;
};

struct cue_entry {
	DWORD id;
	DWORD order;
	DWORD riff_id;
	DWORD chunk_start;
	DWORD block_start;
	DWORD sample_offset;
};

struct list_header {
	DWORD list_id;
	DWORD size;
	DWORD content_type;
};

struct region_entry {
	DWORD size;
	DWORD id;
	DWORD length;
	DWORD type2;
	DWORD wtf1;
	DWORD wtf2;
};

struct label_entry {
	DWORD size;
	DWORD id;
};

struct entry {
	DWORD start;
	DWORD length;
	string label;
};

struct options {
	int sample_rate;
	bool clone_filename;
	string convert_from;
	string convert_to;
};

int save_to_txt (map <DWORD, entry*> *dataset, string outfile);
int read_from_txt (map <DWORD, entry*> *dataset, string infile);

int save_to_srt (map <DWORD, entry*> *dataset, string outfile);
int read_from_srt (map <DWORD, entry*> *dataset, string infile);

int write_to_sfl (map <DWORD, entry*> *dataset, string outfile);
int read_from_sfl (map <DWORD, entry*> *dataset, string infile);

int dump_entries (map <DWORD, entry*> *dataset);

void display_help_message();

bool parse_config_file (options *data);

#endif

