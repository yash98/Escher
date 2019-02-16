#include "Matrix/Matrix.h"

#include <iostream>
#include <string>

int main (int argc, char* argv[]) {
    //  ./yourcode.out convolution_withpadding_matrixmult padsize matrix1.txt matrix1_numrows matrix2.txt matrix2_numrows
    if (argc<2) {
        std::cerr << "No args provided. Check README.md for usage." << std::endl;
        return -1;
    }
    std::string functionName = argv[1];
    if (functionName == "convolution") {
        if (argc < 9) {
            std::cerr << "Number of args incorrect." << std::endl;
            return -1;
        }
        // we take no padsize
        Matrix input = Matrix(argv[2], atoi(argv[3]));
        Matrix kernel = Matrix(argv[4], atoi(argv[5]));
        bool doPadding;
        if (std::string(argv[6]) == "pad") {
            doPadding = true;
        } else if (std::string(argv[6]) == "noPad") {
            doPadding = false;
        } else {
            std::cerr << "Specify info about padding." << std::endl;
            return -1;
        }
        Matrix::convolMethod method;
        if (std::string(argv[7]) == "simpleConvol") {
            method = Matrix::simpleConvol;
        } else if (std::string(argv[7]) == "matrixMultPthread") {
            method = Matrix::matrixMultPthread;
        } else if (std::string(argv[7]) == "matrixMultOpenBLAS") {
            method = Matrix::matrixMultOpenBLAS;
        } else if (std::string(argv[7]) == "matrixMultMKL") {
            method = Matrix::matrixMultMKL;
        } else {
            std::cerr << "Wrong convolMethod given." << std::endl;
            return -1;
        }
        Matrix result = input.convolution(kernel, doPadding, method, atoi(argv[8]));
        result.toOStream(std::cout);
    } else if (functionName == "pooling") {
        if (argc < 8) {
            std::cerr << "Number of args incorrect." << std::endl;
            return -1;
        }
        Matrix input = Matrix(argv[2], atoi(argv[3]));
        Matrix::poolingMethod method;
        if (std::string(argv[4]) == "maxPooling") {
            method = Matrix::maxPooling;
        } else if (std::string(argv[4]) == "avgPooling") {
            method = Matrix::avgPooling;
        } else {
            std::cerr << "Wrong poolingMethod given." << std::endl;
            return -1;
        }
        Matrix result = input.pooling(method, atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
        result.toOStream(std::cout);
    } else if (functionName == "nonLinearActivation") {
        if (argc < 5) {
            std::cerr << "Number of args incorrect." << std::endl;
            return -1;
        }
        Matrix input = Matrix(argv[2], atoi(argv[3]));
        Matrix::nonLinearActMethod method;
        if (std::string(argv[4]) == "relu") {
            method = Matrix::relu;
        } else if (std::string(argv[4]) == "tanH") {
            method = Matrix::tanH;
        } else if (std::string(argv[4]) == "softmax") {
            method = Matrix::softmax;
        } else if (std::string(argv[4]) == "sigmoid") {
            method = Matrix::sigmoid;
        } else {
            std::cerr << "Wrong nonLinearActMethod given." << std::endl;
            return -1;
        }
        input.nonLinearActivation(method);
        input.toOStream(std::cout);
    } else if (functionName=="data_store") {
        Matrix::data_store();
    } else {
        std::cerr << "Wrong Function name given." << std::endl;
        return -1;
    }
}