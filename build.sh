#!/bin/bash

ssdnum=488

ssdfile=build.ssd

cp bbcpdp.ssd ${ssdfile}


add_file_to_ssd () {
    md5sum $1
#   beeb delete ${ssdfile} -y $1
    echo "$.$1    B000 B000" > $1.inf
    beeb putfile ${ssdfile} $1
#   rm -f $1.inf
}

for i in spigot.c test.c
do
    addr=0x100
    name=`echo ${i%.*} | tr "a-z" "A-Z"`
    pdp11-aout-gcc -nostdlib -Ttext $addr src/crt0.s src/$i -lgcc -o $name
    pdp11-aout-objdump -D $name --adjust-vma=$addr > $name.lst
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

# PCC experiments
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


# Unix V7 experiments
#
#add_file_to_ssd unix_v7/PIV7
#add_file_to_ssd unix_v7/PIV7X

beeb title ${ssdfile} "PDP-11 Pi"
beeb info ${ssdfile}

if [ -f $BBC_FILE ];
then
    beeb dkill -y ${ssdnum}
    beeb dput_ssd ${ssdnum} ${ssdfile}
fi
