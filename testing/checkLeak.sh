#!/bin/bash
g++ blackjack.cpp players.cpp main.cpp -o a.out && valgrind -s --leak-check=full --track-origins=yes ./a.out
