# Irrigation Server
## What does it do?
Opens a socket listens for some simple strings.  It then may fiddle some GPIO pins in an attempt to open and close irrigation valves.   This is a simple socket -> gpio wrapper which also adds safty.  More interesting control can then be written in a faster (to develop) looser language.

If it crashes it attempts to clean up the value state to minimise massive flooding. (I should go check if that is working...).

I expect to use it with my python based IrrigationController.  This will be the more fun part.

## What should it do that it doesn't?
Yeah there are some basics,
* It should load the valve configuration from a configuration file.
* It should have a maximum on time over a 24 hour period

Maybe an advanced features, but this could be done at the higher level:
* It should allow pulsed control (n minutes on, m minutes off for k total on time)

In a perfect world I'd make a test version which would do all the things but control the values and run on other hardware platforms so development of the higher level things wouln'd need to water the garden during testing.

## What do I need?
Physical things:
* Raspberry PI
* Some relay board which works with the GPIO pins
* Some irrigation things
Installed on the Raspberry PI
* [Wireing Pi](http://wiringpi.com/)
* g++
* make tools
* git

## How do I contribute?
Your kidding right?
