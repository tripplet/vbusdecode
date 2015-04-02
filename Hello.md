# Introduction #

This is a utility for use with resol solar controllers (or rebranded alternatives)


# Details #

Some Resol controllers have an interface called VBUS.  This interface is an RS485 serial interface.  These controllers transmit much of their state via this interface.

Resol have windows software that will log this, but I found a need to have something simpler and able to run on my WGT634U linux box.

So here is my first attempt, and instead of building in the specifics for the models, I have designed it so the parameters are passed via arguments.  Right now, there is no error checking around args, so you need to get them right.  Below is a table of how to use this for the models I can get info on;

For example;

Resol Deltasol BS Plus
> cat /dev/ttyUSB0 | vbusdecode 0,15,0.1 2,15,0.1 4,15,0.1 6,15,0.1 8,7,1 9,7,1
> Will give out, temp of S1,S2,S3,S4 and pump1 speed and pump2

Included is a sample of a serial port capture , raw.log, from a Resol Deltasol BS Plus ,so can you test with;
> cat raw.log | vbusdecode 0,15,0.1 2,15,0.1 4,15,0.1 6,15,0.1 8,7,1 9,7,1