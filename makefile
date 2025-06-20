# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

# Default target: compile, link, and run the program
all: clean compile link run c

# Compile the source files into object files
compile:
	mkdir -p bin
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/*.cpp
	mv *.o bin/

# Link the object files and create the executable
link:
	g++ -g -Wall -Weffc++ -std=c++11 -o bin/simulation bin/*.o


# Clean rule: remove compiled object files and executables
clean:
	rm -f bin/*

# Run the program with the config path argument
run:
	./bin/simulation config_file.txt 
	@echo "Program finished."

# Optional run with valgrind: Uncomment the next line if you want to run with valgrind
c: all 
	valgrind --leak-check=full --show-reachable=yes ./bin/simulation config_file.txt 
