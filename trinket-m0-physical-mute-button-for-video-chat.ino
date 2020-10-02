/**
 * # Physical Mute Button for Video Chat
 * 
 * Build a physical mute button (for your microphone and camera) for video chat
 * applications like Zoom, Google Meet, Microsoft Teams and WebEx using an
 * [Adafruit Trinket M0](https://www.adafruit.com/product/3500).
 * 
 * See the [README](README.md) for more information.
 * 
 * ## Acknowledgements
 * 
 * Thanks to Elliotmade for the inspiration on this project,
 * [Physical Mute Button for Zoom Meetings](https://elliotmade.com/2020/04/23/physical-mute-button-for-zoom-meetings/).
 * 
 * Refactored by [HaxNobody](https://gist.github.com/HaxNobody/7bde369d7a41348b8b91c1a4f358ea4a)
 * to extend functionality and add comments for my own understanding.
 * 
 * Additionally modified and extended by [Jim Stump](https://github.com/jimstump/trinket-m0-physical-mute-button-for-video-chat)
 * to add MacOS support as well as additional video chat programs.  Also moved to
 * the [Adafruit Trinket M0](https://www.adafruit.com/product/3500) board.
 * 
 * ## License
 * 
 * Since this application traces its roots to [Elliotmade's Physical Mute Button for Zoom Meetings](https://elliotmade.com/2020/04/23/physical-mute-button-for-zoom-meetings/)
 * project and that project was released under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/)
 * this is also released under the same license.
 * 
 * ![Creative Commons License](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png)
 * This work is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/).
 * 
 **/
#include <Adafruit_DotStar.h>  // Library to drive DotStar RGB LEDs
#include <Keyboard.h>          // Library for sending keystrokes as an HID device over USB.
#include "OneButton.h"         // Library for button input functions
#include "html_color_names.h"  // Some color definitions using HTML color names/hex values

// GPIO Pins
#define PIN_LED LED_BUILTIN
#define PIN_BUTTON_1 0
#define PIN_BUTTON_1_LED 1
#define PIN_BUTTON_2 3
#define PIN_BUTTON_2_LED 4
#define PIN_ANALOG_IN A1 // (D2 == A1 on the Trinket M0)
#define PIN_DOTSTAR_DATA 7
#define PIN_DOTSTAR_CLOCK 8

// Application Constants
#define APP_ZOOM 1
#define APP_GOOGLE_MEET 2
#define APP_MS_TEAMS 3
#define APP_WEBEX 4

/**
 * Should we use MacOS keyboard shortcuts or not.
 * 
 * true - use MacOS shortcuts
 * false - use Windows/Chromebook
 */
bool useMacShortcuts;

/**
 * Byte to hold one of the APP_* definitions to indicate which application we
 * should send keyboard shortcuts for
 */
byte currentApplication;

/**
 * Output debug information to the Serial console or not
 */
bool debugMode = true;

/**
 * Defines the OneButton instance for the Microphone button (button1)
 * 
 * The button is connected to `PIN_BUTTON_1` and GND, which is active low
 * and uses the internal pull-up resistor.
 */
#ifdef PIN_BUTTON_1
  OneButton button1 = OneButton(
    PIN_BUTTON_1, // Pin Number
    true,         // Button is active LOW
    true          // Enable internal pull-up resistor
  );
#endif

/**
 * Defines the OneButton instance for the Video button (button2)
 * 
 * The button is connected to `PIN_BUTTON_2` and GND, which is active low
 * and uses the internal pull-up resistor.
 */
#ifdef PIN_BUTTON_2
  OneButton button2 = OneButton(
    PIN_BUTTON_2, // Pin Number
    true,         // Input is active LOW
    true          // Enable internal pull-up resistor
  );
#endif

/**
 * Adafruit_DotStar instance for the onboard LED strip
 */
#define DOTSTAR_NUMPIXELS 1
#if DOTSTAR_NUMPIXELS > 0 && defined(PIN_DOTSTAR_DATA) && defined(PIN_DOTSTAR_CLOCK)
  Adafruit_DotStar strip(DOTSTAR_NUMPIXELS, PIN_DOTSTAR_DATA, PIN_DOTSTAR_CLOCK, DOTSTAR_BGR);
#endif


/**
 * The setup routine.
 * 
 * This runs once when you press reset
 */
