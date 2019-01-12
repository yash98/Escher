#include "Matrix.h"

#include <fstream>

Matrix::Matrix(std::vector<std::vector<float>> inputMatrix) {
    matrix = inputMatrix;
}

Matrix::Matrix(std::string fileName) {
    std::ifstream inputFile;
    
    inputFile.open(fileName, std::ifstream::in);
    if (inputFile.fail()) {
        perror(fileName.c_str());
    }

    // format of file? discuss

    inputFile.close();
}

Matrix::Matrix convolution(const Matrix& kernel, bool doPadding, Matrix::convolMethod method) {}

Matrix::Matrix nonLinearActivation(Matrix::nonLinearActMethod method, bool returnInSame) {}

Matrix::Matrix pooling(Matrix::poolingMethod method, bool returnInSame) {}