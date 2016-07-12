CXX=g++-4.9
CXXFLAGS=-Wall -Wno-sign-compare -Wno-strict-aliasing -std=c++11 -O0
OBJS=algorithms/*.cpp tsplib.cpp main.cpp

mytsp:
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	rm -f mytsp *.o *.s
