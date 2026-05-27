CXX = g++

all: output

output: main
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) -c $< -o $@

run: output
	./output

clean:
	rm -f *.o output