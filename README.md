# subarufobrob
Hijack a subaru's key fob and steal all the things

It uses an RTL-SDR dongle and a Raspberry-Pi to capture and send keyfob codes. The RTL-SDR dongle is used to capture and demodulate a code. This code can then be changed into an 'unlock' command with a new rolling code. This new code can then be sent using rpitx (a separate unrelated Raspberry-Pi tool), to unlock YOUR car.

Usage:
- Build it:
  $ ./build.sh
- Start the demodulator:
  $ ./demod [options]
- Wait until a packet is captured. This will automatically be written to "latestcode.txt" and appended to "receivedcodes.txt"

You now have the latest packet from the FOB in "latestcode.txt" !

To roll over to the next code, run ./rollthecode &lt;command&gt;, where command is either "lock", "unlock", "trunk" or "panic".
To create a file for rpitx to transmit the code, use ./rpitxify &lt;hexcode&gt; someFile.rfa
Then, this code can be sent using sudo rpitx -m RFA -i someFile.rfa -f 433920

There is a simple little shell script, opensesame.sh, that does the above three steps automatically. Run ./opensesame.sh &lt;command&gt;

It has been tested with 2 keyfobs for a 2009 Subaru Forester.

This fob is also used on:
 - 2006 Subaru Baja
 - 2005 - 2010 Subaru Forester
 - 2004 - 2011 Subaru Impreza
 - 2005 - 2010 Subaru Legacy
 - 2005 - 2010 Subaru Outback

So it should work with all the above.


DISCLAIMER: Don't actually steal the thingz!

