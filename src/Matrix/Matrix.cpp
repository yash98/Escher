#include "Matrix.h"
#include "Util.h"

#include <fstream>
#include <algorithm>
#include <iostream>


Matrix::Matrix() {}

Matrix::Matrix(std::vector<std::vector<float>> inputMatrix) {
    matrix = inputMatrix;
}

Matrix::Matrix(std::string fileName, int rowNumber) {
    std::ifstream inputFile;
    
    inputFile.open(fileName, std::ifstream::in);
    // if error in opening file, state reason and throw exception to stop program execution
    if (inputFile.fail()) {
        perror(fileName.c_str());
        inputFile.close();
        inputFile.exceptions(std::ifstream::failbit);
    }

    std::vector<std::vector<float>> inputMatrix;
    for (int i=0; i<rowNumber; i++) {
        std::vector<float> rowVector;
        // minor reserve optimization, assumed rowNumber = columnNumber
        rowVector.reserve(rowNumber);
        inputMatrix.push_back(rowVector);
    }

    // assumption: each line = column, beacause column major order, next column by '/n'
    std::string line;

    while (std::getline(inputFile, line)) {
        // delimiter assumed as ' '
        Matrix::splitColumnMajorAndPushBackRowMajor(line, ' ', inputMatrix);
    }

    matrix = inputMatrix;
    inputFile.close();
}

void Matrix::toOStream(std::ostream& toThisOStream) {
    for (std::vector<float> eachRow: matrix) {
        for (float eachCell: eachRow) {
            toThisOStream << eachCell << " ";
        }
        toThisOStream << std::endl;
    }
}

Matrix Matrix::copy() {
    std::vector<std::vector<float>> copyInputMatrix;
    copyInputMatrix.reserve(matrix.size());
    int numOfColumns = matrix[0].size();

    for (std::vector<float> eachRow: matrix) {
        std::vector<float> copyEachRow;
        copyEachRow.reserve(numOfColumns);
        for (float eachCell: eachRow) {
            copyEachRow.push_back(eachCell);
        }
        copyInputMatrix.push_back(copyEachRow);
    }

    return Matrix(copyInputMatrix);
}

Matrix Matrix::convolution(const Matrix& kernel, bool doPadding, Matrix::convolMethod method) {}

void Matrix::padding(int rowPad, int columnPad) {
    // currently only for zero pad
    // add column pad i.e columns to existing rows
    std::vector<float>::iterator it1;
    for (std::vector<float> eachRow: matrix) {
        it1 = eachRow.begin();
        eachRow.insert(it1, columnPad, 0.0);
        it1 = eachRow.end();
        eachRow.insert(it1, columnPad, 0.0);
    }

    // add rowpad i.e more rows to vec of vec
    std::vector<float> zeroVec;
    zeroVec.reserve(matrix[0].size());
    for (int i=0; i<matrix[0].size(); i++) {
        zeroVec.push_back(0.0);
    }

    std::vector<std::vector<float>>::iterator it2 = matrix.begin();
    matrix.insert(it2, columnPad, zeroVec);
    it2 = matrix.end();
    matrix.insert(it2, columnPad, zeroVec);
}

Matrix Matrix::nonLinearActivation(Matrix::nonLinearActMethod method, bool returnInSame) {
    if(returnInSame){
        if(method != Matrix::softmax){
            for(int i=0;i<this->matrix.size();i++){
                for(int j=0;j<this->matrix[0].size();j++){
                    if(method == relu)this->matrix[i][j] = Util::ReLu(this->matrix[i][j]);
                    else if(method == tanH)this->matrix[i][j] = Util::TanH(this->matrix[i][j]);
                    else if(method == sigmoid)this->matrix[i][j] = Util::Sigmoid(this->matrix[i][j]);
                }
            }
        }
        else{
            for(int i=0;i<this->matrix.size();i++)
                this->matrix[i] = Util::Softmax(this->matrix[i]);
        }
        return this->matrix;
    }
    else{
        std::vector<std::vector<float> > M = this->matrix;
        if(method != softmax){
            for(int i=0;i<this->matrix.size();i++){
                for(int j=0;j<this->matrix[0].size();j++){
                    if(method == Matrix::relu)M[i][j] = Util::ReLu(this->matrix[i][j]);
                    else if(method == tanH)M[i][j] = Util::TanH(this->matrix[i][j]);
                    else if(method == sigmoid)M[i][j] = Util::Sigmoid(this->matrix[i][j]);
                }
            }
        }
        else{
            for(int i=0;i<this->matrix.size();i++)
                M[i] = Util::Softmax(this->matrix[i]);
        }
        return M;
    }
}

Matrix Matrix::pooling(Matrix::poolingMethod method, bool returnInSame) {}

void Matrix::toOStream(std::ostream& toThisOStream) {
    for (std::vector<float> eachRow: matrix) {
        for (float eachCell: eachRow) {
            toThisOStream << eachCell << " ";
        }
        toThisOStream << std::endl;
    }
}

void Matrix::splitColumnMajorAndPushBackRowMajor(std::string const& original, char separator, 
    std::vector<std::vector<float>>& putInMatrix) {

    std::string::const_iterator start = original.begin();
    std::string::const_iterator end = original.end();
    std::string::const_iterator next = std::find(start, end, separator);
    int counter = 0;
    // need to use .at() instead of [] because only .at() throws out_of_range. other throw seg fault
    try {
        while ( next != end ) {
            putInMatrix.at(counter).push_back(stof(std::string(start, next)));
            start = next + 1;
            next = std::find(start, end, separator);
            counter++;
        }
        putInMatrix.at(counter).push_back(stof(std::string(start, next)));
    } catch (const std::out_of_range& e) {
        std::cerr << "rowNumber stated:" << putInMatrix.size() << std::endl;
        throw std::out_of_range("rowNumber is file and command line arg mismatch");
    }
}
