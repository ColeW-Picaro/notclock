CC = g++
CFLAGS = -Wall -std=c++20
OBJS = Clock.o Main.o
SRCS = Main.cpp Clock.cpp
LOCALLIBDIR = /usr/local/lib
LDFLAGS = -L$(LOCALLIBDIR)
LDLIBS =  -lnotcurses++ -lnotcurses -lnotcurses-core

clock : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

Clock.o : Clock.cpp Clock.hpp
	$(CC) $(CFLAGS) -c Clock.cpp Clock.hpp

Main.o : Main.cpp Clock.hpp Clock.cpp
	$(CC) $(CFLAGS) -c Main.cpp

clean :
	rm *.o *.gch clock
