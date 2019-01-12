#Compiler
# we dont want other compiler to be used yet.
CXX = g++

#Flags
# CXX_LINKER_FLAGS

# Remove warning and add -Wall Flag
CXX_ASSEMBLER_FLAGS := -std=c++11 $(ADD_G++_FLAGS) 

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

OUTPUT_DIR = $(OBJ_DIR)/Matrix $(BIN_DIR)/Matrix

.PHONY: all
all:
	make directories
	make $(BIN_DIR)/mat

.PHONY: directories
directories:
	mkdir -p $(OUTPUT_DIR)

# BIN/Executable Rules
$(BIN_DIR)/mat: $(OBJ_DIR)/Matrix/Matrix.o
	$(CXX) $^ -o $@

# OBJ/object Rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXX_ASSEMBLER_FLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -rf bin/ build/
