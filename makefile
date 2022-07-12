#
# PROGRAM: ASSIGNMENT #2
# AUTHOR:  Leonart Jaos
#

CXXFLAGS = -Wall -Werror -std=c++14

z1911688_project2: z1911688_project2.o
	g++ $(CXXFLAGS) -o z1911688_project2 z1911688_project2.o

z1911688_project2.o: z1911688_project2.cpp
	g++ $(CXXFLAGS) -c z1911688_project2.cpp

clean:
	rm -f *.o z1911688_project2
