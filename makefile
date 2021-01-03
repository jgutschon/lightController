FQBN ?= arduino:avr:uno
PORT ?= COM4

ifdef v
override v = -v
endif

build:
	arduino-cli compile $(v) -b $(FQBN) ./lightController

upload:
	arduino-cli upload -b $(FQBN) -p $(PORT) ./lightController

clean:
	del /Q /F /S *.hex
	del /Q /F /S *.elf