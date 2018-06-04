# data
Preliminary data for Glossm. Please import `csv` files in `ASK_for_glossm/` by following order.

* TopicSet
* Topic
* Meaning
* Language

## Pre-processing

`ASK_preprocess/` contains codes for preprocessing the data from ASK REAL project.

### Installation

You need to install [Praat](http://www.fon.hum.uva.nl/praat/) and [sfl2txt](https://sourceforge.net/projects/sfltxtconverter/) before running.

Please add the following line in `~/.bash_profile` or `~/.bashrc`  (see [here](https://github.com/JoFrhwld/FAVE/wiki/Praat-on-OS-X) for further details)

```
export PATH=$PATH:/Applications/Praat.app/Contents/MacOS
```

Run `make` in the `ASK_preprocess/sfl2txt-current` to install sfl2txt locally.

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

