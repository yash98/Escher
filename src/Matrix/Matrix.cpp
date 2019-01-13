#include "../include/Matrix/Matrix.h"
#include "../include/Matrix/Util.h"

#include <cblas.h>

#include <fstream>
#include <algorithm>
#include <iostream>
#include <cmath>

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
    checkConsistency();
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

void Matrix::checkConsistency() {
    int rowSize = matrix[0].size();
    for (std::vector<float> eachRow: matrix) {
        if (rowSize != eachRow.size()) {
            throw std::invalid_argument("Inconsistent matrix.");
        }
    }
}

Matrix Matrix::convolution(const Matrix& kernel, bool doPadding, Matrix::convolMethod method) {
    if (doPadding) {
        this->padding(kernel.matrix.size(), kernel.matrix[0].size());
    }
    
    if (method == simpleConvol) {
        Matrix resultMatrix = Matrix();
        float sumOfProducts;

        for (int x=0; x<matrix.size()-kernel.matrix.size()+1; x++) {
            std::vector<float> resultRow;
            for (int y=0; y<matrix[0].size()-kernel.matrix[0].size()+1; y++) {
                sumOfProducts = 0.0;

                for (int i=kernel.matrix.size()-1+x; i>=x; i--) {
                    for (int j=kernel.matrix[0].size()-1+y; j>=y; j--) {

                        sumOfProducts += 
                        matrix[i][j]*kernel.matrix[x+kernel.matrix.size()-1-i][y+kernel.matrix[0].size()-1-j];
                    
                    }
                }

                resultRow.push_back(sumOfProducts);
            }
            resultMatrix.matrix.push_back(resultRow);
        }

        return resultMatrix;


    } else if (method == matrixMultBLAS) {
        int m = (matrix.size()-kernel.matrix.size()+1)*(matrix[0].size()-kernel.matrix[0].size()+1);
        int n = kernel.matrix.size()*kernel.matrix[0].size();
        int k = 1;

        float processedInput[m][n];
        int count = 0;

        for (int x=0; x<matrix.size()-kernel.matrix.size()+1; x++) {
            for (int y=0; y<matrix[0].size()-kernel.matrix[0].size()+1; y++) {
                for (int i=x; i<=kernel.matrix.size()-1+x; i++) {
                    for (int j=y; j<=kernel.matrix[0].size()-1+y; j++) {

                        processedInput[count][i*kernel.matrix[0].size()+j]=matrix[i][j];
                    
                    }
                }
                count++;
            }
        }

        // not column vector here for ease of coding
        float processedKernel[n][k];
        count = 0;
        for (int i=kernel.matrix.size()-1; i>=0; i--) {
            for (int j=kernel.matrix[0].size()-1; j>=0; j--) {
                processedKernel[count][0] = kernel.matrix[i][j];
                count++;
            }
        }

        // multiply processedInput to kernel
        float resultArray [m][k];

        const float* a = &processedInput[0][0];
        const float* b = &processedKernel[0][0];
        float* c = &resultArray[0][0];

        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
            m, n, k, 
            1.0, a, k, b, n, 0.0, c, n);

        Matrix resultMatrix;

        count = 0;
        for (int x=0; x<matrix.size()-kernel.matrix.size()+1; x++) {
            std::vector<float> resultRow;
            for (int y=0; y<matrix[0].size()-kernel.matrix[0].size()+1; y++) {
                resultRow.push_back(resultArray[count][0]);
                count++;
            }
            resultMatrix.matrix.push_back(resultRow);
        }

        return resultMatrix;
    }
}

void Matrix::padding(int rowPad, int columnPad) {
    // TODO: could cause error due to shallow copy, dont think will cause error yet
    // special padding to maintain size
    // currently only for zero value pad
    // add column pad i.e columns-1/2 to existing rows
    float rowPadFloat = float(rowPad);
    float columnPadFloat = float(columnPad);
    std::vector<float>::iterator it1;
    for (std::vector<float>& eachRow: matrix) {
        it1 = eachRow.begin();
        eachRow.insert(it1, std::floor((columnPadFloat-1)/2), 0.0);
        for (int i=0; i < std::ceil((columnPadFloat-1)/2); i++) {
            eachRow.push_back(0.0);
        }
    }

    // add rowpad i.e more rows to vec of vec
    std::vector<float> zeroVec;
    zeroVec.reserve(matrix[0].size());
    for (int i=0; i<matrix[0].size(); i++) {
        zeroVec.push_back(0.0);
    }

    std::vector<std::vector<float>>::iterator it2 = matrix.begin();
    matrix.insert(it2, std::floor((rowPadFloat-1)/2), zeroVec);
    for (int i = 0; i < std::ceil((rowPadFloat-1)/2); i++) {
        matrix.push_back(zeroVec);
    }
}

void Matrix::nonLinearActivation(Matrix::nonLinearActMethod method) {
    if(method != softmax){
        for(int i=0;i<this->matrix.size();i++){
            for(int j=0;j<this->matrix[0].size();j++){
                if(method == relu)this->matrix[i][j] = Util::ReLu(this->matrix[i][j]);
                else if(method == tanH)this->matrix[i][j] = Util::TanH(this->matrix[i][j]);
                else if(method == sigmoid)this->matrix[i][j] = Util::Sigmoid(this->matrix[i][j]);
            }
        }
    }
    else if(method == softmax){
        Util::Softmax(this->matrix);
    }
}


Matrix Matrix::pooling(Matrix::poolingMethod method, int num_rows, int num_columns) {
    int m = this->matrix.size();
    int n = this->matrix[0].size();
    std::vector<std::vector<float> > M;
    M.reserve(m-num_rows+1);
    for(int i=0;i<m-num_rows+1;i++){
        std::vector<float> rowOfM;
        for(int j=0;j<n-num_columns+1;j++){
            rowOfM.reserve(n-num_columns+1);
            if(method == maxPooling){
                float ret = std::numeric_limits<float>::min();
                for(int k=0;k<num_rows;k++){
                    for(int l=0;l<num_columns;l++){
                        if(ret < this->matrix[i+k][j+l])
                            ret = this->matrix[i+k][j+l];
                    }
                }
                rowOfM.push_back(ret);
            }
            else if (method == avgPooling) {
                float ret = 0.0;
                for(int k=0;k<num_rows;k++){
                    for(int l=0;l<num_columns;l++)
                        ret += this->matrix[i+k][j+l];
                }
                rowOfM.push_back(ret/(float)(num_rows*num_columns));
            }
        }
        M.push_back(rowOfM);
    }
    Matrix ans = Matrix(M);
    return ans;
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

int main (int argc, char* argv[]) {
    //  ./yourcode.out convolution_withpadding_matrixmult padsize matrix1.txt matrix1_numrows matrix2.txt matrix2_numrows
    if (argc<2) {
        std::cerr << "No args provided. Check README.md for usage." << std::endl;
        return -1;
    }
    std::string functionName = argv[1];
    if (functionName == "convolution") {
        if (argc < 8) {
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
        } else if (std::string(argv[7]) == "matrixMult") {
            method = Matrix::matrixMult;
        } else {
            std::cerr << "Wrong convolMetod given." << std::endl;
            return -1;
        }
        Matrix result = input.convolution(kernel, doPadding, method);
        result.toOStream(std::cout);
    } else if (functionName == "pooling") {
        if (argc < 7) {
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
        Matrix result = input.pooling(method, atoi(argv[5]), atoi(argv[6]));
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
    } else {
        std::cerr << "Wrong Function name given." << std::endl;
        return -1;
    }
}