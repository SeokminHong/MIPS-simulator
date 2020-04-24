TARGET = simulator
SOURCES = src/main.cpp src/machine/instruction.cpp src/machine/machine.cpp src/machine/units.cpp
ASSEMBLER = assembler
ASSEMBLEDIR = ./examples/p2

$(TARGET): $(SOURCES)
	g++ -std=c++1y -o $(TARGET) -O2 -Wall $(SOURCES)

test1: $(TARGET)
	./$(TARGET) examples/p1/p1e1.txt 5 > output/p1/out-p1e1.txt && diff examples/p1/p1e1-output.txt output/p1/out-p1e1.txt

test2: $(TARGET)
	./$(TARGET) examples/p1/p1e2.txt 10 > output/p1/out-p1e2.txt && diff examples/p1/p1e2-output.txt output/p1/out-p1e2.txt

test3: $(TARGET)
	./$(TARGET) examples/p1/p1e3.txt 15 > output/p1/out-p1e3.txt && diff examples/p1/p1e3-output.txt output/p1/out-p1e3.txt

test4: $(TARGET)
	./$(TARGET) examples/p1/p1e4.txt 30 > output/p1/out-p1e4.txt && diff examples/p1/p1e4-output.txt output/p1/out-p1e4.txt


test: test1 test2 test3 test4 

assemble:
	g++ -std=c++1y -o $(ASSEMBLER) -O2 -Wall $(ASSEMBLER).cpp
	
	for file in $(wildcard $(ASSEMBLEDIR)/*.s) ; do \
		./$(ASSEMBLER) $${file}; \
	done
	