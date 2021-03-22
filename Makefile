CC = g++
CFLAGS = -Wall -std=c++20
OBJS = Clock.o
LOCALLIBDIR = /usr/local/lib
LDFLAGS = -L$(LOCALLIBDIR)
LDLIBS =  -lnotcurses++ -lnotcurses -lnotcurses-core

clock : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

Clock.o : Clock.cpp Clock.hpp
	$(CC) $(CFLAGS) -c Clock.hpp Clock.cpp

clean :
	rm *.o *.gch clock
