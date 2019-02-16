#ifndef DIGITRECOG_H
#define DIGITRECOG_H

#include "Matrix/Matrix.h"

#include <string>
#include <vector>
#include <ostream>

namespace DigitRecog {
    Matrix pngToMatrix(std::string filename);
    void recognizeDigit(std::string filename, std::ostream& toThisOStream);
    std::vector<Matrix> convLayer(std::vector<Matrix> inputChannel, 
        int squareKernelSide, int numOfFilters, std::string parameterFileName, 
        Matrix::convolMethod method, int numThreads);
    std::vector<Matrix> maxPoolLayer(std::vector<Matrix> inputChannel, int stride);
    std::vector<Matrix> reluLayer(std::vector<Matrix> inputChannel);
};

#endif /* DIGITRECOG_H */