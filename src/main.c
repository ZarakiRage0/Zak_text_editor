
#include<zak.h>



int main(int argc, char* argv[]) {
  enableRawMode();
  initEditor();
  if(argc >= 2){
    editorOpen(argv[1]);
  }

  editorSetStatusMessage("HELP: Ctrl-Q = quit");

  while (1) {
    editorRefreshScreen();
    editorProcessKeyPress();
  }

  return 0;
}
