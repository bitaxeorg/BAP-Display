#include "display.h"



#ifdef T_DISPLAY
DisplayDriver *currentDisplayDriver = &tDisplayDriver;
#endif



// Initialize the display
void initDisplay()
{
  currentDisplayDriver->initDisplay();
}

// Alternate screen state
void alternateScreenState()
{
  currentDisplayDriver->alternateScreenState();
}

// Alternate screen rotation
void alternateScreenRotation()
{
  currentDisplayDriver->alternateScreenRotation();
}

// Draw the loading screen
void drawLoadingScreen()
{
  currentDisplayDriver->loadingScreen();
}

// Draw the setup screen
void drawSetupScreen()
{
  currentDisplayDriver->setupScreen();
}

// Reset the current cyclic screen to the first one
void resetToFirstScreen()
{
  currentDisplayDriver->current_cyclic_screen = 0;
}

// Switches to the next cyclic screen without drawing it
void switchToNextScreen()
{
  currentDisplayDriver->current_cyclic_screen = (currentDisplayDriver->current_cyclic_screen + 1) % currentDisplayDriver->num_cyclic_screens;
}

// Draw the current cyclic screen
void drawCurrentScreen(unsigned long mElapsed)
{
  currentDisplayDriver->cyclic_screens[currentDisplayDriver->current_cyclic_screen](mElapsed);
}

// Animate the current cyclic screen
void animateCurrentScreen(unsigned long frame)
{
  currentDisplayDriver->animateCurrentScreen(frame);
}

// Do LED stuff
void doLedStuff(unsigned long frame)
{
  currentDisplayDriver->doLedStuff(frame);
}
