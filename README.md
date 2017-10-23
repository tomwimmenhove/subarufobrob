# subarufobrob
Hijack a subaru's key fob and steal all the things

# *UPDATE*
I am hearing claims from multiple dealers/spokes persons (UK, Australia and BeNeLux) that this only affects US models. I have no way of confirming this, but if true, people outside the US are unlikely to be affected. Please share any information about this if you have a way to test and confirm this. Also, if anybody has a spare non-US keyfob for any of the listed models that can be made available to me for testing, please let me know!

# Description of the vulnerability
The rolling code used by the key fob and car is predictable in the sense that it is not random. It is simply incremental.

# Impact
An attacker can 'clone' the key fob, unlock cars and, when increasing the rolling code with a sufficiently high value, effectively render the user's key fob unusable.

# Affected vehicles
The exploit has only been tested on a 2009 Subaru Forester but the same fob is used, and the exploit should work on, the following vehicles:
 - 2006 Subaru Baja
 - 2005 - 2010 Subaru Forester
 - 2004 - 2011 Subaru Impreza
 - 2005 - 2010 Subaru Legacy
 - 2005 - 2010 Subaru Outback

# Solution
Don't use the most predictable sequential type of rolling code. Don't send the command twice so that, in case of Samy Kamkar's rolljam attack, not even the XOR checksum has to be recalculated when changing a lock to an unlock command, since the 2 commands cancel each other out, leaving the checksum intact.

# Required hardware
In order to run the exploit, a receiver and transmitter, capable of receiving and transmitting on the 433MHz ISM band, are necessary. In our case, we're using an RTL-SDR RTL2832U DBV-T tuner USB dongle as a receiver and a Raspberry Pi B+ v1.2 with **rpitx** as a transmitter. The Raspberry Pi is also used as the host computer for the exploit to run on. Furthermore, a USB WiFi dongle is used in conjunction with **hostapd** in order to be able to remotely connect (ssh) to the Raspberry Pi for control. Alternatively, a Raspberry Pi Zero W could probably be used in order to negate the need for the additional WiFi dongle and further reduce the physical footprint and cost of the device, although this has not been tested.
The RTL-SDR dongle should be fitted with a suitable 433MHz antenna to allow for acceptable reception of the key fob's signal. On the Tx side, a quarter-wavelength (173mm or 6.8") wire can be soldered directly to GPIO 18 (Pin 12 of the GPIO header P1) on the Raspberry Pi. Finally some type of portable power source is required for portable operation, I.E a li-ion power bank.
Total cost of the hardware, when using the Raspberry Pi Zero W, should under $25 (not including a power bank).

# Building
 - Download, compile and install rpitx: https://github.com/F5OEO/rpitx
 - Make sure librtlsdr-dev is installed
 - Build:<br>
   $ **git clone https://github.com/tomwimmenhove/subarufobrob.git**<br>
   $ **cd subarufobrob**<br>
   $ **mkdir build**<br>
   $ **cd build**<br>
   $ **cmake ..**<br>
   $ **make**<br>
   
# Operation
To start capturing packets sent from a key fob, start the 'fobrob' application<br>
   $ **./fobrob**<br>
Run ./fobrob -h for help op options
When a packet is captured, the rolling code will be automatically written to the file "latestcode.txt", as well as appended to "receivedcodes.txt". Now that we have received a valid packet, we can 'roll' the code over to the next one and use that to issue any of the following commands to the car: "lock", "unlock", "trunk" or "panic". In order to create a new rolling code for a particular command, use:<br>
   $ **./rollthecode &lt;command&gt; [increment]**<br>
where &lt;command&gt; is one of the four commands mentioned above and [increment] is an optional integer the rolling code should be incremented with. The new code is, again, written to "latestcode.txt". Now the code can be converted to a format that **rpitx** understands:<br>
   $ **./rpitxify &lt;CODE&gt; &lt;filename&gt;**<br>
where &lt;CODE&gt; is the code contained in the "latestcode.txt" file and &lt;filename&gt; is the rpitx-compatible file to write to. At this point, the code can be transmitted using<br>
   $ **sudo rpitx -m RFA -i &lt;filename&gt; -f 433920**<br>
where v is the previously mentioned rpitx-compatible file.
For convenience, a shell script is added to automate everything but the initial packet capture. To use this, run<br>
   $ **./opensesame.sh &lt;command&gt;**<br>
which will roll the code over by one, convert to rpitx and transmit respectively.

# DISCLAIMER
**Don't actually steal all the things!**

P.S. Thanks to pmsac at toxyn dot org for figuring out the checksum algorithm!
