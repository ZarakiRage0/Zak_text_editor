.PHONY: all clean distclean
CC = gcc
CPPFLAGS = -I./include
CFLAGS = -Wall -Wextra -pedantic

OBJ_DIR = ./bin/misc
BUILD_DIR = ./bin/build
SRC_DIR = ./src
INCLUDE_DIR = ./include

all :
  

clean:
	rm -rf $(OBJ_DIR)/*.o

distclean:
	make clean
	rm -rf $(BUILD_DIR)/*
