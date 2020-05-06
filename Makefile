TARGET = simulator
SOURCES = src/main.cpp src/machine/instruction.cpp src/machine/machine.cpp src/machine/units.cpp
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
