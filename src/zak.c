#include<zak.h>

struct termios origin_termios;

void enableRawMode(){

  if (tcgetattr(STDIN_FILENO, &origin_termios)){/*gets the terminos stuct*/
    die("tcgetattr");
  }
  atexit(disableRawMode);/* executes the disable function at exit*/

  struct termios raw = origin_termios;/* to not change the attributes of origin*/

  /* (in bitwise operators) ~ is NOT, &= (and + assign) */

  raw.c_lflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);/* turns off crt-m, ctrl-s and ctrl-q and some other flags*/
  raw.c_oflag &= ~(OPOST);/* turns off output processing*/
  raw.c_cflag |= (CS8);/* sets the char size to 8 bits */
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);/* turns off echo, cononical mode, ctrl-v,  ctrl-c and ctrl-z*/

  /* read() timeout*/
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)){
    die("tcsetattr");
  };/*then it sets it*/
}

void disableRawMode(){
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin_termios) == -1){
    die("tcsetattr");
  }
}

void die(const char *s) {
  perror(s);
  exit(1);
}
