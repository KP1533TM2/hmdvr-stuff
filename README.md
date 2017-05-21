# hmdvr-stuff
Bits and pieces of HMDVR "DVR for drones"

Just my little "spare time in the eveining one day" effort to figure out as much things as I can
about the thing that is a HMDVR, well, a Chinese DVR that's being sold in probably many places
such as banggood, aliexpress and ebay.
Since later revisions (probably because of video decoder IC change) it stopped recording
noisy video properly and is now partially, well, unfit for purspose of recording "drone" 
(or, infact any other R/C model) video, since being analog, it's almost never noiseless.
It does record that video, but as the signal starts being weakened and polluted with noise, the
video turns into a black screen epillepsy-fest. That's basically why I'm doing all of this :P

One could attach the box to the model itself and get great video, but one could also attach
a proper action camera to get much better recording quality. This HMDVR box thingy, while attached
to goggles or ground station, could give one an advantage of always having a copy of FPV video on
the ground, as long as the video downlink works, which in turn might help one to locate his model
in case of, say, crash.
It's also a much neater (although, a little less video quality-wise) alternative to fiddling with
a laptop, VRx and an USB capture card in the field.

# What's been done so far
* pics/layers.cxf - HMDVR V1.3 board scan corrected and aligned for track tracing;
* some bits of Chinese GM7150 datasheet translated here and there, just a little;
* KiCAD project started, TVP5150 schematic component made;
* Arduino and a logic analyzer hooked up to I2C bus between main processor and GM7150 in order 
to sniff transactions and inject my own - works easily due to the nature of I2C and the fact that
the main processor only does a few transactions on power-up;
* Main processor initialization transactions were sniffed and deciphered according to TVP5150AM1
datasheet;
* Arduino 'sketch' (grrr) that implements I2C host controllable over serial connection with some
video decoder chip specifics; Allows to poke around the video decoder IC registers from PC. Both
read and write are available. Works on Mega 2560, could be adapted to smaller Arduinos mainly by
limiting buffer sizes. Just attach its TWI/I2C bus in parallel to the HMDVR one and it should work;
* Python script that implements video decoder IC class that allows to probe, read and write
registers to the chip using the aforementioned Arduino sketch;
* Some poking around and experimening with registers and other stuff here and there has already shown:
  * There's probably a mistake on the PCB, which, surprisingly however, doesn't affect GM7150's 
 operation (I thought it would, though). /PDN signal of GM7150 is attached through 0-ohm SMD
 'resistor' to ground, which, would that be a TVP5150, would send the chip into permanent low-power
 (non-operational?) mode;
  * GM7150 consciously detects weak signal (status reg 0x89, bit 6), but always blanks it;
  * Page 20 of GM7150 datasheet probably has a typo - regs 0x18, 0x19 are described instead
 0x13, 0x14. Not sure what to think of that;
  * Ref 0x7f is "Patch execute" on TVP5150, but 'reserved' on GM7150. This is the first register main
 processor writes to on power-up, and I have no clue what to make of it;
  * What the heck are patches, where do I find one, and how I can push one in the video decoder?
 There's no TI patch loading app note in sight, and I suspect GM7150 doesn't have that feature at
 all;
  * GM7150 doesn't have SECAM colour system features, no big loss for FPV, I guess;
  * F/V bit controls in reg 0x15 of TVP5150 are 'reserved' on GM7150;
  * On startup, main processor, along a few others, makes a write to reg 0x17, which is reserved and
 undocumented on both chips;
  * GM7150 datasheet's datecode is 2014.4. Seems a bit old, but couldn't find any newer ones.
  * Just found those things somehow, those might give a clue about GM7150 initialization:
    * https://github.com/OpenNuvoton/NuMaker_NuWicam_RDK/blob/master/sources/kernel/drivers/media/video/w55fa93_hw/GM7150/GM7150.dat
    * https://github.com/OpenNuvoton/NuMaker_NuWicam_RDK/blob/master/sources/kernel/drivers/media/video/w55fa93_hw/GM7150/sensor_gm7150.h
   
# What there is to be done
Quite frankly, I don't have a clue. Blindly poking around isn't fun, so I'll leave it all at that.
My own conclusion as of now: GM7150 video decoder can't be set up to digitize noisy video properly,
while TVP5150 has this working right out of the box. Probably TV/VCR switch/detector is at fault? 
Or maybe GM7150 isn't just (almost) direct clone, but a completely different beast inside?
I don't know, this requires further investigation. However, is this investigation also worth
anyone's time, I'm unsure.

![Alt text](/pics/ridiculousness.jpg?raw=true "derp")

If anyone feels like contributing to this, feel free to do so.
