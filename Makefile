CC     = gcc
CFLAGS = -Wall -O2
LIBS   = -lGL -lGLEW -lglfw -lraylib -lm

all: output

output: main.c
	$(CC) main.c -o output $(CFLAGS) $(LIBS)

run: output
	./output

debug: main.c
	$(CC) main.c -o output -Wall -g $(LIBS)

run-debug: debug
	gdb ./output

clean:
	rm -f output