import os, subprocess

cwd_dir = os.getcwd()
cwd_dir = "/Volumes/mkhdd/glossm-data"

raw_dir = os.path.join(cwd_dir, "raw")
tag_dir = os.path.join(cwd_dir, "tag")
out_dir = os.path.join(cwd_dir, "out")

sfl_dir = os.path.join(raw_dir, "sfl")
wav_dir = os.path.join(raw_dir, "wav")

for directory in [raw_dir, sfl_dir, wav_dir, tag_dir, out_dir]:
    if os.path.isdir(directory) is False:
        os.mkdir(directory)

sfl_sub_dir = [sub_dir for sub_dir in os.listdir(sfl_dir) if sub_dir.startswith('.') is False]
wav_sub_dir = [sub_dir for sub_dir in os.listdir(wav_dir) if sub_dir.startswith('.') is False]

tag_sub_dir = [os.path.join(tag_dir, sub_dir) for sub_dir in sfl_sub_dir]

for sub_dir in sfl_sub_dir:
    input_dir = os.path.join(sfl_dir, sub_dir)
    output_dir = os.path.join(tag_dir, sub_dir)

    if os.path.isdir(output_dir) is False:
        os.mkdir(output_dir)

    sfl_files = [file for file in os.listdir(input_dir) if file.startswith('.') is False]

    for file in sfl_files:
        input_file = os.path.join(input_dir, file)
        output_file = os.path.join(output_dir, file).rstrip('sfl') + 'txt'
        if 'Bashkir' in file:
            sample_rate = '44100'
        elif 'Buryat' in file:
            sample_rate = '48000'
        else:
            sample_rate = '44100'
        print(file, sample_rate)
        subprocess.call(['./sfl2txt-current/sfl2txt', '-i', input_file, '-o', output_file, '-r', sample_rate])


for sub_dir in wav_sub_dir:
    i_dir = os.path.join(wav_dir, sub_dir)
    o_dir = os.path.join(out_dir, sub_dir)

    if os.path.isdir(o_dir) is False:
        os.mkdir(o_dir)

    wav_files = [file for file in os.listdir(i_dir) if file.startswith('.') is False]

    if sub_dir in os.listdir(tag_dir):
        t_dir = os.path.join(tag_dir, sub_dir)
        tag_files = [file for file in os.listdir(t_dir) if file.startswith('.') is False]

        for file in sorted(wav_files):
            i_file, i_type = os.path.splitext(file)

            for tag in tag_files:
                if tag.startswith(i_file) and 'Bashkir' in tag:
                    o_file = i_file
                    o_type = i_type.lower()

                    t_file, t_type = os.path.splitext(tag)

                    print("Splitting %s ..." % i_file + i_type)

                    i_file_full = os.path.join(i_dir, file)
                    t_file_full = os.path.join(t_dir, t_file+t_type)
                    arguments = [i_file_full, o_dir, o_file, o_type, t_file_full]
                    subprocess.call(['praat', '--run', 'datasplit.praat'] + arguments)