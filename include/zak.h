#ifndef ZAK_H
#define ZAH_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <stdarg.h>

/*** defines ***/
#define ZAK_VERSION "0.0.1"
#define TAB_STOP 8

#define CTRL_KEY(k) ( (k) & 0x1f )

enum editorKey {
  ARROW_LEFT = 1000,
  ARROW_RIGHT ,
  ARROW_UP ,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

/*** data ***/

/**
  *@brief Stores a row of text
  */
typedef struct erow{
  int size;
  int rsize;
  char* chars;
  char* render;
} erow;

/**
  *@brief A struct containing our editor state
  */
struct editorConfig {
  int cx, cy; //cursor pos
  int rx; // rendered cursor x pos
  int rowoff; //row offset
  int coloff; //column offset
  int screenrows;
  int screencols;
  int numrows;
  erow* row;
  char* filename;
  char statusmsg[80];
  time_t statusmsg_time;
  struct termios origin_termios;
};

struct editorConfig E;

/*** append buffer ***/

/**
  *@brief append buffer struct used to write the screen
  */
struct abuf {
  char *b;
  int len;
};
#define ABUF_INIT {NULL, 0}

/**
  *@brief appends a string to a buffer
  */
void abAppend(struct abuf* ab, const char* c, int length);

/**
  *@brief frees the buffer
  */
void abFree(struct abuf* ab);


/*** terminal ***/

/**
  * @brief Changes terminal attributes to enable raw mode.
  * It passes the data as-is to the program without interpreting
  * any of the special characters.
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

/**
  *@brief waits until one key is pressed
  * and returns it.
  */
int editorReadKey();

/**
  *@brief gets the cursor position.
  * used in getWindowSize if the request fails.
  */
int getCursorPosition(int *rows, int *cols);

/**
  *@brief gets the window size using ioctl() with the TIOCGWINSZ request.(Get Window Size)
  */
int getWindowSize(int* rows, int* columns);

/*** input ***/

/**
  *@brief changes the cursor position depending on the key
  */
void editorMoveCursor(int key);

/**
  *@brief waits for akey from editorReadKey()
  * and then processes it.
  * Ex: ctrl-q : exit the program.
  */
void editorProcessKeyPress();

/*** output ***/

/**
  *@brief Sets up row offset depending on the file cursor
  */
void editorScroll();

/**
  *@brief draws '~' after the end of the file being edited.
  */
void editorDrawRows();


/**
  *@brief writes status bar
  */
void editorDrawStatusBar(struct abuf *ab);

/**
  *@brief renders the interface.
  * clears the screen and reposition the cursor.
  */
void editorRefreshScreen();

/**
  *@brief
  */
void editorSetStatusMessage(const char *fmt, ...);


/**
  *@brief draw message bar
  */
void editorDrawMessageBar(struct abuf *ab);

/*** init ***/
/**
  *@brief initialise the fields in E struct.
  */
void initEditor();



/*** File I/O ***/

/**
  *@brief Opens and Reads file from a disk
  */
void editorOpen(char*);


/*** row operations ***/

/**
  *@brief allocates space for a new erow, then copies it to the end
  * of E.row array.
  */
void editorAppendRow(char*, int);

/**
  *@brief writes the string of  row in the render .
  */
void editorUpdateRow(erow *row);

/**
  *@brief get the rendered x pos of the cursor
  */
int editorRowCxToRx(erow *row, int cx);
#endif