void setup() {
  if (debugMode) {
    Serial.begin(9600);
  }

  #if DOTSTAR_NUMPIXELS > 0 && defined(PIN_DOTSTAR_DATA) && defined(PIN_DOTSTAR_CLOCK)
    // configure the strip before getting the application so we set the correct color
    strip.begin(); // Initialize pins for output
  #endif
  
  setApplicationSettings();

  #ifdef PIN_BUTTON_1
    button1.attachClick(handleButton1Click); // Set up button 1 for microphone mute toggle function
    button1.attachLongPressStart(handleButton1LongPressStart); // Set up button 1 for microphone temporary unmute function
    button1.attachLongPressStop(handleButton1LongPressStop); // Set up button 1 for microphone temporary unmute release function
    button1.setPressTicks(300); // Reduce long-press delay for button 1 to make temporary unmute more responsive (default 1000)
    //button1.attachDoubleClick(handleButton1DoubleClick); // Set up button 1 double-click to complete MacOS Keyboard Setup routine
  #endif

  #ifdef PIN_BUTTON_2
    button2.attachClick(handleButton2Click); // Set up button 2 for video toggle function
    button2.attachLongPressStart(handleButton2Click); // bind long-press start to the click handler so it still does _something_
    button2.setPressTicks(300); // Reduce long-press delay for button 2 to make it more responsive (default 1000)
  #endif

  Keyboard.begin();
  Keyboard.releaseAll(); // Clear any current key presses that may be happening for some reason
  delay(50); // Delay before entering loop

  // initialize the digital pins as an output for the LEDs.
  // and turn it on to indicate that we are ready
  #ifdef PIN_LED
    pinMode(PIN_LED, OUTPUT);
    turnLedOn();
  #endif
  #ifdef PIN_BUTTON_1_LED
    pinMode(PIN_BUTTON_1_LED, OUTPUT);
    turnButton1LedOn();
  #endif
  #ifdef PIN_BUTTON_2_LED
    pinMode(PIN_BUTTON_2_LED, OUTPUT);
    turnButton2LedOn();
  #endif
}

/**
 * The main loop routine
 * 
 * Check status of buttons in a continuous loop
 */
void loop() {
  #ifdef PIN_BUTTON_1
    button1.tick();
  #endif
  #ifdef PIN_BUTTON_2
    button2.tick();
  #endif
}

/**
 * Output debug information, if enabled by setting debugMode equal to true
 */
void debug(const String &s) {
  if (debugMode) {
    //Keyboard.println(s);
    Serial.println(s);
  }
}

/**
 * Figure out what application we should send keyboard shortcuts for.
 * 
 * Read the potentiometer value from PIN_ANALOG_IN and use that to determine
 * the current application setting
 */
void setApplicationSettings() {
  #ifdef PIN_ANALOG_IN
    // values range from 0 to 1023
    // we have 8 application options
    // switch is labeled so 
    // min (0) is the "center" for application 1 selection
    // 1/7 (146) app 2
    // 2/7 (292) qpp 3
    // 3/7 (438) app 4
    // 4/7 (585) app 5
    // 5/7 (731) app 6
    // 6/7 (877) app 7
    // max (1023) is the "center" for application 8 selection
    //
    // indicators are labeled at 1/3 (341) and 2/3 (683) of the range
    // 0 (0) - 1/14 (73) choose application 1
    // 1/14 (73) - 3/14 (219) choose application 2
    // 3/14 (219) - 5/14 (365) choose application 3
    // 5/14 (365) - 7/14 (512) choose application 4
    // 7/14 (512) - 9/14 (658) choose application 5
    // 9/14 (658) - 11/14 (804) choose application 6
    // 11/14 (804) - 13/14 (950) choose application 7
    // 13/14 (950) - max (1023) choose application 8
  
    // read the value
    int sensorValue = analogRead(PIN_ANALOG_IN);
    char debugString[100];
    sprintf(debugString, "Sensor Value: %d", sensorValue);
    debug(debugString);
    
    if (sensorValue < 73) {
      debug("Application: Zoom (PC)");
      useMacShortcuts = false;
      currentApplication = APP_ZOOM;
      updateDotStarColor(COLOR_BLUE);
    } else if (sensorValue >= 73 && sensorValue < 219) {
      debug("Application: Google Meet (PC)");
      useMacShortcuts = false;
      currentApplication = APP_GOOGLE_MEET;
      updateDotStarColor(COLOR_LIME);
    } else if (sensorValue >= 219 && sensorValue < 365) {
      debug("Application: MS Teams (PC)");
      useMacShortcuts = false;
      currentApplication = APP_MS_TEAMS;
      updateDotStarColor(COLOR_DARKVIOLET);
    } else if (sensorValue >= 365 && sensorValue < 512) {
      debug("Application: WebEx (PC)");
      useMacShortcuts = false;
      currentApplication = APP_WEBEX;
      updateDotStarColor(COLOR_GOLD);
    } else if (sensorValue >= 512 && sensorValue < 658) {
      debug("Application: Zoom (MAC)");
      useMacShortcuts = true;
      currentApplication = APP_ZOOM;
      updateDotStarColor(COLOR_BLUE);
    } else if (sensorValue >= 658 && sensorValue < 804) {
      debug("Application: Google Meet (MAC)");
      useMacShortcuts = true;
      currentApplication = APP_GOOGLE_MEET;
      updateDotStarColor(COLOR_LIME);
    } else if (sensorValue >= 804 && sensorValue < 950) {
      debug("Application: MS Teams (MAC)");
      useMacShortcuts = true;
      currentApplication = APP_MS_TEAMS;
      updateDotStarColor(COLOR_DARKVIOLET);
    } else if (sensorValue >= 950) {
      debug("Application: WebEx (MAC)");
      useMacShortcuts = true;
      currentApplication = APP_WEBEX;
      updateDotStarColor(COLOR_GOLD);
    }
  #else
    debug("No Analog Input - Using Default Application: Zoom (PC)");
    useMacShortcuts = false;
    currentApplication = APP_ZOOM;
    updateDotStarColor(HUE_BLUE);
  #endif
}

