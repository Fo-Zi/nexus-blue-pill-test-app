# Simple Makefile for Blue Pill flashing

.PHONY: build flash clean

build:
	mkdir -p build
	cd build && cmake .. && make

flash: build
	openocd -f openocd.cfg -c "program build/blue_pill_test_app verify reset exit"

clean:
	rm -rf build
