# Physical Mute Button for Video Chat

Build a physical mute button (for your microphone and camera) for video chat
applications like Zoom, Google Meet, Microsoft Teams and WebEx using an
[Adafruit Trinket M0](https://www.adafruit.com/product/3500).

## Acknowledgements

Thanks to Elliotmade for the inspiration on this project,
[Physical Mute Button for Zoom Meetings](https://elliotmade.com/2020/04/23/physical-mute-button-for-zoom-meetings/).

Refactored by [HaxNobody](https://gist.github.com/HaxNobody/7bde369d7a41348b8b91c1a4f358ea4a/1c4d891c09977bd7866d5bf634bce52e46ae9e10)
to extend functionality and add super helpful comments.

Additionally modified and extended by [Jim Stump](https://github.com/jimstump/trinket-m0-physical-mute-button-for-video-chat)
to add MacOS support as well as additional video chat programs.  Also moved to
the [Adafruit Trinket M0](https://www.adafruit.com/product/3500) board as
features outgrew the Digispark ATtiny85's capabilities.

## How Does This Work

This program will send USB HID keyboard presses to activate microphone and
video functions using the application's default keyboard shortcuts.
 * A momentary press on button 1 will toggle microphone mute on or off.
 * Press and hold button 1 to activate PTT (Push-to-Talk) functionality for
   supported applications (Zoom-only).  All other applications will perform the
   mute toggle action.
 * A momentary press on button 2 will toggle video on or off for supported
   applications (not WebEx).

The following video chat programs are supported for selection:
 * Zoom
 * Google Meet
 * Microsoft Teams
 * Cisco WebEx

The video chat program can be chosen using a potentiometer dial attached to
the `PIN_ANALOG_IN` pin.  On startup and when the buttons are pressed, the
analog value from the potentiometer is read to determine which video chat
application and platform keyboard shortcuts should be used.  Once determined,
the color of the onboard DotStar pixel on the Trinket M0 will be changed to
indicate the current selection (Zoom is blue, Google Meet is Lime, Microsoft
Teams is dark violet, and WebEx is gold).  If you don't plan on using a
potentiometer for this, then comment out the `PIN_ANALOG_IN` definition at the
top of the program and the PC keyboard shortcuts for Zoom will be used (this
can be configured at the bottom of the `setApplicationSettings` function -
I didn't get a chance to make that more easily configurable yet).

## MacOS Keyboard Setup Assistant

![MacOS Keyboard Setup Assistant Window](images/macos-keyboard-setup-assistant.png)

As mentioned in the "How Does This Work" section, this mute button
device/program identifies itself as a keyboard to the connecting computer so
that it can send the appropriate keyboard shortcuts when the buttons are
pressed.  In MacOS, the first time a non-Apple keyboard is connected the
Keyboard Setup Assistant window displays.  This application asks you to press
certain keys on the keyboard in an attempt to identify and configure the
correct keyboard layout.

While a function (`runMacKeyboardSetup`) exists to press the correct keys to
help identify the keyboard as an "ANSI (United States and others)" standard
101-key or 102-key keyboard, it is not currently called.  I originally had it
configured on a double click of button 1 but took it out because I didn't like
the delay that was then required to correctly detect a single click of the
button for mute toggling.

So, how do you get past the Keyboard Setup Assistant?  Click the "Continue"
button on the Introduction screen of the wizard until it displays the
"Identifying Your Keyboard" screen where it asks you to press a specific key on
the keyboard.  At that point, press one of the mute buttons a couple times
(in my testing 3 times was required) until the wizard shows a popup saying that
the keyboard couldn't be identified.  Click the "OK" button to dismiss the
popup and the wizard will then ask you to select the correct keyboard type.
Choose the "ANSI (United States and others)" option and then click the "Done"
button to exit the wizard.  At this point, the device should be correctly
configured and you should not be prompted to go through the setup again on that
computer (at least for the current user).

## Current Hardware

While there are plenty of parts that can be used to build this project, I
thought it might be helpful to list out what I'm currently using in my
prototype.

* [Adafruit Trinket M0](https://www.adafruit.com/product/3500)
* [24mm Arcade Button with Built-in LED](https://www.amazon.com/dp/B06Y29LBJ4)
  (I used 2)
* [10K Linear Potentiometer](https://www.adafruit.com/product/562)
* [Potentiometer Knob](https://www.adafruit.com/product/2047)
* [Arcade Button Quick-Connect Wires](https://www.adafruit.com/product/1152)
  (I used 4 pairs)
* Female/Female (F/F) jumper cables (I used 3 cables)
* [Panel Mount Micro B Male to Micro B Female USB Connector](https://www.adafruit.com/product/3258)
* [Polycase TS-2420P 4.00 x 2.13 x 2.00 inch Enclosure](https://www.polycase.com/ts-2420p)
* The appropriate micro USB cable to connect to your computer(s).
* [6x2mm Rubber Feet](https://www.amazon.com/dp/B06XCNM69B) for the bottom of the enclosure.
* [1/2 Inch Diameter Round Labels, Glossy Clear Film](https://www.avery.com/blank/labels/94503) for the button graphics.
* [1 5/8 Inch Diameter Round Label, Matte White](https://www.avery.com/blank/labels/94507) for the Application Selector/Potentiometer label.

## Wiring Help

The pins that are used for different features are configurable using the
`PIN_*` definitions near the top of the program code
(`trinket-m0-physical-mute-button-for-video-chat.ino`).  If you don't plan on
using something (like the potentiometer to choose a chat application or the
LEDs or whatever) simply comment out the `#define` statement by adding a
comment (`//`) at the beginning of the line and the associated code won't
be executed.

However, if you are using the same hardware as above, it may be easiest to just
wire it the same way as I did.  Since this was my first Arduino project, I
found it helpful when others were specific about how they wired everything
together, so I'll do the same.  First, I soldered the header pins to the board
so that they were on "top".  Then I wired everything up as described below.
If you want things to be more permanent you could solder your wires directly
to the board and the components' pins, but be sure to install the components
in your enclosure first (specifically the buttons).

![Wiring Diagram](images/mute-button-wiring.png)

### Ground (GND)

Since everything here connects to the ground (GND) pin on the board, I took
one of the F/F jumper cables and cut it in half.  I then stripped both cut
ends.  One half of this cable will attach to the GND pin and the other half
will connect to the potentiometer later.  The wires connected to the GND pins
on the buttons will have the white plastic connector cut off and will get
their ends stripped as well.  The bare ends of all of these GND connections
will then be connected together with a wire nut.

### Button 1 (Microphone Mute)

I wired the one side of the button 1 switch (microphone mute) to the GND
terminal and the other side to PIN 0 (configured with `PIN_BUTTON_1`).
With switches, it doesn't matter which side of the switch is used for
which connection.

Since my buttons have built-in LEDs that I also wanted to control, I also
wired the `+` terminal to PIN 1 (configured with `PIN_BUTTON_1_LED`) and
the `-` terminal to GND.

To make things easy, I used one of the jumper quick connect pairs to wire one
side of the switch and the `+` terminal of the LED to the GPIO pins using the
2 wire connector and I used another pair (with the connector cut off) to go to
GND as described above.

### Button 2 (Video Toggle)

Button 2 is wired pretty much the same as button 1.  This time around, the
switch is wired to GND and PIN 3 (configured with `PIN_BUTTON_2`) and the LED
is wired to GND and PIN 4 (configured with `PIN_BUTTON_2_LED`).

### Potentiometer (Application Selector)

In my house, we currently use Zoom, Google Meet, Microsoft Teams and WebEx to
various degrees, and of course each of these programs uses a different keyboard
shortcut.  I didn't want to continuously reprogram the board every time we
switched applications so I tried to come up with a way to switch programs at
"run time".  I thought about having a switch or tap sequence cycle through
available options but thought that would be too complicated, especially for my
children.  I settled on a dial using a potentiometer.

A potentiometer has three pins.  The outer 2 pins get connected to the "power"
pin (in this case 3V - if your board has multiple power output pins, make sure
you use the one that matches the logic voltage for your GPIO pins) and GND.
While, in theory, it doesn't matter which side is which, it does affect which
side of the dial reads low (the ground side) and which reads high (the power
side), and thus how your code/label is designed.  For me, with the knob
pointing towards me, I have the left pin connected to GND (using that half of
a F/F jumper cable I mentioned in the Ground section) and the right pin
connected to the 3V pin using another F/F jumper cable.  This allows me to
create a label with the values "Zoom", "Google Meet", "MS Teams", "WebEx",
"Zoom (MacOS)", "Google Meet (MacOS)", "MS Teams (MacOS)" and "WebEx (MacOS)"
clockwise around the potentiometer's 300 degree range.  The center pin of the
potentiometer is the "signal" pin and is wired to pin 2 (configured with
`PIN_ANALOG_IN`).  Somewhat confusingly, on the Trinket M0, the pin that is
labeled 2 on the top of the board is digital input 2 and analog input A1.  Make
sure you check the documentation for whatever board you are using to ensure you
set the value of `PIN_ANALOG_IN` correctly for use with analog input.

### The Enclosure

So, I started out this project by making a "case" using cardboard and packing
tape while prototyping/beta testing.  That won't do for the long haul though.

I chose a 4 inch by ~2 inch by 2 inch plastic enclosure from Polycase.  I
drilled 2 holes evenly spaced in the top of the enclosure to mount the arcade
buttons for the microphone and video toggle.  To do this, I centered my 1"
drill bit 1 5/32" from the edge of the enclosure.  For the potentiometer, I
drilled a 5/16" hole in the center of the front of the enclosure and also made
a small slit using a 1/16" bit for the alignment peg.  Finally, I made a
rectangular hole in the lower center of the back panel for the USB jack using a
series of 1/16" drill holes, smoothed out with a utility knife and some
sandpaper.

To help drill the holes as precisely as possible, I made some
patterns/templates using paper.  I measured exactly where the holes' center
needed to be and drew the shape of the USB jack.  Then I taped the paper
templates to the enclosure and drilled 1/16" pilot holes through the paper,
exactly where they needed to be.  I took the paper off to complete the larger
holes, so that I could reuse the templates.

Even though the screws on the enclosure were recessed, I could just barely feel
the screwheads with my finger.  Since I was afraid of scratching my desk or
computer, I opted to add some tiny rubber feet to the bottom for some
additional protection.

### Labels

To complete the project, I printed Font Awesome
[microphone](https://fontawesome.com/icons/microphone?style=solid) and
[video](https://fontawesome.com/icons/video?style=solid) icons on 1/2" round
clear labels for the arcade buttons.  I used a 1 5/8" round white label for the
application selector.

## Programming the Board

Since this is Arduino code, the first thing you'll need to do is setup and
configure the Arduino IDE on your computer.  If you are on Windows, you will
also need to install the correct drivers for your microcontroller board.  If
you are using the Trinket M0, you can get the
[Windows drivers here](https://learn.adafruit.com/adafruit-trinket-m0-circuitpython-arduino/windows-driver-installation)
and the [Arduino IDE setup steps here](https://learn.adafruit.com/adafruit-trinket-m0-circuitpython-arduino/arduino-ide-setup).

Once the Arduino IDE is all setup, you'll need the code in this repository, so
either clone the repository or download it as a zip file and extract it.  Then
it _should_ be as simple as opening the .ino file in the Arduino IDE, making
sure the correct board type is selected, and pressing the "Upload" button to
compile and install the program.  Once it reboots you should be good to go!

One thing to note, if you are using the Trinket M0, you might need to double
click the reset button once the board is plugged into your computer to get it
recognized by the Arduino IDE.  If the onboard DotStar is red at that point,
try a different USB cable.  I ran into an issue where about half of my
stockpile of micro USB cables were charging-only and didn't support data.
Guess which one I grabbed first.  I wasted a bit of time on that initially.

## TODO

At some point in the future, I'd like to try to read the current microphone
mute/video state of the chat application.  However, this almost certainly
requires switching to a standard serial connection and running custom software
on the machine to figure out the state and communicate that to the
microcontroller, similarly to [toelke/google-hangouts-muter](https://github.com/toelke/google-hangouts-muter).
However that does limit the portability of this, so that future may never come.

## Development Resources

OneButton Documentation:  
https://github.com/mathertel/OneButton

Keyboard Codes:  
https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/  
https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf

Adafruit Trinket M0 Pinout Documentation:  
https://learn.adafruit.com/adafruit-trinket-m0-circuitpython-arduino/pinouts

Adafruit DotStar LED Documentation:  
https://learn.adafruit.com/adafruit-dotstar-leds/arduino-library-use  
https://github.com/adafruit/Adafruit_DotStar

## License

Since this application traces its roots to [Elliotmade's Physical Mute Button for Zoom Meetings](https://elliotmade.com/2020/04/23/physical-mute-button-for-zoom-meetings/)
project and that project was released under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/)
this is also released under the same license.

![Creative Commons License](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png)  
This work is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/).
