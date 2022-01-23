#!/bin/bash

# SSD disk number to copy to in MMB file (if present)
#
SSDNUM=488

# Load address
#
ADDR=0x100

# List of seperate programs to build
#
PROGS="test.c mini.c spigot.c"

# Enable/disable assembler build
#
BUILD_S=1

# Enable/Disable C build
#
BUILD_C=1

# Control specific C builds
#
# A = Ack compiler with Ack assembler
# G = GCC compiler with GCC assembler
# P = PCC compiler with GCC assembler
# Q = PCC compiler with Unix V7 assembler
# U = Unix V7 compiler with Unix V7 assembler
#
BUILD_C_TARGETS="A G P Q U"

add_file_to_ssd () {
    md5sum $1
#   beeb delete ${SSDFILE} -y $1
    echo "$.$1    B000 B000" > $1.inf
    beeb putfile ${SSDFILE} $1
#   rm -f $1.inf
}

# Prepare the ssd image
SSDFILE=build.ssd
cp bbcpdp.ssd ${SSDFILE}

# Prepare the PCC Libraries
# TODO: Build a library .a file
PCC_LIBS=$(find lib -name '*.s')

# Compile the mangle program removes 0x100 bytes of the a.out file
gcc src/mangle.c -o mangle

# Setup apout to point to a local unix_v7 directoty tree
export APOUT_ROOT=unix_v7
mkdir -p $APOUT_ROOT/tmp

if [ $BUILD_S == "1" ]
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

if [ $BUILD_C == "1" ]
then

    for b in $BUILD_C_TARGETS
    do

        for i in $PROGS
        do

            name=`echo ${b}${i%.*} | tr "a-z" "A-Z"`

            case $b in

                # A = Ack compiler with Ack assembler

                A)
                    ack -w -mpdpv7 src/crt0_ack.s src/$i -o $name
                    ./mangle ${name} ${name}
                    pdp11-aout-objdump -D $name --adjust-vma=$ADDR > $name.lst
                    ;;

                # G = GCC compiler with GCC assembler

                G)
                    pdp11-aout-gcc -nostdlib -Ttext $ADDR src/crt0_gcc.s src/$i -lgcc -o $name
                    pdp11-aout-objdump -D $name --adjust-vma=$ADDR > $name.lst
                    pdp11-aout-strip -D $name
                    ;;

                # P = PCC compiler with GCC assembler

                P)
                    # Use PCC as a compiler only
                    pdp11-aout-bsd-pcc -S src/$i
                    asm=$name.s
                    mv ${i%.*}.s ${asm}
                    # Hacky fixups...
                    sed -i "s/jgt/bgt/"   ${asm}
                    sed -i "s/jlt/blt/"   ${asm}
                    sed -i "s/jle/ble/"   ${asm}
                    sed -i "s/jge/bge/"   ${asm}
                    sed -i "s/jeq/beq/"   ${asm}
                    sed -i "s/jhi/bhi/"   ${asm}
                    sed -i "s/jlo/blo/"   ${asm}
                    sed -i "s/jne/bne/"   ${asm}
                    sed -i "s/jlos/blos/" ${asm}
                    sed -i "s/jhos/bhos/" ${asm}
                    sed -i "s/jbr/br/"    ${asm}
                    # Use GCC as an assembler/linker
                    pdp11-aout-gcc -nostdlib -Ttext $ADDR -o ${name} src/crt0_pcc.s $PCC_LIBS ${asm}
                    pdp11-aout-objdump -D $name --adjust-vma=$ADDR > $name.lst
                    pdp11-aout-strip -D $name
                    ;;

                # Q = PCC compiler with Unix V7 assembler

                Q)
                    pdp11-aout-bsd-pcc -S src/$i
                    asm=$name.s
                    mv ${i%.*}.s ${asm}
                    apout unix_v7/bin/as -o ${name} src/pad.s src/crt0_pcc.s $PCC_LIBS ${asm}
                    apout unix_v7/bin/strip ${name}
                    ./mangle ${name} ${name}
                    ;;

                # U = Unix V7 compiler with Unix V7 assembler

                U)
                    ;;

            esac

            # Add the file into the SSD image
            add_file_to_ssd $name
        done
    done
fi

# Final Packaging
beeb title ${SSDFILE} "PDP-11 Pi"
beeb info ${SSDFILE}

# Copy to the SD Card if it's present
if [ -f $BBC_FILE ];
then
    beeb dkill -y ${SSDNUM}
    beeb dput_ssd ${SSDNUM} ${SSDFILE}
fi
