#!/bin/bash

# E.g.: ./build.sh -d --unsafe

cc="g++"
options="-I. -m32 -fPIC -Wall -fvisibility=hidden"

separator="-------------------------"
list_item=" - "
wait_indicator="..."

while [[ $# -gt 0 ]]; do
    arg="$1"
    case $arg in
        -d | --debug)
            debug="-g -ggdb -O0" # debug build without optimization
            ;;
        *)
            unrecognized_arg=$arg
            break
            ;;
    esac
    shift
done

if [ -v unrecognized_arg ]; then
    echo "Unrecognized argument \"$unrecognized_arg\", aborting."
    exit 1
fi

echo $separator
echo "Options:"

echo -n "$list_item"
echo -n "Debug:  "
if [ -v debug ]; then
    echo "ON"
else
    echo "OFF"
    debug=""
fi

echo $separator
echo "Compiling:"

mkdir -p ../bin
mkdir -p objects

echo -n "$list_item"
echo -n "cracking.cpp"
echo $wait_indicator
$cc $debug $options $constants -c cracking.cpp -o objects/cracking.opp

echo -n "$list_item"
echo -n "libcod.cpp"
echo $wait_indicator
$cc $debug $options $constants -c libcod.cpp -o objects/libcod.opp

echo -n "$list_item"
echo -n "gsc.cpp"
echo $wait_indicator
$cc $debug $options $constants -c gsc.cpp -o objects/gsc.opp

echo -n "$list_item"
echo -n "gsc_player.cpp"
echo $wait_indicator
$cc $debug $options $constants -c gsc_player.cpp -o objects/gsc_player.opp

echo -n "$list_item"
echo -n "gsc_bots.cpp"
echo $wait_indicator
$cc $debug $options $constants -c gsc_bots.cpp -o objects/gsc_bots.opp

echo -n "Linking libcod1.so"
echo $wait_indicator
objects="$(ls objects/*.opp)"
$cc -m32 -shared -L/lib32 -o ../bin/libcod1.so -ldl $objects

echo $separator

rm objects -r
