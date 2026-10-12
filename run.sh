#!/bin/bash

for f in "$@"; do
    name=$(basename "$f" .json)
    ./build/vsystem "$f" > "results/$name.txt" 2>&1 || result=1
done