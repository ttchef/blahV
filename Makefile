
CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lglfw -lm -lvulkan -l:lib$(LIBNAME).a

SRC = $(wildcard src/*.c) 
OBJ = $(SRC:.c=.o)

LIBNAME = vulkan-render
EXENAME = main

all: run

bin/obj/%.o: %.c 
	$(CC) -c $< -o $@ $(CFLAGS) 

lib/$(LIBNAME): $(OBJ)
	mkdir -p lib
	ar cr lib/$(LIBNAME).a $(OBJ)
	rm -f $(OBJ)

install: lib/$(LIBNAME)
	cp lib/$(LIBNAME).a /usr/local/lib/ 
	cp -r include/$(LIBNAME) /usr/local/include/

uninstall:
	rm -rf /usr/local/lib/$(LIBNAME).a 
	rm -rf /usr/local/include/$(LIBNAME)/

example: install 
	./compile.sh
	$(CC) $(CFLAGS) example.c -o bin/$(EXENAME) $(LDFLAGS)

run: example
	./compile.sh
	./$(EXENAME)

cloc:
	cloc . --exclude-dir=vendor,build,third_party,bin

clean:
	rm -rf lib $(OBJ)

.PHONY all clean install uninstall 




