# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS =

# Directories
SRC_DIR = src
BIN_DIR = bin

# Source files and object files
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/Settlement.cpp $(SRC_DIR)/Facility.cpp $(SRC_DIR)/SelectionPolicy.cpp $(SRC_DIR)/Plan.cpp $(SRC_DIR)/Simulation.cpp $(SRC_DIR)/Actions.cpp $(SRC_DIR)/Auxiliary.cpp
OBJS = $(BIN_DIR)/main.o $(BIN_DIR)/Settlement.o $(BIN_DIR)/Facility.o $(BIN_DIR)/SelectionPolicy.o $(BIN_DIR)/Plan.o $(BIN_DIR)/Simulation.o $(BIN_DIR)/Action.o $(BIN_DIR)/Auxiliary.o

# Output executable
TARGET = $(BIN_DIR)/program

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)