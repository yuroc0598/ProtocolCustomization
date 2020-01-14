#!/bin/bash
USR=`logname`
AFL_PATH="./"
PROG_PATH="$AFL_PATH/testcases/openssl"
NTHREAD=$2

# usage

function usage
{
echo "Usage of this testing script:"
echo "---sudo ./auto.bash PACKET_SEQ_Number Number_of_Threads"
echo "---currently only support:"
echo "------Number_of_Threads: int from 1 to N, don't be greedy and leave some cores for others"
}

# check arg format

if [ $# -ne 2 ] || [ $1 -lt 1 ] || [ $2 -lt 1]
then
echo "invalid arguments, check usage below:"
usage
exit -1
fi

# check if the output dir is empty or not


function clearDir
{
path=$1
if [ ! -d "$path" ]
then
mkdir -p "$path" -m 777
else
rm -rf $path/*
fi
}


function createDir
{
path=$1
if [ ! -d "$path" ]
then
mkdir -p "$path" -m 777
fi
}

createDir $PROG_PATH/in/p1
createDir $PROG_PATH/in/p2
createDir $PROG_PATH/in/p3
createDir $PROG_PATH/in/p4
createDir $PROG_PATH/in/p5
createDir $PROG_PATH/in/p6
createDir $PROG_PATH/in/p7
createDir $PROG_PATH/in/p8

clearDir  "$PROG_PATH/out/p1"
clearDir  "$PROG_PATH/out/p2"
clearDir  "$PROG_PATH/out/p3"
clearDir  "$PROG_PATH/out/p4"
clearDir  "$PROG_PATH/out/p5"
clearDir  "$PROG_PATH/out/p6"
clearDir  "$PROG_PATH/out/p7"
clearDir  "$PROG_PATH/out/p8"

# always start from p1
for (( i=0;i<$NTHREAD;i++ ))
do
sub_out_dir=fuzzer"$i"
if [ $i -eq 0 ]
then
$AFL_PATH/experimental/asan_cgroups/limit_memory.sh -u $USR $AFL_PATH/afl-fuzz  -i $PROG_PATH/in/  -o $PROG_PATH/out -M $sub_out_dir -m none -t 400 -f $PROG_PATH/input -p $1 $PROG_PATH/openssl 
else
$AFL_PATH/experimental/asan_cgroups/limit_memory.sh -u $USR $AFL_PATH/afl-fuzz  -i $PROG_PATH/in/ -o $PROG_PATH/out -S $sub_out_dir -m none -t 400 -f $PROG_PATH/input  -p $1 $PROG_PATH/openssl > /dev/null 2>&1 &
fi
done
