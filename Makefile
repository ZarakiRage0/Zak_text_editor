.PHONY: all clean distclean
CC = gcc
CPPFLAGS = -I./include
CFLAGS = -Wall -Wextra -pedantic -std=c99

OBJ_DIR = ./bin/misc
BUILD_DIR = ./bin/build
SRC_DIR = ./src
INCLUDE_DIR = ./include

all : $(BUILD_DIR)/main


$(BUILD_DIR)/main : $(OBJ_DIR)/zak.o $(OBJ_DIR)/main.o
	$(CC) $^ -o $@

$(OBJ_DIR)/main.o : $(SRC_DIR)/main.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJ_DIR)/zak.o : $(SRC_DIR)/zak.c $(INCLUDE_DIR)/zak.h
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@



clean:
	rm -rf $(OBJ_DIR)/*.o

distclean:
	make clean
	rm -rf $(BUILD_DIR)/*
