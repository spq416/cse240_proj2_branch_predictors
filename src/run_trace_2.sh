#!/bin/bash
FILES=../../../traces_2/*

for f in $FILES
do  
    echo "Processing $f file..."
    bunzip2 -c -k $f | ./predictor
done

