#!/bin/bash

gcc -O2 -c hex.c
gcc -O2 -c manchester.c
gcc -O2 -c protocol.c
gcc -O2 -o code2ookbitstring code2ookbitstring.c hex.o protocol.o manchester.o
gcc -O2 -o demod demod.c hex.o protocol.o manchester.o
gcc -O2 -o fx2ooksend fx2ooksend.c hex.o protocol.o manchester.o
gcc -O2 -o rollthecode rollthecode.c hex.o protocol.o manchester.o

