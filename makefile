# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

# Default target: compile, link, and run the program
all: compile link run

# Compile the source files into object files
compile:
	@echo "Compiling source files..."
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/*.cpp
	@echo "Compilation finished."

# Link the object files and create the executable
link:
	@echo "Linking object files..."
	g++ -g -Wall -Weffc++ -std=c++11 -o bin/main *.o
	@echo "Linking finished."

# Optional clean rule: remove compiled object files and executables
clean:
	rm -f *.o
	rm -f bin/*

# Run the program with the config path argument
run:
	@echo "Running the program with the config file..."
	./bin/main config_file.txt 
	@echo "Program finished."

# Optional run with valgrind: Uncomment the next line if you want to run with valgrind
# valgrind --leak-check=full --show-reachable=yes ./bin/main