/** 
 *  Button One Click Handler
 *  
 *  This function will be called when button 1 is pressed for more than 50ms and
 *  less than 300ms (configured in `setup` using `button1.setPressTicks`).
 */
void handleButton1Click() {  
  // first we turn off the LED to indicate that we received the click
  turnLedOff();
  turnButton1LedOff();
  
  // update the current application in case the nob was changed
  setApplicationSettings();

  // clear any current key presses
  Keyboard.releaseAll();

  // toggle the microphone mute state
  toggleMicrophoneMute();

  // turn the LED back on to indicate that we are ready for another command
  delay(50);
  turnLedOn();
  turnButton1LedOn();
}

/**
 * Button One Double-click Handler
 * 
 * This function is called when button 1 is pressed twice within 600ms
 */
void handleButton1DoubleClick() {
  // first we turn off the LED to indicate that we received the click
  turnLedOff();
  turnButton1LedOff();
  
  // update the current application in case the nob was changed
  setApplicationSettings();

  runMacKeyboardSetup();

  // turn the LED back on to indicate that we are ready for another command
  delay(50);
  turnLedOn();
  turnButton1LedOn();
}

/**
 * Button One Long Press Start Handler
 * 
 * This function will be called when button 1 is pressed and held down for more
 * than 300ms (configured in `setup` using `button1.setPressTicks`).
 */
void handleButton1LongPressStart() {
  // first we turn off the LED to indicate that we received the long press
  turnLedOff();
  turnButton1LedOff();
  
  // update the current application in case the nob was changed
  setApplicationSettings();

  // clear any current key presses
  Keyboard.releaseAll();

  startPushToTalk();
}

/**
 * Button One Long Press Stop Handler
 * 
 * This function will be called when button 1 is released after being held down.
 */
void handleButton1LongPressStop() {
  stopPushToTalk();

  // don't update the application here - whatever we started we want to stop
  
  // turn the LED back on to indicate that we are ready for another command
  delay(50);
  turnLedOn();
  turnButton1LedOn();
}

/** 
 *  Button Two Click Handler
 *  
 *  This function will be called when button 2 is pressed for more than 50ms and
 *  less than 1000ms
 */
void handleButton2Click() {
  // first we turn off the LED to indicate that we received the click
  turnLedOff();
  turnButton2LedOff();
  
  // update the current application in case the nob was changed
  setApplicationSettings();

  // clear any current key presses
  Keyboard.releaseAll();

  // toggle the video state
  toggleVideoState();

  // turn the LED back on to indicate that we are ready for another command
  delay(50);
  turnLedOn();
  turnButton2LedOn();
}

/**
 * Toggle the microphone mute state for the selected application
 */
void toggleMicrophoneMute() {
  debug("Toggle Microphone Mute");
  switch (currentApplication) {
    case APP_ZOOM:
      toggleMicrophoneMuteZoom();
      break;
    
    case APP_GOOGLE_MEET:
      toggleMicrophoneMuteGoogleMeet();
      break;
    
    case APP_MS_TEAMS:
      toggleMicrophoneMuteTeams();
      break;
    
    case APP_WEBEX:
      toggleMicrophoneMuteWebEx();
      break;
  }
}

