#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>


/**
  * @brief Changes terminal attributes to enable raw mode.
  * It passes the data as-is to the program without interpreting any of the special characters.
  * Ex: ctrl-c, ctrl-s ....
  * When the program exits, it disables the raw mode.
  */
void enableRawMode();

/**
  *@brief Used to return the terminal attributes to their orginal state
  */
void disableRawMode();

/**
  *@brief Prints an error message and exit the program.
  */
void die(const char *s);
