#!/bin/bash

for digits in `seq 1 10000`
do
    ./spigot $digits 1 $1 | fold -1 > pi
    pi $digits | tr -d "." | fold -1 > ref
    diff pi ref > /dev/null
    if [[ "$?" != "0" ]]; then
        echo $digits':*'
    fi
done