/**
 * Toggle the microphone mute state in Zoom
 */
void toggleMicrophoneMuteZoom() {
  if (useMacShortcuts) {
    // Mac doesn't seem to have a keyboard shortcut for
    // bringing Zoom into the foreground
    
    // send cmd-shift-A for Mac
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('a');
    Keyboard.releaseAll();
  } else {
    // send ctl-alt-shift to bring Zoom into the foreground
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.releaseAll();
    delay(50); // Give Zoom a chance to get into the foreground
    
    // send alt-A for PC/Chromebook/etc
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('a');
    Keyboard.releaseAll();
  }
}

/**
 * Toggle the microphone mute state in Google Meet
 */
void toggleMicrophoneMuteGoogleMeet() {
  if (useMacShortcuts) {
    // send cmd-D for Mac
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('d');
    Keyboard.releaseAll();
  } else {
    // send ctrl-D for PC/Chromebook/etc
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('d');
    Keyboard.releaseAll();
  }
}

/**
 * Toggle the microphone mute state in Microsoft Teams
 */
void toggleMicrophoneMuteTeams() {
  if (useMacShortcuts) {
    // send cmd-shift-M for Mac
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('m');
    Keyboard.releaseAll();
  } else {
    // send ctrl-shift-M for PC/Chromebook/etc
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('m');
    Keyboard.releaseAll();
  }
}

/**
 * Toggle the microphone mute state in Cisco WebEx
 */
void toggleMicrophoneMuteWebEx() {
  if (useMacShortcuts) {
    // send cmd-shift-M for Mac
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('m');
    Keyboard.releaseAll();
  } else {
    // send ctrl-M for PC/Chromebook/etc
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('m');
    Keyboard.releaseAll();
  }
}

/**
 * Start Push-to-talk (PTT) for the selected application
 * 
 * Currently only implemented for Zoom
 */
void startPushToTalk() {
  debug("Start Push To Talk");
  switch (currentApplication) {
    case APP_ZOOM:
      startPushToTalkZoom();
      break;
    
    case APP_GOOGLE_MEET:
    case APP_MS_TEAMS:
    case APP_WEBEX:
      // Not implemented in Google Meet, Microsoft Teams, or WebEx so we are
      // just going to toggle the microphone mute for a better UX, otherwise
      // the button doesn't do _anything_ on a long press detection
      toggleMicrophoneMute();
      break;
  }
}

/**
 * Start Push-to-talk (PTT) for Zoom
 */
void startPushToTalkZoom() {
  if (useMacShortcuts) {
    // Mac doesn't seem to have a keyboard shortcut for
    // bringing Zoom into the foreground
    
    // Press and hold the space key for Mac
    Keyboard.press(' ');
  } else {
    // send ctl-alt-shift to bring Zoom into the foreground
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.releaseAll();
    delay(50); // Give Zoom a chance to get into the foreground
    
    // Press and hold the space key for PC/Chromebook/etc
    Keyboard.press(' ');
  }
}

/**
 * Stop Push-to-talk (PTT) for the selected application
 * 
 * Currently only implemented for Zoom
 */
void stopPushToTalk() {
  debug("Stop Push To Talk");
  // Delay to prevent cutting off the speaker's last word
  delay(300);
  
  switch (currentApplication) {
    case APP_ZOOM:
      stopPushToTalkZoom();
      break;
    
    case APP_GOOGLE_MEET:
    case APP_MS_TEAMS:
    case APP_WEBEX:
      // Not implemented in Google Meet, Microsoft Teams, or WebEx
      break;
  }
}

/**
 * Stop Push-to-talk (PTT) for Zoom
 */
void stopPushToTalkZoom() {
  // Release the PTT key to re-mute Zoom after temporary unmute
  Keyboard.releaseAll();
}

/**
 * Toggle the video state for the selected application
 */
void toggleVideoState() {
  debug("Toggle Video State");
  switch (currentApplication) {
    case APP_ZOOM:
      toggleVideoStateZoom();
      break;
    
    case APP_GOOGLE_MEET:
      toggleVideoStateGoogleMeet();
      break;
    
    case APP_MS_TEAMS:
      toggleVideoStateTeams();
      break;
    
    case APP_WEBEX:
      // Not implemented in WebEx
      //toggleVideoStateWebEx();
      break;
  }
}

