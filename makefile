# Makefile

CXX = g++
LIBRARY = libutility.so
SRCS = Q1.cpp Q2.cpp Q3.cpp Q4.cpp

all: arrivals full_schedule airplane updateDB

arrivals: Q1.o $(LIBRARY)
	$(CXX) -o arrivals.out Q1.o -L. -lutility -Wl,-rpath=`pwd`

full_schedule: Q2.o $(LIBRARY)
	$(CXX) -o full_schedule.out  Q2.o -L. -lutility -Wl,-rpath=`pwd`

airplane: Q3.o $(LIBRARY)
	$(CXX) -o airplane.out  Q3.o -L. -lutility -Wl,-rpath=`pwd`

updateDB: Q4.o $(LIBRARY)
	$(CXX) -o updateDB.out  Q4.o -L. -lutility -Wl,-rpath=`pwd`

%.o: %.cpp
	$(CXX) -c $<

$(LIBRARY): utility.o
	$(CXX) -shared -o $(LIBRARY) utility.o

utility.o: utility.cpp utility.h
	$(CXX) -c -fPIC utility.cpp


TARGETS = arrivals.out full_schedule.out airplane.out updateDB.out
OBJECTS = Q1.o Q2.o Q3.o Q4.o
SHARED_OBJECTS = libutility.so
UTILITY_OBJECT = utility.o
.PHONY: clean
clean:
	rm -f $(TARGETS) $(OBJECTS) $(SHARED_OBJECTS) $(UTILITY_OBJECT)
