#include "Matrix.h"

#include <fstream>
#include <algorithm>
#include <iostream>
#include <math.h>
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

    Matrix(inputMatrix);
    inputFile.close();
}

Matrix Matrix::convolution(const Matrix& kernel, bool doPadding, Matrix::convolMethod method) {}

Matrix padding(bool returnInSame, int rowPad, int columnPad) {}

Matrix Matrix::nonLinearActivation(Matrix::nonLinearActMethod method, bool returnInSame) {
    if(returnInSame){
        if(method != Matrix::softmax){
            for(int i=0;i<this->matrix.size();i++){
                for(int j=0;j<this->matrix[0].size();j++){
                    if(method == relu)this->matrix[i][j] = ReLu(this->matrix[i][j]);
                    else if(method == tanH)this->matrix[i][j] = TanH(this->matrix[i][j]);
                    else if(method == sigmoid)this->matrix[i][j] = Sigmoid(this->matrix[i][j]);
                }
            }
        }
        else{
            for(int i=0;i<this->matrix.size();i++)
                this->matrix[i] = Softmax(this->matrix[i]);
        }
        return this->matrix;
    }
    else{
        std::vector<std::vector<float> > M = this->matrix;
        if(method != softmax){
            for(int i=0;i<this->matrix.size();i++){
                for(int j=0;j<this->matrix[0].size();j++){
                    if(method == Matrix::relu)M[i][j] = ReLu(this->matrix[i][j]);
                    else if(method == tanH)M[i][j] = TanH(this->matrix[i][j]);
                    else if(method == sigmoid)M[i][j] = Sigmoid(this->matrix[i][j]);
                }
            }
        }
        else{
            for(int i=0;i<this->matrix.size();i++)
                M[i] = Softmax(this->matrix[i]);
        }
        return M;
    }
}

Matrix Matrix::pooling(Matrix::poolingMethod method, bool returnInSame) {}

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


float ReLu(float num){
    return (num > 0) ? num : 0.0;
}

float TanH(float num){
    float n1 = exp(num);
    float n2 = exp(-1*num);
    float ans = (n1 - n2)/(n1 + n2);
    return ans;
}

float Sigmoid(float num){
    return (1/(1+exp(-1*num)));
}

std::vector<float> Softmax(std::vector<float> vect){
    std::vector<float> ans;
    ans.reserve(vect.size());
    float denominator = 0;
    for(int i=0;i<vect.size();i++){
        ans[i] = exp(vect[i]);
        denominator += ans[i];
    }
    for(int i=0;i<ans.size();i++)
        ans[i] /= denominator;
    return ans;
}