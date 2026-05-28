CXX = g++
CXXFLAGS = -g

all: output

output: main.o
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: output
	./output

clean:
	rm -f *.o output