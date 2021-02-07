#include <termios.h>
#include <unistd.h>
#include <stdlib.h>



/**
  *@brief turns off echoing and canonical mode.
  */
void enableRawMode();

/**
  *@brief Used to return the terminal attributes to their orginal state
  */
void disableRawMode();
