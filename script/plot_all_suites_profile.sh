#!/bin/bash

python3 script/run.py --suitename=perfect --plot=profiling
python3 script/run.py --suitename=parsec --plot=profiling 
python3 script/run.py --suitename=spec --plot=profiling
python3 script/run.py --suitename=allsuite --plot=profiling