/**
 * Toggle the video state for Zoom
 */
void toggleVideoStateZoom() {
  if (useMacShortcuts) {
    // Mac doesn't seem to have a keyboard shortcut for
    // bringing Zoom into the foreground
    
    // send cmd-shift-V for Mac
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('v');
    Keyboard.releaseAll();
  } else {
    // send ctl-alt-shift to bring Zoom into the foreground
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.releaseAll();
    delay(50); // Give Zoom a chance to get into the foreground
    
    // send alt-V for PC/Chromebook/etc
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('v');
    Keyboard.releaseAll();
  }
}

/**
 * Toggle the video state in Google Meet
 */
void toggleVideoStateGoogleMeet() {
  if (useMacShortcuts) {
    // send cmd-E for Mac
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('e');
    Keyboard.releaseAll();
  } else {
    // send ctrl-E for PC/Chromebook/etc
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('e');
    Keyboard.releaseAll();
  }
}

/**
 * Toggle the video state in Microsoft Teams
 */
void toggleVideoStateTeams() {
  if (useMacShortcuts) {
    // send cmd-shift-O for Mac
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('o');
    Keyboard.releaseAll();
  } else {
    // send ctrl-shift-O for PC/Chromebook/etc
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('o');
    Keyboard.releaseAll();
  }
}

/**
 * Hits the correct keys to get through the MacOS Keyboard Setup wizard to
 * identify the keyboard as a "ANSI (United States and others)" standard
 * 101-key or 102-key keyboard so that it is useable.
 * 
 * Presses the "Z" key (the key immediately to the right of the Shift key on the
 * left side of the keyboard) followed by the "/" (and ?) key (the key
 * immediately to the left of the Shift key on the right side of the keyboard).
 */
void runMacKeyboardSetup() {
  debug("Run Mac Keyboard Setup Keys");
  delay(1000);
  Keyboard.releaseAll(); // Clear any current key presses
  Keyboard.press('z'); // Press the key immediately to the right of the Shift key on the left side of the keyboard
  Keyboard.releaseAll(); // Clear any current key presses
  delay(2000);
  Keyboard.press('/'); // Press the key immediately to the left of the Shift key on the right side of the keyboard (/ and ?)
  Keyboard.releaseAll(); // Clear any current key presses
  delay(1000);
}

/**
 * Turn the LED (connected to `PIN_LED` and GND) on
 */
void turnLedOn() {
  #ifdef PIN_LED
    digitalWrite(PIN_LED, HIGH);
  #endif
}

/**
 * Turn the LED (connected to `PIN_LED` and GND) off
 */
void turnLedOff() {
  #ifdef PIN_LED
    digitalWrite(PIN_LED, LOW);
  #endif
}

/**
 * Turn on the LED (connected to `PIN_BUTTON_1_LED` and GND) inside Button 1
 */
void turnButton1LedOn() {
  #ifdef PIN_BUTTON_1_LED
    digitalWrite(PIN_BUTTON_1_LED, HIGH);
  #endif
}

/**
 * Turn off the LED (connected to `PIN_BUTTON_1_LED` and GND) inside Button 1
 */
void turnButton1LedOff() {
  #ifdef PIN_BUTTON_1_LED
    digitalWrite(PIN_BUTTON_1_LED, LOW);
  #endif
}

/**
 * Turn on the LED (connected to `PIN_BUTTON_2_LED` and GND) inside Button 2
 */
void turnButton2LedOn() {
  #ifdef PIN_BUTTON_2_LED
    digitalWrite(PIN_BUTTON_2_LED, HIGH);
  #endif
}

/**
 * Turn off the LED (connected to `PIN_BUTTON_2_LED` and GND) inside Button 2
 */
void turnButton2LedOff() {
  #ifdef PIN_BUTTON_2_LED
    digitalWrite(PIN_BUTTON_2_LED, LOW);
  #endif
}

/**
 * Update the color of the onboard DotStar
 * 
 * color is a single "packed" 32-bit RGB color.
 * 
 * Use the COLOR_* definitions from "html_color_names.h" for ease of use
 */
void updateDotStarColor(uint32_t color) {
  #if DOTSTAR_NUMPIXELS > 0 && defined(PIN_DOTSTAR_DATA) && defined(PIN_DOTSTAR_CLOCK)
    // color is passed through strip.gamma32() to provide 'truer' colors
    // before filling the strip
    strip.fill(strip.gamma32(color));
    strip.show();
  #endif
}
