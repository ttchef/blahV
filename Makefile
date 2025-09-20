
CC = gcc # clang
CFLAGS = -Wall -Wextra -Wconversion -g -Iinclude -Ivendor -fvisibility=hidden
#CFLAGS = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -O0 -Wall -Wextra -Iinclude -Ivendor -fvisibility=hidden

LDFLAGS = -lglfw -lm -lvulkan 

SRC = $(wildcard src/*.c) 
OBJ = $(SRC:.c=.o)

LIBNAME = blahV
EXENAME = main

BUILD ?= static

ifeq ($(BUILD),shared)
	CFLAGS += -fPIC
	CFLAGS += -DBLAHV_BUILD_SHARED
else 
	CFLAGS += -DBLAHV_BUILD_STATIC
endif

all: $(BUILD)

%.o: %.c 
	$(CC) -c $< -o $@ $(CFLAGS) 

shared: $(OBJ)
	mkdir -p lib 
	$(CC) -shared -o lib/lib$(LIBNAME).so $(OBJ) $(LDFLAGS)
	rm -f $(OBJ)

static: $(OBJ)
	mkdir -p lib
	ar cr lib/lib$(LIBNAME).a $(OBJ)
	rm -f $(OBJ)

install: $(BUILD)
ifeq ($(BUILD),static)
		cp lib/lib$(LIBNAME).a /usr/local/lib/
else 
		cp lib/lib$(LIBNAME).so /usr/local/lib/
endif
		cp -r include/$(LIBNAME) /usr/local/include/

uninstall:
	rm -rf /usr/local/lib/lib$(LIBNAME).a 
	rm -rf /usr/local/lib/lib$(LIBNAME).so
	rm -rf /usr/local/include/$(LIBNAME)/

compile_shaders:
	./compile.sh

example:
	./compile.sh
	$(CC) $(CFLAGS) example.c -o $(EXENAME) $(LDFLAGS) -l$(LIBNAME)

run: example
	./compile.sh
	./$(EXENAME)

cloc:
	cloc . --exclude-dir=vendor,build,third_party,bin,compile_commands.json

clean:
	rm -rf lib $(OBJ) bin

