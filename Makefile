TARGET = simulator
SOURCE = src/main.cpp
SOURCE-MACHINE = src/machine/instruction.cpp src/machine/machine.cpp
SOURCE-UNITS = src/units/alu.cpp src/units/units.cpp
SOURCES = $(SOURCE) $(SOURCE-MACHINE) $(SOURCE-UNITS)
CODEDIR = ./examples/p2
MAKEREGISTER = make-register
TESTTARGET = make-test
TESTSCRIPT = .test.sh

all: $(SOURCES)
	g++ -std=c++1y -g -o $(TARGET) -O2 -Wall $(SOURCES)

test:
	g++ -std=c++1y -o $(TESTTARGET) -O2 -Wall $(TESTTARGET).cpp

test-all: all test	
	for file in $(basename $(notdir $(wildcard $(CODEDIR)/*.s))) ; do \
		./${TESTSCRIPT} $${file};\
	done
