#!/bin/bash

FILES2=../../traces_large/*
for d in $FILES2
do
    echo " $d"
    sub_dir="${d}/*"
    for f1 in $sub_dir
    do
	echo "Processing $f1 file ... "
	bunzip2 -c -k $f1 | ./predictor
    done
done

