#include<zak.h>

struct termios origin_termios;

void enableRawMode(){
  tcgetattr(STDIN_FILENO, &origin_termios); /*get the terminos stuct*/
  atexit(disableRawMode);/* execute the disable function at exit*/


  struct termios raw = origin_termios;/* to not change the attributes of origin*/
  raw.c_lflag &= ~(ECHO | ICANON);/* turns off echo and cononical mode*/
   /* ~ is NOT, &= and + assign*/

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);/*then it sets it*/
}

void disableRawMode(){
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin_termios);
}
