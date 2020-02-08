CC = clang

CFLAGS = -g -Wall -lGL -lGLU -lglut -lm

TARGETS = clean gl-test

default: gl-test

all: $(TARGETS)

gl-test: main.c 
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	$(RM) -rf penn-sh *~ *dSYM