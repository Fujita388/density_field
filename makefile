all:

CPPFLAGS=-std=c++11
CPPFILE=vtk.cpp split.cpp
OFILE=vtk.o split.o

o:
	$(CXX) -c $(CPPFLAGS) $(CPPFILE)
	$(CXX) $(CPPFLAGS) $(OFILE)

# divisibled by 2 → mogrifyでリサイズ
mp4:
	ffmpeg -i density.%04d.png -pix_fmt yuv420p density.mp4

clean:
	$(RM) $(OFILE) a.out *.vtk *.png
