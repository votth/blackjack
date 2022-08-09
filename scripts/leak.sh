#!/bin/bash

$SOURCES='*.cpp'

g++ $SOURCES -o ./out/leak.out && valgrind -s --leak-check=full --track-origins=yes ./out/leak.out
