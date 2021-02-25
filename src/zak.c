#include<zak.h>

/*** terminal ***/
void enableRawMode(){
  if (tcgetattr(STDIN_FILENO, &E.origin_termios) == -1){/*gets the terminos stuct*/
    die("tcgetattr");
  }
  atexit(disableRawMode);/* executes the disable function at exit*/

  struct termios raw = E.origin_termios;/* to not change the attributes of origin*/

  /* (in bitwise operators) ~ is NOT, &= (and + assign) */

  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);/* turns off crt-m, ctrl-s and ctrl-q and some other flags*/
  raw.c_oflag &= ~(OPOST);/* turns off output processing*/
  raw.c_cflag |= (CS8);/* sets the char size to 8 bits */
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);/* turns off echo, cononical mode, ctrl-v,  ctrl-c and ctrl-z*/

  /* read() timeout*/
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1){
    die("tcsetattr");
  };/*then it sets it*/
}

void disableRawMode(){
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.origin_termios) == -1){
    die("tcsetattr");
  }
}

void die(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);//clears the display.
  write(STDOUT_FILENO, "\x1b[H", 3);//repositions the cursor.
  perror(s);
  exit(1);
}

int editorReadKey(){
  int nread;
  char c;
  while( (nread = read(STDIN_FILENO, &c, 1)) != 1 ){
    if ( nread == -1 && errno != EAGAIN){
      die("read");
    }
  }
  if (c == '\x1b') {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1) {
      return '\x1b';}
    if (read(STDIN_FILENO, &seq[1], 1) != 1) {
      return '\x1b';}
      if (seq[0] == '[') {
        if (seq[1] >= '0' && seq[1] <= '9') {
          if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
          if (seq[2] == '~') {
            switch (seq[1]) {
              case '1': return HOME_KEY;
              case '3': return DEL_KEY;
              case '4': return END_KEY;
              case '5': return PAGE_UP;
              case '6': return PAGE_DOWN;
              case '7': return HOME_KEY;
              case '8': return END_KEY;
            }
          }
        } else {
          switch (seq[1]) {
            case 'A': return ARROW_UP;
            case 'B': return ARROW_DOWN;
            case 'C': return ARROW_RIGHT;
            case 'D': return ARROW_LEFT;
            case 'H': return HOME_KEY;
            case 'F': return END_KEY;
          }
        }
      } else if (seq[0] == 'O') {
        switch (seq[1]) {
          case 'H': return HOME_KEY;
          case 'F': return END_KEY;
      }
    }
    return '\x1b';
  } else {
    return c;
  }
}

int getCursorPosition(int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) {
    return -1;
  }
  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1){
      break;
    }
    if (buf[i] == 'R') {
      break;
    }
    i++;
  }
  buf[i] = '\0';
  if (buf[0] != '\x1b' || buf[1] != '[') {
    return -1;
  }
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) {
    return -1;
  }
  return 0;
}

int getWindowSize(int* rows, int* columns){
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
      return -1;
    }
    return getCursorPosition(rows, columns);
  } else {
    *columns = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

/*** input ***/
void editorMoveCursor(int key) {
  switch (key) {
    case ARROW_LEFT:
      if (E.cx != 0) {
        E.cx--;
      }
      break;
    case ARROW_RIGHT:
      if (E.cx != E.screencols - 1) {
        E.cx++;
      }
      break;
    case ARROW_UP:
      if (E.cy != 0) {
        E.cy--;
      }
      break;
    case ARROW_DOWN:
      if (E.cy != E.screenrows - 1) {
        E.cy++;
      }
      break;
    }
}

void editorProcessKeyPress(){
  int c = editorReadKey();
  int times = E.screenrows;
  switch(c){
    case CTRL_KEY('q'):
      write(STDOUT_FILENO, "\x1b[2J", 4);//clears the display.
      write(STDOUT_FILENO, "\x1b[H", 3);//repositions the cursor.
      exit(0);
      break;

    case HOME_KEY:
      E.cx = 0;
      break;
    case END_KEY:
      E.cx = E.screencols - 1;
      break;


    case PAGE_UP:
    case PAGE_DOWN:
      {
          while (times--)
            editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
      }
      break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      editorMoveCursor(c);
      break;
  }
}


/*** output ***/
void editorDrawRows(struct abuf *ab) {
  int y;
  char welcome[80];
  int welcomelen;
  int padding;

  for (y = 0; y < E.screenrows; y++) {
  if (y == E.screenrows / 3) {
    welcomelen = snprintf(welcome, sizeof(welcome),"ZAK editor -- version %s", ZAK_VERSION);
    if (welcomelen > E.screencols) {
      welcomelen = E.screencols;
    }
    padding = (E.screencols - welcomelen) / 2;
    if (padding) {
      abAppend(ab, "~", 1);
      padding--;
    }
    while (padding--) {
      abAppend(ab, " ", 1);
    }
    abAppend(ab, welcome, welcomelen);
  } else {
    abAppend(ab, "~", 1);
  }
    abAppend(ab, "\x1b[K", 3);
    if (y < E.screenrows - 1) { // exception last line we dont \r\n
      abAppend(ab, "\r\n", 2);
    }

  }
}

void editorRefreshScreen() {
  struct abuf ab = ABUF_INIT;

  abAppend(&ab, "\x1b[?25l", 6);//hides the cursor.
  abAppend(&ab, "\x1b[H", 3);//repositions the cursor.

  editorDrawRows(&ab);

  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
  abAppend(&ab, buf, strlen(buf));//reposition the cursor.

  abAppend(&ab, "\x1b[?25h", 6);//shows back the cursor.

  write(STDOUT_FILENO, ab.b, ab.len);//writes the buffer
  abFree(&ab);
}


/*** init ***/
void initEditor() {
  E.cx = 0;
  E.cy = 0;
  if (getWindowSize(&E.screenrows, &E.screencols) == -1) {
    die("getWindowSize");
  }
}


/*** append buffer ***/
void abAppend(struct abuf* ab, const char* c, int length) {
  char *new = realloc(ab->b, ab->len + length);

  if (new == NULL) return;

  memcpy(&new[ab->len], c, length);
  ab->b = new;
  ab->len += length;
}

void abFree(struct abuf *ab) {
  free(ab->b);
}
