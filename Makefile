##############################################################################
# Makefile for bst application
# CMP_SC 3050
# Spring 2024
# A4
# By Jim Ries (RiesJ@missouri.edu)
##############################################################################
CC = gcc
CFLAGS = -Wall -Werror -c -g -std=c11 -I.
LDFLAGS = -lm -lcs3050 -L.

%.o : %.c 
	@echo Compiling $^ ...
	@$(CC) $(CFLAGS) $^

mybst : main.o bst.o
	@echo Linking $@ ...
	@$(CC) $^ -lm -o $@
	@echo Done!

clean : 
	@rm -rf *.o
	@rm -rf mybst
	@echo All Clean!
