from modules import *
from pydub import AudioSegment
# import subprocess

# Set current working directory (cwd)
cwd_dir = os.getcwd()
cwd_dir = "/Volumes/mkhdd/glossm-data"

# Set input (raw) and output (preprocessed) directories based on cwd
pre_dir = os.path.join(cwd_dir, "preprocessed")
wav_dir = os.path.join(cwd_dir, "preprocessed", "wav")
mp3_dir = os.path.join(cwd_dir, "preprocessed", "mp3")

# Make directory if not exists
for directory in [pre_dir, wav_dir, mp3_dir]:
    if os.path.isdir(directory) is False:
        os.mkdir(directory)

# Get subdirectories of input files
wav_sub_dir = get_subdirectories(wav_dir)

# Make subdirectories of output files
mp3_sub_dir = [os.path.join(mp3_dir, sub_dir) for sub_dir in wav_sub_dir]

for directory in mp3_sub_dir:
    if os.path.isdir(directory) is False:
        os.mkdir(directory)

# For all the subdirectories in the wav directory ...
for sub_dir in wav_sub_dir:
    # ... and for all the wav files in the subdirectory ...
    for wav_file in get_subdirectories(os.path.join(wav_dir, sub_dir), 'wav'):
        wav_file_dir = os.path.join(wav_dir, sub_dir, wav_file)
        mp3_file_dir = os.path.join(mp3_dir, sub_dir, os.path.splitext(wav_file)[0]+'.mp3')

        print("Converting %s to mp3 ..." % wav_file)
        AudioSegment.from_wav(wav_file_dir).export(mp3_file_dir, format="mp3")

        # cmd = 'lame -V %s %s' % (wav_file_dir, mp3_file_dir)
        # subprocess.call(cmd, shell=True)
