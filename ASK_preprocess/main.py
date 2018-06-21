from collections import defaultdict
from modules import *

# Set current working directory (cwd)
cwd_dir = os.getcwd()
# cwd_dir = "/Volumes/mkhdd/glossm-data"

# Set input (raw) and output (preprocessed) directories based on cwd
raw_dir = os.path.join(cwd_dir, "raw")
sfl_dir = os.path.join(cwd_dir, "raw", "sfl")
wav_dir = os.path.join(cwd_dir, "raw", "wav")

pre_dir = os.path.join(cwd_dir, "preprocessed")
tag_dir = os.path.join(cwd_dir, "preprocessed", "tag")
out_dir = os.path.join(cwd_dir, "preprocessed", "wav")
csv_dir = os.path.join(cwd_dir, "preprocessed", "csv")

# Make directory if not exists
for directory in [raw_dir, sfl_dir, wav_dir, pre_dir, tag_dir, out_dir, csv_dir]:
    if os.path.isdir(directory) is False:
        os.mkdir(directory)

# Get subdirectories of input files
sfl_sub_dir = get_subdirectories(sfl_dir)
wav_sub_dir = get_subdirectories(wav_dir)

# Make subdirectories of output files
tag_sub_dir = [os.path.join(tag_dir, sub_dir) for sub_dir in sfl_sub_dir]
out_sub_dir = [os.path.join(out_dir, sub_dir) for sub_dir in sfl_sub_dir]

for directory in tag_sub_dir + out_sub_dir:
    if os.path.isdir(directory) is False:
        os.mkdir(directory)

# For all the subdirectories in the sfl directory ...
for sub_dir in sfl_sub_dir:
    i_dir = os.path.join(sfl_dir, sub_dir)
    o_dir = os.path.join(tag_dir, sub_dir)

    # ... convert sfl files to txt files
    for file in get_subdirectories(i_dir, 'sfl'):
        print("Converting %s to txt ..." % (file))
        convert_sfl_to_txt(i_dir, o_dir, file)

# result_list will contain the names of all split files and their labels
result_dict = defaultdict(list)

# For all the subdirectories in the wav directory ...
for sub_dir in wav_sub_dir:
    if sub_dir in get_subdirectories(tag_dir):
        i_dir = os.path.join(wav_dir, sub_dir)
        o_dir = os.path.join(out_dir, sub_dir)
        t_dir = os.path.join(tag_dir, sub_dir)

        # ... and for all the wav files in the subdirectory ...
        for wav_file in get_subdirectories(i_dir, 'wav'):
            i_file, i_type, o_prefix, o_type = get_io_filenames(wav_file)

            # ... find matching txt files ...
            for tag_file in [item for item in get_subdirectories(t_dir) if item.startswith(i_file)]:
                i_file_full = os.path.join(i_dir, wav_file)
                t_file_full = os.path.join(t_dir, tag_file)

                # ... and split the wav files using Praat
                print("Splitting %s ..." % wav_file)
                praat_split_wav(i_file_full, t_file_full, o_dir, o_prefix, o_type)

                # ... and append the result list
                label_list = get_label_list(t_file_full)
                result_dict[sub_dir] += new_results(wav_file, label_list)

# Write results in a single csv file
write_results(csv_dir, result_dict)