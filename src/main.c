
#include<zak.h>

int main(void) {
  char c;
  c = '\0';
  enableRawMode();
  
  while (1) {

    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
      die("read");
    }

    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }

    if (c == 'q') break;/*press q to exit */
  }

  return 0;
}
