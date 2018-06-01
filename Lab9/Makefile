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

all: cracker server crack

cracker: src/cracker.cc
	g++ -std=c++17 -o cracker src/cracker.cc

server: server.cc
	g++ -std=c++17 -o server server.cc

crack: $(SRC) crack.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	@rm crack cracker server

submit: clean
	@tar czvf ~/CMPS109-Lab9.tar.gz \
	    src/*.cc src/*.h
