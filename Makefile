CXX    = g++
CFLAGS = -Wall -O2
LIBS   = -lSDL3 -lGL -lm -lglm -lGLEW
SRC    = main.cpp screen.cpp

all: output

output: $(SRC)
	$(CXX) $(SRC) -o output $(CFLAGS) $(LIBS)

run: output
	./output

clean:
	rm -f output