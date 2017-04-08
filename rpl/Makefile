CXX 		= 	g++
CXX_FLAGS 	= 	-std=c++11 -Wall -O3
CPP_FILES	= 	$(wildcard *.cpp)
OBJ_FILES 	= 	$(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.o)))

.PHONY: clean

rplsh: $(OBJ_FILES)
	$(CXX) $(CXX_FLAGS) -o $@ $^

obj/%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

clean:
	rm -rf *.o
	rm -rf obj/*
