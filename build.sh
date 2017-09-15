#!/bin/bash

gcc -O2 -c hex.c
gcc -O2 -c manchester.c
gcc -O2 -c protocol.c
gcc -O2 -c filter.c
gcc -O2 -c runningavg.c
gcc -O2 -o demod demod.c runningavg.o hex.o protocol.o manchester.o filter.o -lpthread -lrtlsdr
gcc -O2 -o rollthecode rollthecode.c hex.o protocol.o manchester.o

