#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>
#include <ostream>

class Matrix {
public:
    // DATA MEMBERS
    std::vector<std::vector<float>> matrix;

    // TYPES OF MATRIX REPRESENTATION, IF NEEDED LATER
    // enum majorOrder {};

    // OPTIONS FOR FUNCTIONS
    enum convolMethod {simpleConvol, matrixMult};
    enum nonLinearActMethod {relu, tanH, softmax, sigmoid};
    enum poolingMethod {maxPooling, avgPooling};

    // CONSTRUCTORS
    Matrix();
    Matrix(std::vector<std::vector<float>> inputMatrix);
    Matrix(std::string fileName, int rowNumber);

    // FUNCTION MEMBERS
    Matrix convolution(const Matrix& kernel, bool doPadding, convolMethod method);

    Matrix padding(bool returnInSame, int rowPad, int columnPad);

    Matrix nonLinearActivation(nonLinearActMethod method, bool returnInSame);

    Matrix pooling(poolingMethod method, bool returnInSame);

    // CONVERTOR
    void toOStream(std::ostream& toThisOStream);

    // HELPERS ONLY APPLICABLE TO MATRIX CLASS
private:
    static void splitColumnMajorAndPushBackRowMajor(std::string const& original, 
        char separator, std::vector<std::vector<float>>& putInMatrix);
};


#endif  /* MATRIX_H */