#Compiler
# we dont want other compiler to be used yet.
CXX = g++

#Flags
# CXX_LINKER_FLAGS

# Remove warning and add -Wall Flag
CXX_ASSEMBLER_FLAGS := -std=c++11 $(ADD_G++_FLAGS) 
INCLUDE_FLAGS = -I /opt/OpenBLAS/include/ -I /opt/intel/compilers_and_libraries_2019.1.144/linux/mkl/include/ -DMKL_ILP64 -m64
SHARED_LINK_FLAGS = -L/opt/OpenBLAS/lib -lopenblas -lpthread   -Wl,--start-group /opt/intel/compilers_and_libraries_2019.1.144/linux/mkl/lib/intel64/libmkl_intel_ilp64.a /opt/intel/compilers_and_libraries_2019.1.144/linux/mkl/lib/intel64/libmkl_gnu_thread.a /opt/intel/compilers_and_libraries_2019.1.144/linux/mkl/lib/intel64/libmkl_core.a -Wl,--end-group -lgomp -lpthread -lm -ldl

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
$(BIN_DIR)/mat: $(OBJ_DIR)/Matrix/Matrix.o $(OBJ_DIR)/Matrix/Util.o
	$(CXX) $^ -o $@ $(SHARED_LINK_FLAGS)

# OBJ/object Rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXX_ASSEMBLER_FLAGS) -c $^ -o $@ $(INCLUDE_FLAGS)

.PHONY: clean
clean:
	rm -rf bin/ build/
