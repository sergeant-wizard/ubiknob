# Ubiknob

Generic rotary encoders powered by Teensy for XPlane to control anything with "Knobs", such as...

* NAV/COM frequency
* Autopilot desired altitude, heading, course

![example_image](https://github.com/sergeant-wizard/ubiknob/blob/master/docs/example.jpg)

## Concepts

* Small space. In an ideal world with infinite desk space it's much more realistic to place tens of knobs and buttons where they'd be in an actual cockpit, but many of us don't have that space.
* Plane agnostic. There are products out there that replicate specific components but they might seem out of place in a different setting, e.g., GA vs Airliners.
* Delegate toggle switches, push switches, indicators to touchscreens. Tools like AirManager makes this easy. Touchscreens are incredibly robust and I don't have a problem with simulating a toggle switch on a touchscreen. Knobs, however are the missing piece --- it's impractical to operate a dual rotary encoder on a touchscreen.
