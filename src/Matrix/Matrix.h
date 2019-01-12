#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>

class Matrix {
public:
    // DATA MEMBERS
    std::vector<std::vector<float>> matrix;

    // OPTIONS FOR FUNCTIONS
    enum convolMethod {simpleConvol, matrixMult};
    enum nonLinearActMethod {relu, tanH, softmax, sigmoid};
    enum poolingMethod {maxPooling, avgPooling};

    // CONSTRUCTORS
    Matrix(std::vector<std::vector<float>> inputMatrix);
    Matrix(std::string fileName);

    // FUNCTION MEMBERS
    Matrix convolution(const Matrix& kernel, bool doPadding, convolMethod method);

    Matrix nonLinearActivation(nonLinearActMethod method, bool returnInSame);

    Matrix pooling(poolingMethod method, bool returnInSame);
};


#endif  /* MATRIX_H */