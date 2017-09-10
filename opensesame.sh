#!/bin/bash

if [ "$1" == "" ]; then
        echo Need a command as argument: lock, unlock or trunk
        exit 1
fi

## Roll to the next code and force a command
./rollthecode $1

## Read the next code
CODE=$(cat latestcode.txt)

## Turn the code into a string of ones and zeros
PKT=$(./code2ookbitstring "$CODE")

./fx2ooksend "$PKT" | fx2lpddsfm -f 433.92M -d 433.92M -v -t float

