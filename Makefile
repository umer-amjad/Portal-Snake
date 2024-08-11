CXX=g++
CPPVERSION=--std=c++17
LDLIBS=-lncursesw -lpthread
SOURCES=main.cc Board.cc View.cc
HEADERS=Tile.h Board.h View.h Input.h

portal_snake: $(SOURCES) $(HEADERS)
	$(CXX) $(CPPVERSION) $(SOURCES) $(LDLIBS) -o portal_snake.out

clean:
	rm portal_snake.out