#!/bin/bash

rm -f *~
rm -f src/*~
rm -f PIEIS
rm -f *.o
rm -f *.inf
rm -f mangle
rm -f *.lst
rm -f *.sym

for i in A G P Q U
do
    for j in TEST MINI SPIGOT
    do
        rm -f ${i}${j}
        rm -f ${i}${j}.s
    done
done
