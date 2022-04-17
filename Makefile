# Yes, we're using a makefile for scripts, don't @ me.

setup:
	mkdir -p dist
	mkdir -p build
	cd build && cmake ..

build_binary:
	cd build/src && make -j4 && cp main.uf2 ../../dist