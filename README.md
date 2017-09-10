# subarufobrob
Hijack a subaru's key fob and steal all the things

Usage:
- Build it:
  $ ./build.sh
- Start the demodulator:
  $ nc -l -p 4242 | ./demod
- Start listener.grc in GNURadio, or start listener.py directly to start capturing data and passing it on the the demodulator above
- Wait until a packet is captured. This will automatically be written to "latestcode.txt" and appended to "receivedcodes.txt"
- Stop the demodulator and gnuradio script.

You now have the latest packet from the FOB in "latestcode.txt" !

To roll over to the next code, run ./rollthecode <command>, where command is either "lock", "unlock" or "trunk".

Currently the only way to send the new code back to the car is using a custom transmitter with a DDS chip that can be found here: https://circuitmaker.com/Projects/Details/Tom-Wimmenhove/USB-DDS
which can be controlled with the code here: https://github.com/tomwimmenhove/fx2lpdds
This transmitter doesn't really support the 433MHz band, but it's nyquist images are strong enough for an acceptable transmit range. To use this, simply use ./opensesame.sh <command>

I might work on this a little more to make transmitting easier with, for example, rpitx on the raspberry pi. This way, no custom or expensive hardware will be needed.

It has been tested with 2 keyfobs for a 2009 Subaru Forester.

This fob is also used on:
 - 2006 Subaru Baja
 - 2005 - 2010 Subaru Forester
 - 2004 - 2011 Subaru Impreza
 - 2005 - 2010 Subaru Legacy
 - 2005 - 2010 Subaru Outback

So it should work with all the above.

