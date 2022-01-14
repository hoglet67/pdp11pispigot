#!/bin/bash

export PATH=~/pdp11/local/bin:$PATH
export LD_LIBRARY_PATH=~/pdp11/local/lib:$LD_LIBRARY_PATH

ssdnum=488

ssdfile=build.ssd

cp bbcpdp.ssd ${ssdfile}

add_file_to_ssd () {
    md5sum $name
#   beeb delete ${ssdfile} -y $1
    echo "$.$name    B000 B000" > $1.inf
    beeb putfile ${ssdfile} $1
    rm -f $1.inf
}


for i in spigot.c test.c
do
    name=`echo ${i%.*} | tr "a-z" "A-Z"`
    pdp11-aout-gcc -nostdlib src/$i src/ldiv.s src/lrem.s src/csv.s -o $name
    pdp11-aout-objdump -D $name > $name.lst
    pdp11-aout-strip -D $name
    add_file_to_ssd $name
done

for i in pieis.s
do
    name=`echo ${i%.*} | tr "a-z" "A-Z"`
    pdp11-aout-gcc -T src/$name.ld -nostdlib src/$i -o $name
    pdp11-aout-objdump -D $name > $name.lst
    pdp11-aout-strip -D $name
    add_file_to_ssd $name
done

beeb title ${ssdfile} "PDP-11 Pi"
beeb info ${ssdfile}

if [ -f $BBC_FILE ];
then
    beeb dkill -y ${ssdnum}
    beeb dput_ssd ${ssdnum} ${ssdfile}
fi
