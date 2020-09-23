FQBN ?= arduino:avr:uno

build:
	arduino-cli compile -b $(FQBN) ./lightController

upload:
	arduino-cli upload -b $(FQBN) -p COM4 ./lightController

clean:
	del /Q /F /S *.hex
	del /Q /F /S *.elf