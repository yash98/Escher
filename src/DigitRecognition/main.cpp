#include "DigitRecognition/DigitRecog.h"
#include "Matrix/Matrix.h"

#include <string>
#include <vector>
#include <iostream>

int main (int argc, char* argv[]) {
    std::vector<std::string> parameterFiles;
    parameterFiles.assign(argv+1, argv+argc);
    DigitRecog::recognizeDigit(argv[1], std::cout, parameterFiles, Matrix::matrixMultMKL, 4);
}