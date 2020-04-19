CC = clang

CFLAGS = -g -Wall -lGL -lGLU -lglut -lm -lGLEW

TARGETS = clean gl-test

default: gl-test

all: $(TARGETS)

gl-test: main.o shaderprogram.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $^

shaderprogram.o: shaderprogram.c
	$(CC) $(CFLAGS) -c $^

clean:
	$(RM) -rf penn-sh *~ *dSYM