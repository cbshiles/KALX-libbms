OBJ = obj
CXXFLAGS = -D_DEBUG -g -Wall -std=c++0x

curves_test : curves_test.cpp

curves_test.obj : basis_spline.h polynomial.h

.PHONY : clean
clean :
	-rm -f curves_test.exe *.$(OBJ)
