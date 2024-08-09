CXX=g++
CPPVERSION=--std=c++17
LDLIBS=-lncurses -lpthread
SOURCES=main.cc Board.cc View.cc

portal_snake:
	$(CXX) $(CPPVERSION) $(SOURCES) $(LDLIBS) -o portal_snake

clean:
	rm portal_snake