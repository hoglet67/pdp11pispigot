#!/bin/bash

rm -f result.*
max=200
echo "Exploring Pi between 1 and $max digits"
for extra in `seq -f '%02g' 0 20`
do
    echo $extra
    for digits in `seq 1 $max`
    do
        ./spigot $digits 1 $extra >> result.$extra
    done
done
md5sum result.*
