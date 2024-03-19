#!/bin/bash

echo "running parsec"
python3 script/run.py --suitename=parsec --launch vanila_bdi
wait
echo "running spec"
python3 script/run.py --suitename=spec --launch vanila_bdi
wait
echo "done"