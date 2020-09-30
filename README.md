# Physical Mute Button for Video Chat

Build a physical mute button (for your microphone and camera) for video chat
applications like Zoom, Google Meet, Microsoft Teams and WebEx using an
[Adafruit Trinket M0](https://www.adafruit.com/product/3500).

## Acknowledgements

Thanks to Elliotmade for the inspiration on this project,
[Physical Mute Button for Zoom Meetings](https://elliotmade.com/2020/04/23/physical-mute-button-for-zoom-meetings/).

Refactored by [HaxNobody](https://gist.github.com/HaxNobody/7bde369d7a41348b8b91c1a4f358ea4a)
to extend functionality and add comments for my own understanding.

Additionally modified and extended by [Jim Stump](https://github.com/jimstump/trinket-m0-physical-mute-button-for-video-chat-2-button)
to add MacOS support as well as additional video chat programs.  Also moved to
the [Adafruit Trinket M0](https://www.adafruit.com/product/3500) board.

## How Does This Work

This program will send USB HID keyboard presses to activate microphone and
video functions using the application's default keyboard shortcuts.
 * A momentary press on button 1 will toggle microphone mute on or off.
 * Press and hold button 1 to activate PTT (Push-to-Talk) functionality for
   supported applications (Zoom-only).  All other apps will perform the
   mute toggle.
 * A momentary press on button 2 will toggle video on or off for supported
   applications (not WebEx).

The following video chat programs are supported for selection:
 * Zoom
 * Google Meet
 * Microsoft Teams
 * Cisco WebEx

The video chat program can be chosen using a potentiometer dial attached to
the PIN_ANALOG_IN pin.  On startup and when the buttons are pressed, the
analog value from the potentiometer is read to determine which video chat
application and platform keyboard shortcuts should be used.  Once determined,
the color of the onboard DotStar pixel on the Trinket M0 will be changed to
indicate the current selection (Zoom is blue, Google Meet is Lime, Microsoft
Teams is dark violet, and WebEx is gold).

## Current Hardware

While there are plenty of parts that can be used to build this project, I
thought it might be helpful to list out what I'm currently using in my
prototype.

* [Adafruit Trinket M0](https://www.adafruit.com/product/3500)
* [24mm Arcade Button with Built-in LED](https://www.amazon.com/dp/B06Y29LBJ4)
* [10K Linear Potentiometer](https://www.adafruit.com/product/562)
* [Potentiometer Knob](https://www.adafruit.com/product/2047)
* [Arcade Button Quick-Connect Wires](https://www.adafruit.com/product/1152)
* F/F and M/F jumper cables
* [Panel Mount USB C to Micro B Male](https://www.adafruit.com/product/4056)
* Still looking for a final enclosure, but [this 4.00 x 2.13 x 2.00 in one](https://www.polycase.com/ts-2420p) looks promising.

## TODO

 * Try to read the current microphone mute/video state
   * This probably requires switching to a serial connection and running
     custom software on the machine something like https://github.com/toelke/google-hangouts-muter

## Development Resources

OneButton Docs:  
https://github.com/mathertel/OneButton

Keyboard Codes:  
https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf

## License

Since this application traces its roots to [Elliotmade's Physical Mute Button for Zoom Meetings](https://elliotmade.com/2020/04/23/physical-mute-button-for-zoom-meetings/)
project and that project was released under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/)
this is also released under the same license.

![Creative Commons License](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png)  
This work is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/).
