#CFLAGS=--std=c++11
CFLAGS=--std=c++0x -lpthread
DEPS=point.h lof.h p2_helper.h thread_org.h
all:	point.o lof.o p2_helper.o thread_org.o p2.o
	g++ -o P2 $^ $(CFLAGS)
%.o:	%.cpp $(DEPS)
	g++ -c -o $@ $< $(CFLAGS)
#debug:	info.cpp info.hpp
#	g++ -g -o P2 info.cpp
clean:
	rm -rf *.o *~ P2
