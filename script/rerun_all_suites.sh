#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <launch>"
    exit 1
fi

python3 script/run.py --suitename=perfect --launch $1
python3 script/run.py --suitename=parsec --launch $1
python3 script/run.py --suitename=spec --launch $1
