#!/bin/bash

ssdnum=488

ssdfile=build.ssd

cp bbcpdp.ssd ${ssdfile}

build_gcc_c=1
build_gcc_s=0
build_pcc=0
build_v7=0

add_file_to_ssd () {
    md5sum $1
#   beeb delete ${ssdfile} -y $1
    echo "$.$1    B000 B000" > $1.inf
    beeb putfile ${ssdfile} $1
#   rm -f $1.inf
}

if [ $build_gcc_c == "1" ]
then
   for i in test.c mini.c spigot.c
   do
       addr=0x100
       name=`echo ${i%.*} | tr "a-z" "A-Z"`
       pdp11-aout-gcc -nostdlib -Ttext $addr src/crt0.s src/$i -lgcc -o $name
       pdp11-aout-objdump -D $name --adjust-vma=$addr > $name.lst
       pdp11-aout-strip -D $name
       add_file_to_ssd $name
   done
fi

if [ $build_gcc_s == "1" ]
then
    for i in pieis.s
    do
        name=`echo ${i%.*} | tr "a-z" "A-Z"`
        pdp11-aout-gcc -T src/$name.ld -nostdlib src/$i -o $name
        pdp11-aout-objdump -D $name > $name.lst
        pdp11-aout-strip -D $name
        add_file_to_ssd $name
    done
fi


# PCC experiments
if [ $build_pcc == "1" ]
then
    for i in spigot.c mini.c
    do
        name=`echo P${i%.*} | tr "a-z" "A-Z"`

        # Use PCC as a compiler only
        pdp11-aout-bsd-pcc -S src/$i
        mv ${i%.*}.s $name.s

        asm=$name.s
        sed -i "s/jgt/bgt/" ${asm}
        sed -i "s/jlt/blt/" ${asm}
        sed -i "s/jle/ble/" ${asm}
        sed -i "s/jge/bge/" ${asm}
        sed -i "s/jeq/beq/" ${asm}
        sed -i "s/jne/bne/" ${asm}
        sed -i "s/jlos/blos/" ${asm}
        sed -i "s/jbr/br/"  ${asm}

        # Use GCC as an assembler/linker
        pdp11-aout-gcc -nostdlib -Ttext 0x100  src/crt0.s ${asm} lib/*.s -o ${name}
        pdp11-aout-objdump -D $name --adjust-vma=$addr > $name.lst
        #pdp11-aout-strip -D $name
        add_file_to_ssd $name
    done
fi


# Unix V7 experiments
#
if [ $build_v7 == "1" ]
then
    add_file_to_ssd unix_v7/PIV7
    add_file_to_ssd unix_v7/PIV7X
fi

beeb title ${ssdfile} "PDP-11 Pi"
beeb info ${ssdfile}

if [ -f $BBC_FILE ];
then
    beeb dkill -y ${ssdnum}
    beeb dput_ssd ${ssdnum} ${ssdfile}
fi
