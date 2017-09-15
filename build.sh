#!/bin/bash

gcc -O3 -c hex.c
gcc -O3 -c manchester.c
gcc -O3 -c protocol.c
gcc -O3 -c filter.c
gcc -O3 -o demod demod.c hex.o protocol.o manchester.o filter.o -lpthread -lrtlsdr
gcc -O3 -o rollthecode rollthecode.c hex.o protocol.o manchester.o

