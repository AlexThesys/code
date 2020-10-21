#!/bin/bash

for i in $1/*.*; do
    file=${i##*/}
    if [[ -f $i ]] && [[ -e $2/${file} ]]; then
        echo "//------------------------------"
        echo $file
        diff $1 $2/${file}
        if [[ $1 -ef $2/${file} ]]; then
            echo "Same files!"
        fi
    fi
done
