CXX=clang++
#CXX=g++

all:
	$(CXX) traverse_pixels.cpp -I/usr/include/opencv4 `pkg-config --libs opencv4` -o run -O3

clean:
	rm ./run
