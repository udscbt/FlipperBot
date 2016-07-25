BOARD=arduino:avr:uno
PORT=/dev/ttyACM0
BUILD_DIR=build
SRC_DIR=src
LIB_DIR=lib
INC_DIR=include
OPTIONS=--board $(BOARD) --port $(PORT) --verbose-build --verbose-upload --preserve-temp-files --pref build.path=$(BUILD_DIR)
ARDUINO=arduino
COMMAND=$(ARDUINO) $(OPTIONS) $(INO)/$(INO).ino
INO:=$(wildcard src/*.ino)
INO:=$(notdir $(INO))
INO:=$(patsubst %.ino,%,$(INO))

LIBS=FBCP Scheduler Network

default:
	@echo "Specify target"

compile: include src
	$(MAKE) common WHAT=--verify

upload: include src
	$(MAKE) common WHAT=--upload

common:
	-rm -R $(INO)
	mkdir $(INO)
	cp $(SRC_DIR)/* $(INO)
	cp $(LIB_DIR)/* $(INO)
	cp $(INC_DIR)/* $(INO)
	sed -i "s/void \* const NULL = 0;//" $(INO)/schemo.h
	for f in $(INO)/*; do sed -i "1s/^/#include <StandardCplusplus.h>\\n/" $$f; done
	./schemop -sq -f "" $(INO)/*
	$(COMMAND) $(WHAT); #rm -R $(INO)

include: $(LIBS)

src: $(LIBS)

$(LIBS):
	$(MAKE) -C ../../$@ Makefile lib
	cp ../../$@/*.h $(INC_DIR)
	cp ../../$@/*.cpp $(LIB_DIR)
	-rm $(LIB_DIR)/main.cpp

clean:
	-rm $(INC_DIR)/*
	-rm $(LIB_DIR)/*
	-rm -R $(BUILD_DIR)/*
	-rm -R $(INO)
