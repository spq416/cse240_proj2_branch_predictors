#!/bin/bash
FILES=../traces/*

for f in $FILES
do  
    echo "Processing $f file..."
    bunzip2 -c -k $f | ./predictor
done

