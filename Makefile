CXX=g++-4.9
CXXFLAGS=-Wno-sign-compare -Wno-sizeof-pointer-memaccess -Wno-strict-aliasing -std=c++11 -O3
OBJS=algorithms/*.cpp tsplib.cpp main.cpp

mytsp:
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	rm -f mytsp *.o *.s
