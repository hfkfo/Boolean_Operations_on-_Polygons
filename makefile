CC = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes`
OBJ = _BOOLEAN_OP.so
all: $(OBJ)

_BOOLEAN_OP.so: src/_BOOLEAN_OP.cpp include/BOOLEAN_OP.hpp
	$(CXX) $(CXXFLAGS) $<  -I include `python3-config --includes --ldflags` -o $@  -lblas -lmkl_rt


test:
	python3 -m pytest -v

clean:
	rm -rf *.so __pycache__ .pytest* 

