##
##  Copyright 2017 Tom Wimmenhove<tom.wimmenhove@gmail.com>
##
##  This file is part of Subarufobrob
##
##  Subarufobrob is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  Foobar is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
##

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

