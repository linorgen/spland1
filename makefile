# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

# # Compiler and flags
# CXX = g++
# CXXFLAGS = -std=c++17 -Wall -I./include

# # Directories
# SRCDIR = ./src
# INCDIR = ./include
# BINDIR = ./bin

# # Source and output files
# SRCS = $(wildcard $(SRCDIR)/*.cpp)
# OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(SRCS))
# TARGET = $(BINDIR)/program

# # Default target
# all: $(BINDIR) $(TARGET)

# # Build the target (link object files to create the executable)
# $(TARGET): $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o $@ $^

# # Rule for compiling .cpp files to .o files in bin directory
# $(BINDIR)/%.o: $(SRCDIR)/%.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Ensure the bin directory exists
# $(BINDIR):
# 	mkdir -p $(BINDIR)

# # Clean up
# clean:
# 	rm -rf $(BINDIR) $(TARGET)

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS =

# Directories
SRC_DIR = src
BIN_DIR = bin

# Source files and object files
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/Settlement.cpp $(SRC_DIR)/Facility.cpp
OBJS = $(BIN_DIR)/main.o $(BIN_DIR)/Settlement.o $(BIN_DIR)/Facility.o

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