CC     = gcc
CFLAGS = -Wall -O2 -Icglm/include
LIBS   = -lGL -lGLEW -lglfw -lm
SRC    = main.c shader.c glError.c

all: output

output: $(SRC)
	$(CC) $(SRC) -o output $(CFLAGS) $(LIBS)

run: output
	./output

debug: $(SRC)
	$(CC) $(SRC) -o output -Wall -g $(LIBS)

run-debug: debug
	gdb ./output

clean:
	rm -f output