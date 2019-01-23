CPPFLAGS= -O3 -Wall -Wextra -Wfatal-errors
ccs=Model.cc Camera.cc Light.cc Material.cc Face.cc Sphere.cc Ray.cc
objs=Model.o Camera.o Light.o Material.o Face.o Sphere.o Ray.o
assignment=raytracer

compile:
	g++ -I ./ -c ${ccs} ${assignment}.cc ${CPPFLAGS} 
	g++ -o $(assignment) ${assignment}.o ${objs}

tar:
	tar -cv $(MAKEFILE_LIST) README.txt driver*.txt *.obj *.mtl *.cc *.h *.ppm Eigen >Rictor-$(assignment).tar

clean:
	rm -f $(assignment) $(assignment).tar *.o *.gch
