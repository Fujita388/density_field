all:

o:
	g++ -c -std=c++11 vtk.cpp split.cpp
	g++ -std=c++11 vtk.o split.o

mp4:
	ffmpeg -i density.%04d.png -pix_fmt yuv420p density.mp4

clean:
	rm -rf a.out *.o *.vtk *.png
