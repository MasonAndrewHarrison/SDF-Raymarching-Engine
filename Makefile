CC     = gcc
CXX    = g++
CFLAGS = -Wall -O2
LIBS   = -lSDL3 -lGL -lGLEW -lm
DEBUG_FLAGS = -Wall -g -fsanitize=address -fno-omit-frame-pointer

CPP_SRC = main.cpp rendering/screen.cpp rendering/shader.cpp program.cpp rendering/primitives.cpp rendering/voxelGrid.cpp
C_SRC   = input/event.c

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

debug: $(CPP_SRC) $(C_SRC)
	$(CXX) $(CPP_SRC) $(C_SRC) -o output -O2 $(DEBUG_FLAGS) $(LIBS)

clean:
	rm -f *.o output