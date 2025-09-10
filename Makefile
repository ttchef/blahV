
CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -g -Iinclude -Ivendor
LDFLAGS = -lglfw -lm -lvulkan -l$(LIBNAME)

SRC = $(wildcard src/*.c) 
OBJ = $(SRC:.c=.o)

LIBNAME = blahV
EXENAME = main

all: lib/lib$(LIBNAME)

%.o: %.c 
	$(CC) -c $< -o $@ $(CFLAGS) 

lib/lib$(LIBNAME): $(OBJ)
	mkdir -p bin
	mkdir -p bin/obj
	mkdir -p lib
	ar cr lib/lib$(LIBNAME).a $(OBJ)
	rm -f $(OBJ)

install: lib/lib$(LIBNAME)
	cp lib/lib$(LIBNAME).a /usr/local/lib/ 
	cp -r include/$(LIBNAME) /usr/local/include/

uninstall:
	rm -rf /usr/local/lib/lib$(LIBNAME).a 
	rm -rf /usr/local/include/$(LIBNAME)/

example:
	./compile.sh
	$(CC) $(CFLAGS) example.c -o bin/$(EXENAME) $(LDFLAGS)

run: example
	./compile.sh
	./bin/$(EXENAME)

cloc:
	cloc . --exclude-dir=vendor,build,third_party,bin

clean:
	rm -rf lib $(OBJ) bin






