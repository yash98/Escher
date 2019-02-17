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
    enum convolMethod {simpleConvol, matrixMultPthread, matrixMultOpenBLAS, matrixMultMKL};
    enum nonLinearActMethod {relu, tanH, softmax, sigmoid};
    enum poolingMethod {maxPooling, avgPooling};

    // CONSTRUCTORS
    Matrix();
    Matrix(std::vector<std::vector<float>> inputMatrix);
    Matrix(std::string fileName, int rowNumber);

    // SPECIAL CLASS FUNCTIONS
    Matrix copy();

    void toOStream(std::ostream& toThisOStream);

    void checkConsistency();

    // FUNCTION MEMBERS
    Matrix convolution(const Matrix& kernel, bool doPadding, convolMethod method, int numOfThreads);

    void padding(int rowPad, int columnPad);

    void nonLinearActivation(nonLinearActMethod method);

    Matrix pooling(poolingMethod method, int num_rows, int num_columns, int stride);

    void openblasMatrixMult(float* a, float* b, float* c, 
        int m, int n, int numOfThreads);
    void mklMatrixMult(float* a, float* b, float* c, 
        int m, int n, int numOfThreads);

    void addMatrixInto(Matrix& other);

    void addConstInto(float givenFloat);

    static Matrix zeroes(int rows, int columns);

    static Matrix randomMatrix(int rows, int columns);

    static void data_store();

    // HELPERS ONLY APPLICABLE TO MATRIX CLASS
private:
    static void splitColumnMajorAndPushBackRowMajor(std::string const& original, 
        char separator, std::vector<std::vector<float>>& putInMatrix);
};


#endif  /* MATRIX_H */