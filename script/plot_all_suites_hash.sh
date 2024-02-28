#!/bin/bash

python3 script/run.py --suitename=perfect --plot=hash
python3 script/run.py --suitename=parsec --plot=hash 
python3 script/run.py --suitename=spec --plot=hash
python3 script/run.py --suitename=allsuite --plot=hash


