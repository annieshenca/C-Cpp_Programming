#######################################################################
#
# Copyright (C) 2018 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without
# the express written permission of the copyright holder.
#
#######################################################################

CFLAGS = -Wall -Isrc -lpthread -lcrypt -g
CC = g++ -std=c++17

SRC=$(wildcard src/*.cc)

all: main test crack

main: src/main.cc
	g++ -std=c++17 -o main src/main.cc

test: test.cc
#	$(CC) -o $@ $^ $(CFLAGS)
	g++ -std=c++17 -o test test.cc

crack: $(SRC) crack.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	@rm crack main test

submit: clean
	@tar czvf ~/CMPS109-Lab9.tar.gz \
	    src/*.cc src/*.h
