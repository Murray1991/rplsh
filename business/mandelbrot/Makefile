
CXX	= g++ -std=c++11
CFLAGS 	= -Wall `pkg-config --cflags opencv` #-pg
RPL	= ../../
FF	= ../../fastflow/
OPT     = -O3 #-DNDEBUG -g
LIBS 	= `pkg-config --libs opencv`

SRC=$(wildcard ff*.cpp)
BIN=$(patsubst %.cpp,%,$(SRC))

ff:	$(BIN)

sequential: sequential.cpp
	$(CXX) $(OPT) $(CFLAGS) -DPRE=seq -I$(RPL) -o $@ $< $(LIBS)

$(BIN): %: %.cpp
	$(CXX) $(OPT) $(CFLAGS) -DPRE=$(BIN) -I$(FF) -I$(RPL) -o $@ $< $(LIBS) -pthread
