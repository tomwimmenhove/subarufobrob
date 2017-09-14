#!/bin/bash

if [ "$1" == "" ]; then
        echo Need a command as argument: lock, unlock or trunk
        exit 1
fi

## Roll to the next code and force a command
./rollthecode $1

## Read the next code
CODE=$(cat latestcode.txt)

# Create the right format for rpitx
./rpitxify $CODE rpitx.rfa

# Send it
sudo rpitx -m RFA -i rpitx.rfa -f 433920

