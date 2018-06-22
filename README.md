# data
Preliminary data for Glossm. Please import `csv` files in `ASK_for_glossm/` by following order.

* TopicSet
* Topic
* Meaning
* Language

## Pre-processing

`ASK_preprocess/` contains codes for preprocessing the data from ASK REAL project.

### Installation

1. Install [Praat](http://www.fon.hum.uva.nl/praat/).

Add the following line in `~/.bash_profile` (see [here](https://github.com/JoFrhwld/FAVE/wiki/Praat-on-OS-X) for further details)

```
export PATH=$PATH:/Applications/Praat.app/Contents/MacOS
```

2. Intall [sfl2txt](https://sourceforge.net/projects/sfltxtconverter/).

Run `make` in the `ASK_preprocess/sfl2txt-current` to install sfl2txt locally.

3. Install [ffmpeg](http://www.ffmpeg.org).

```
brew install ffmpeg
```

4. Install [pydub](http://pydub.com/) locally.

```
git clone https://github.com/jiaaro/pydub.git
```

### File directory convention

All raw data should be posited in `raw/` directory, categorized by their language names as below.

```
|____raw
| |____wav
| | |____07_Bashkir
| | | |____07_Bashkir_01.WAV
| | | |____07_Bashkir_02.WAV
| | | |____07_Bashkir_04.WAV
| | | |____...
| | |____07_Buryat02
| | | |____07_Buryat02_01.WAV
| | | |____07_Buryat02_02.WAV
| | | |____07_Buryat02_03.WAV
| | | |____...
| |____sfl
| | |____07_Bashkir
| | | |____07_Bashkir_01_mark#1#.sfl
| | | |____07_Bashkir_02_mark#1#.sfl
| | | |____07_Bashkir_03_mark#1#.sfl
| | | |____...
| | |____07_Buryat02
| | | |____07_Bashkir_01_mark#1#.sfl
| | | |____07_Bashkir_02_mark#1#.sfl
| | | |____07_Bashkir_03_mark#1#.sfl
| | | |____...
```

