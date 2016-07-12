CXX=g++-4.9
CXXFLAGS=-Wall -Wno-strict-aliasing -std=c++11 -O0
OBJS=tsp.h main.cpp

mytsp:
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	rm -f mytsp *.o *.s