# Nipplio Arduino Core Library

This library can be used to link your Arduino with the [nipplio](https://nipplio.web.app) website and use it e.g. as a nippleboard.

## Usage

```
#include <Nipplio.h>

Nipplio nipplio;

void setup()
{
  // the initial setup for nipplio
  nipplio.setup();
  String slotNames[] = {"Button name top", "Button name left", "Button name right", "Button name bottom"};
  // set the slotnames for each button. These button names will be shown in the nipplio web ui for the board config.
  nipplio.setSlotNames(slotNames);
}

void loop()
{
  // the nipplio loop function
  nipplio.loop();
}

void onButtonPressed() {
  // call this function if you want to trigger a sound. The slotId must be an Integer starting at Index 1
  nipplio.triggerSlotWithNumber(slotId);
}

```
