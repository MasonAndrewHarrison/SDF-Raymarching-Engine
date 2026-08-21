CC     = gcc
CXX    = g++
CFLAGS = -Wall -O2
LIBS   = -lSDL3 -lGL -lGLEW -lm

CPP_SRC = main.cpp screen.cpp shader.cpp program.cpp
C_SRC   = event.c

CPP_OBJ = $(CPP_SRC:.cpp=.o)
C_OBJ   = $(C_SRC:.c=.o)

all: output

output: $(CPP_OBJ) $(C_OBJ)
	$(CXX) $(CPP_OBJ) $(C_OBJ) -o output $(LIBS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: output
	./output

clean:
	rm -f *.o output