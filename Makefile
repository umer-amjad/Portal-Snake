CXX=g++
CPPVERSION=--std=c++17
LDLIBS=-lncurses -lpthread
SOURCES=main.cc Board.cc View.cc
HEADERS=Tile.h Board.h View.h Input.h

portal_snake: $(SOURCES) $(HEADERS)
	$(CXX) $(CPPVERSION) $(SOURCES) $(LDLIBS) -o portal_snake

clean:
	rm portal_snake