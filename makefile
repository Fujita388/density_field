all:

o:
	g++ -c -std=c++11 vtk.cpp split.cpp
	g++ -std=c++11 vtk.o split.o

clean:
	rm -rf a.out *.o *.vtk
