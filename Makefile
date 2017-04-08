CXX 		= 	g++
CXX_FLAGS 	= 	-std=c++11 -Wall -O3
CPP_FILES	= 	$(wildcard rpl/*.cpp)
OBJ_FILES 	= 	$(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.o)))

.PHONY: clean show

show:
	echo $(OBJ_FILES)

rplsh: $(OBJ_FILES)
	$(CXX) $(CXX_FLAGS) -o $@ $^

obj/%.o: rpl/%.cpp
	$(CXX) $(CXX_FLAGS) -I rpl/ -c -o $@ $<

clean:
	rm -rf *.o
	rm -rf obj/*
