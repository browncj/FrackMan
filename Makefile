CC = g++    # On lnxsrv07 or 09, replace this with /usr/local/cs/bin/g++
CXXFLAGS = -g
LIBS = -lglut -lGL -lGLU
RPATH =

# On lnxsrv07 or 09, uncommment the following line
#RPATH = -Wl,--rpath=/usr/local/cs/lib64

OBJECTS = $(filter-out test.o, $(filter-out main.o, $(patsubst %.cpp, %.o, $(wildcard *.cpp))))
HEADERS = $(wildcard *.h)

.PHONY: default all clean


all: FrackMan

%.o: %.cpp $(HEADERS)
	$(CC) $(CXXFLAGS) -c -std=c++11 $< -o $@

FrackMan: $(OBJECTS) main.o
	$(CC) $(CXXFLAGS) $(RPATH) $(OBJECTS) main.o $(LIBS) -o $@

test: $(OBJECTS) test.o
	$(CC) $(CXXFLAGS) $(RPATH) $(OBJECTS) test.o $(LIBS) -o $@

clean:
	rm -f *.o
	rm -f FrackMan
	rm -f test
