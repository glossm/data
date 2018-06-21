import os, subprocess, csv, datetime


def get_subdirectories(mother_directory, extension = None):
    if extension is None:
        dir_list = [sub_dir for sub_dir in os.listdir(mother_directory)
                    if not sub_dir.startswith('.') and not sub_dir.endswith('\r')]
    else:
        dir_list = [sub_dir for sub_dir in os.listdir(mother_directory)
                    if sub_dir.endswith(extension) or sub_dir.endswith(extension.upper())]
    return sorted(dir_list)

def line_prepender(filename, line):
    with open(filename, 'r+') as f:
        content = f.read()
        f.seek(0, 0)
        f.write(line.rstrip('\r\n') + '\n' + content)


def euckr_to_utf8(filename):
    with open(filename, 'r+') as f:
        content = f.read()
        f.seek(0, 0)
        try:
            f.write(unicode(content, 'euc-kr').encode('utf-8'))
        except UnicodeDecodeError:
            try:
                f.write(unicode(content, 'cp949').encode('utf-8'))
            except Exception as e:
                print(e)


def get_io_filenames(wav_file):
    i_file, i_type = os.path.splitext(wav_file)
    o_prefix = '_'.join(i_file.split('_')[:3])
    o_type = i_type.lower()

    return i_file, i_type, o_prefix, o_type


def convert_sfl_to_txt(input_dir, output_dir, file):
    input_file = os.path.join(input_dir, file)
    output_file = os.path.join(output_dir, file).rstrip('sfl') + 'txt'
    sample_rate = get_sample_rate(file)

    args = ['./sfl2txt-current/sfl2txt', '-i', input_file, '-o', output_file, '-r', sample_rate]
    subprocess.call(args)
    euckr_to_utf8(output_file)
    line_prepender(output_file, "start_time\tend_time\tlabel")


def get_sample_rate(file):
    sample_rate_dict = {'Bashkir': '44100',
                        'Dolgan': '44100',
                        'Buryat': '48000',
                        'Chulym': '48000',
                        'Khamnigan': '48000',
                        'Dagur': '22050',
                        'Kyrgyz': '22050',
                        }
    for k, v in sample_rate_dict.items():
        if k in file:
            return v
    else:
        print("Sample rate unknown! (default: 44100 Hz)")
        return '44100'

def praat_split_wav(i_file_full, t_file_full, o_dir, o_prefix, o_type):
    arguments = [i_file_full, o_dir, o_prefix, o_type, t_file_full]
    subprocess.call(['praat', '--run', 'datasplit.praat'] + arguments)


def get_label_list(t_file_full):
    with open(t_file_full, 'rU') as file2:
        tag_lines = file2.read().strip('\n ').split('\n')
        label_list = [line.split('\t') for line in tag_lines[1:]]

    return label_list

def new_results(wav_file, label_list):
    i_file, i_type, o_prefix, o_type = get_io_filenames(wav_file)
    new_result_list = []

    for l, label_line in enumerate(label_list):
        unsplit_file = i_file
        start_time = str(datetime.timedelta(seconds=float(label_line[0])))
        end_time = str(datetime.timedelta(seconds=float(label_line[1])))
        label = ''.join(label_line[2:])
        splitted_file = "%s_%04d%s" % (o_prefix, l, o_type)
        new_result_list.append([unsplit_file, splitted_file, start_time, end_time, label])

    return new_result_list


def write_results(csv_dir, result_dict):
    for k, v in result_dict.items():
        with open(os.path.join(csv_dir, k + ".csv"), "w") as file:
            enumerated_list = [["Number", "InputFile", "OutputFile", "StartTime", "EndTime", "Label"],] \
                              + [[i+1] + line for i, line in enumerate(v)]
            writer = csv.writer(file)
            writer.writerows(enumerated_list)