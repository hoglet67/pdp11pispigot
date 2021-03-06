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
# P = PCC compiler with Unix V7 assembler
# Q = PCC compiler with GCC assembler (broken because of jbr/jcc)
# U = Unix V7 compiler with Unix V7 assembler
#
BUILD_C_TARGETS="A G P U"

add_file_to_ssd () {
    md5sum $1
#   beeb delete ${SSDFILE} -y $1
    echo "$.$1    B000 B000" > $1.inf
    beeb putfile ${SSDFILE} $1
#   rm -f $1.inf
}

# Setup apout to point to a local unix_v7 directoty tree
export APOUT_ROOT=unix_v7
mkdir -p $APOUT_ROOT/tmp

# Swallow warning from apout that mentions a_magic2
run_apout () {
    apout $* |& grep -v a_magic2
}

# Prepare the ssd image
SSDFILE=build.ssd
cp bbcpdp.ssd ${SSDFILE}

# Add in PIBAS
add_file_to_ssd PIBAS

# Prepare the PCC Libraries
PCC_LIBS=$(find lib -name '*.s')

# Compile the mangle program removes 0x100 bytes of the a.out file
gcc src/mangle.c -o mangle

# Package the Unix V7 libraries into an archive
UNIX_V7_LIB=lib/libunix.a
rm -f $UNIX_V7_LIB
for i in $PCC_LIBS
do
    OBJ=${i%.s}.o
    run_apout unix_v7/bin/as -o $OBJ $i
    run_apout unix_v7/bin/ar cr $UNIX_V7_LIB $OBJ
done
echo "$UNIX_V7_LIB includes:"
run_apout unix_v7/bin/ar t $UNIX_V7_LIB

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
                    ./mangle ${name} ${name} -d
                    pdp11-aout-objdump -D $name --adjust-vma=$ADDR > $name.lst
                    pdp11-aout-strip -D $name
                    ;;

                # G = GCC compiler with GCC assembler

                G)
                    pdp11-aout-gcc -nostdlib -Ttext $ADDR src/crt0_gcc.s src/$i -lgcc -o $name
                    pdp11-aout-objdump -D $name --adjust-vma=$ADDR > $name.lst
                    pdp11-aout-strip -D $name
                    ;;

                # Q = PCC compiler with GCC assembler

                Q)
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

                # P = PCC compiler with Unix V7 assembler

                P)
                    pdp11-aout-bsd-pcc -S src/$i
                    asm=$name.s
                    mv ${i%.*}.s ${asm}
                    run_apout unix_v7/bin/cc -o ${name} src/pad.s src/crt0_pcc.s ${asm} $UNIX_V7_LIB
                    ./mangle ${name} ${name} -d
                    pdp11-aout-objdump -D $name --adjust-vma=$ADDR > $name.lst
                    pdp11-aout-strip -D $name
                    ;;


                # U = Unix V7 compiler with Unix V7 assembler
                # TODO: this could use pad.s / crt0_pcc.s
                U)
                    run_apout unix_v7/bin/cc -o ${name} src/crt0_v7.s  src/v7/$i
                    ./mangle ${name} ${name} -d
                    pdp11-aout-objdump -D $name --adjust-vma=$ADDR > $name.lst
                    pdp11-aout-strip -D $name
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
