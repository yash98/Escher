#include "Matrix/Matrix.h"
#include "Matrix/Util.h"

#include <fstream>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <ctime>
#include <stdlib.h>

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

Matrix Matrix::convolution(const Matrix& kernel, bool doPadding, Matrix::convolMethod method, int numOfThreads) {
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

    } else {
        int m = (matrix.size()-kernel.matrix.size()+1)*(matrix[0].size()-kernel.matrix[0].size()+1);
        int n = kernel.matrix.size()*kernel.matrix[0].size();
        int k = 1;

        float* processedInput = new float[m*n];
        int count = 0;

        for (int x=0; x<matrix.size()-kernel.matrix.size()+1; x++) {
            for (int y=0; y<matrix[0].size()-kernel.matrix[0].size()+1; y++) {
                for (int i=x; i<=kernel.matrix.size()-1+x; i++) {
                    for (int j=y; j<=kernel.matrix[0].size()-1+y; j++) {
                        processedInput[count]=matrix[i][j];
                        count++;
                    }
                }
            }
        }

        float* processedKernel = new float[n];
        count = 0;
        for (int i=kernel.matrix.size()-1; i>=0; i--) {
            for (int j=kernel.matrix[0].size()-1; j>=0; j--) {
                processedKernel[count] = kernel.matrix[i][j];
                count++;
            }
        }

        // multiply processedInput to kernel
        float* resultArray = new float[m]; 

        if (method == Matrix::matrixMultPthread) {
            Util::parallelizedMatrixTransVectorMult(processedInput, processedKernel, 
                resultArray, m, n, numOfThreads);
        } else if (method == Matrix::matrixMultOpenBLAS) {
            openblasMatrixMult(processedInput, processedKernel, resultArray, m, n, numOfThreads);
        } else if (method == Matrix::matrixMultMKL) {
            mklMatrixMult(processedInput, processedKernel, resultArray, m, n, numOfThreads);
        } else {
            std::cerr << "method: " << method << std::endl;
            throw std::invalid_argument("Wrong convolMethod passed to function.");
        }

        Matrix resultMatrix;

        count = 0;
        for (int x=0; x<matrix.size()-kernel.matrix.size()+1; x++) {
            std::vector<float> resultRow;
            for (int y=0; y<matrix[0].size()-kernel.matrix[0].size()+1; y++) {
                resultRow.push_back(resultArray[count]);
                count++;
            }
            resultMatrix.matrix.push_back(resultRow);
        }
        
        delete [] processedInput;
        delete [] processedKernel;
        delete [] resultArray;

        return resultMatrix;
    }
}

void Matrix::padding(int rowPad, int columnPad) {
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


Matrix Matrix::pooling(Matrix::poolingMethod method, int num_rows, int num_columns, int stride) {
    int m = this->matrix.size();
    int n = this->matrix[0].size();
    std::vector<std::vector<float> > M;
    M.reserve(m-num_rows+1);
    for(int i=0;i<m-num_rows+1;i+=stride){
        std::vector<float> rowOfM;
        for(int j=0;j<n-num_columns+1;j+=stride){
            rowOfM.reserve(n-num_columns+1);
            if(method == maxPooling){
                float ret = -std::numeric_limits<float>::max();
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

void Matrix::addMatrixInto(Matrix& other) {
    if ((matrix.size()==other.matrix.size()) && (matrix[0].size()==other.matrix[0].size())) {
        for (int i=0; i<matrix.size(); i++) {
            for (int j=0; j<matrix[0].size(); j++) {
                matrix[i][j] += other.matrix[i][j];
            }
        }
    } else {
        throw std::invalid_argument("addMatrixInto: matrix size mismatch");
    }
}

void Matrix::addConstInto(float givenFloat) {
    for (int i=0; i<matrix.size(); i++) {
        for (int j=0; j<matrix[0].size(); j++) {
            matrix[i][j] += givenFloat;
        }
    }
}

Matrix Matrix::zeroes(int rows, int columns) {
    return Matrix(std::vector<std::vector<float>>(rows, std::vector<float>(columns)));
}

Matrix Matrix::randomMatrix(int rows, int columns) {
    int secret_num;
	srand(time(NULL));
	std::vector<std::vector<float>> vec;
	for(int i=0;i<rows;i++){
		std::vector<float> rowVector;
		rowVector.reserve(rows);
		for(int j=0;j<columns;j++){
			secret_num = rand();
			rowVector.push_back(float(secret_num));
		}
		vec.push_back(rowVector);
	}
	Matrix mat = Matrix(vec);
    return mat;
}