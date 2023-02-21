# Yes, we're using a makefile for scripts, don't @ me.

setup:
	mkdir -p dist
	mkdir -p build
	cd build && cmake ..

build_binary:
	cd build/src && make -j4 && cp main.uf2 ../../dist

build_test:
	gcc src/main.test.c -o dist/test

build_test_run:
	make build_test
	dist/test

build_test_mac:
	gcc-12 src/main.test.c -o dist/test

build_test_run_mac:
	make build_test_mac
	dist/test

watch_test:
	inotifywait -m -e create -e close_write -e delete -r src | while read path action file; do clear && make build_test_run; done