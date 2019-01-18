#Compiler
# we dont want other compiler to be used yet.
CXX = g++

#Flags
# CXX_LINKER_FLAGS

# Remove warning and add -Wall Flag
CXX_ASSEMBLER_FLAGS := $(ADD_G++_FLAGS) 
INCLUDE_FLAGS = -I include/

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXX_ASSEMBLER_FLAGS +=-g
endif

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

OUTPUT_DIR = $(OBJ_DIR)/Matrix $(BIN_DIR)

.PHONY: all
all:
	make directories
	make $(BIN_DIR)/matrix

.PHONY: directories
directories:
	mkdir -p $(OUTPUT_DIR)

# BIN/Executable Rules
$(BIN_DIR)/matrix: $(OBJ_DIR)/Matrix/Matrix.o $(OBJ_DIR)/Matrix/Util.o
	$(CXX) $^ -o $@

# OBJ/object Rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXX_ASSEMBLER_FLAGS) -c $^ -o $@ $(INCLUDE_FLAGS)

.PHONY: clean
clean:
	rm -rf bin/ build/
