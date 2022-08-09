#!/bin/bash

$SOURCES='blackjack.cpp player.cpp main.cpp'

g++ "$SOURCES" -g -o ./out/debug.out && gdb ./out/debug.out
