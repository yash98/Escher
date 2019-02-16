#ifndef DIGITRECOG_H
#define DIGITRECOG_H

#include "Matrix/Matrix.h"

#include <string>
#include <vector>
#include <ostream>

namespace DigitRecog {
    Matrix pngToMatrix(std::string filename);
    void recognizeDigit(std::string filename, std::ostream& toThisOStream);
};

#endif /* DIGITRECOG_H */