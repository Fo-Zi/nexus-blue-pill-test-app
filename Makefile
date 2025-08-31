# Simple Makefile for Blue Pill flashing

.PHONY: flash
flash:
	openocd -f openocd.cfg -c "program build/blue_pill_test_app verify reset exit"