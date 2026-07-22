# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Target
TARGET = $(BIN_DIR)/smart_library

# Source files
SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/Book.cpp $(SRC_DIR)/Library.cpp
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/Book.o $(OBJ_DIR)/Library.o

# Default target
all: directories $(TARGET)

# Create directories
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Link
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/Library.h
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR)/Book.o: $(SRC_DIR)/Book.cpp $(INCLUDE_DIR)/Book.h
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR)/Library.o: $(SRC_DIR)/Library.cpp $(INCLUDE_DIR)/Library.h
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run
run: all
	./$(TARGET)

.PHONY: all clean run directories