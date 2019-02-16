#Compiler
# we dont want other compiler to be used yet.
CXX = g++

#Flags
# CXX_LINKER_FLAGS

# Remove warning and add -Wall Flag

CXX_ASSEMBLER_FLAGS := $(ADD_G++_FLAGS) 

MAT_INCLUDE_FLAGS = -Iinclude/ -I${OPENBLASROOT}/include/ -DMKL_ILP64 -m64 -I${MKLROOT}/include  
MAT_SHARED_LINK_FLAGS = -L${OPENBLASROOT}/lib -lopenblas -lpthread -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_ilp64 -lmkl_gnu_thread -lmkl_core -lgomp -lpthread -lm -ldl

DIGIT_INCLUDE_FLAGS = -Iinclude/ -I${OPENCVINCLUDE}/
DIGIT_SHARED_LINK_FLAGS = $(MAT_SHARED_LINK_FLAGS) -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CXX_ASSEMBLER_FLAGS +=-g
endif


# Directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

OUTPUT_DIR = $(OBJ_DIR)/Matrix $(OBJ_DIR)/DigitRecognition $(BIN_DIR)

.PHONY: all
all:
	make directories
	make $(BIN_DIR)/digit

.PHONY: directories
directories:
	mkdir -p $(OUTPUT_DIR)

# BIN/Executable Rules
# digit exec
$(BIN_DIR)/digit: $(OBJ_DIR)/DigitRecognition/DigitRecog.o $(OBJ_DIR)/Matrix/Matrix.o $(OBJ_DIR)/Matrix/data_store.o $(OBJ_DIR)/Matrix/Matrix_MKL.o $(OBJ_DIR)/Matrix/Matrix_OpenBLAS.o $(OBJ_DIR)/Matrix/Util.o
	$(CXX) $^ -o $@ $(DIGIT_SHARED_LINK_FLAGS)

# matrix library exec
$(BIN_DIR)/matrix: $(OBJ_DIR)/Matrix/Matrix.o $(OBJ_DIR)/Matrix/data_store.o $(OBJ_DIR)/Matrix/Matrix_MKL.o $(OBJ_DIR)/Matrix/Matrix_OpenBLAS.o $(OBJ_DIR)/Matrix/Util.o
	$(CXX) $^ -o $@ $(MAT_SHARED_LINK_FLAGS)

# digit OBJ Rules
$(OBJ_DIR)/DigitRecognition/%.o: $(SRC_DIR)/DigitRecognition/%.cpp
	$(CXX) $(CXX_ASSEMBLER_FLAGS) -c $^ -o $@ $(DIGIT_INCLUDE_FLAGS)

# MAT OBJ Rules
$(OBJ_DIR)/Matrix/Matrix_MKL.o: $(SRC_DIR)/Matrix/Matrix_MKL.cpp
	$(CXX) $(CXX_ASSEMBLER_FLAGS) -c $^ -o $@ -Iinclude/ -DMKL_ILP64 -m64 -I${MKLROOT}/include 

$(OBJ_DIR)/Matrix/Matrix_OpenBLAS.o: $(SRC_DIR)/Matrix/Matrix_OpenBLAS.cpp
	$(CXX) $(CXX_ASSEMBLER_FLAGS) -c $^ -o $@ -Iinclude/ -I${OPENBLASROOT}/include/

$(OBJ_DIR)/Matrix/%.o: $(SRC_DIR)/Matrix/%.cpp
	$(CXX) $(CXX_ASSEMBLER_FLAGS) -c $^ -o $@ -Iinclude/

.PHONY: clean
clean:
	rm -rf bin/ build/