CXX = g++
CXXFLAGS = -Wall -std=c++20
BUILDDIR=$(PWD)/build
SRC=$(wildcard src/*.cpp)
DEPS=$(wildcard include/*.hpp)
INCLUDE= -Iinclude
LDLIBS =  -lnotcurses++ -lnotcurses -lnotcurses-core
TARGET = Clock

all: $(TARGET)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $<

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $(TARGET) $(LDLIBS)

clean :
	rm *.o *.gch $(TARGET)
