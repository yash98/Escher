#include "DigitRecognition/DigitRecog.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <fstream>
#include <tuple>
#include <algorithm>

Matrix DigitRecog::pngToMatrix(std::string imgFileName) {
    cv::Mat img = cv::imread(imgFileName, cv::IMREAD_GRAYSCALE);
    if ((img.cols != 28) || (img.rows != 28)) {
        cv::Mat tempImg;
        cv::resize(img, tempImg, cv::Size(28,28));
        img = tempImg.reshape(0, std::vector<int>{28, 28});
    } else {
        img = img.reshape(0, std::vector<int>{28, 28});
    }
    // std::cout << img << std::endl;
    std::vector<std::vector<float>> imgVector;
    imgVector.reserve(28);
    for (int i=0; i<28; i++) {
        std::vector<float> rowOfImgVector;
        rowOfImgVector.reserve(28);
        for (int j=0; j<28; j++) {
            float f = (float) img.at<uchar>(i, j);
            rowOfImgVector.push_back((255.0-f)/255.0);
        }
        imgVector.push_back(rowOfImgVector);
    }
    
    Matrix m = Matrix(imgVector);
    return m;
}

std::vector<Matrix> DigitRecog::convLayer(std::vector<Matrix>& inputChannel, 
int squareKernelSide, int numOfFilters, std::string parameterFileName, 
Matrix::convolMethod method, int numThreads) {

    std::vector<Matrix> outputChannel;
    outputChannel.reserve(numOfFilters);

    std::ifstream parameterFile;
    parameterFile.open(parameterFileName, std::ifstream::in);

    // 3d convol = convol and sum together
    for (int i=0; i<numOfFilters; i++) {
        Matrix accumulatorMatrix = Matrix::zeroes(inputChannel[0].matrix.size()-squareKernelSide+1, 
            inputChannel[0].matrix[0].size()-squareKernelSide+1);
        for (int j=0; j<inputChannel.size(); j++) {
            Matrix& eachInputMatrix = inputChannel[j];

            std::vector<std::vector<float>> kernelVec(squareKernelSide, std::vector<float>(squareKernelSide));
            for (int k=0; k<squareKernelSide; k++) {
                for (int l=0; l<squareKernelSide; l++) {
                    std::string line;
                    std::getline(parameterFile, line);
                    // file in row major
                    // reading in column major order to transpose
                    kernelVec[l][k] = stof(line);
                }
            }
            Matrix kernel = Matrix(kernelVec);
            Matrix resultMatrix = eachInputMatrix.convolution(kernel, false, method, numThreads);
            accumulatorMatrix.addMatrixInto(resultMatrix);
        }
        outputChannel.push_back(accumulatorMatrix);
    }

    // Add bias
    for (int i=0; i<numOfFilters; i++) {
        Matrix& toAddBiasToMatrix = outputChannel[i];
        std::string line;
        std::getline(parameterFile, line);
        toAddBiasToMatrix.addConstInto(stof(line));
    }

    parameterFile.close();
    return outputChannel;
}

std::vector<Matrix> DigitRecog::maxPoolLayer(std::vector<Matrix>& inputChannel, int stride, int squarePoolsize) {
    std::vector<Matrix> outputChannel;
    outputChannel.reserve(inputChannel.size());

    for (Matrix eachInputMatrix: inputChannel) {
        outputChannel.push_back(eachInputMatrix.pooling(Matrix::maxPooling, squarePoolsize, squarePoolsize, stride));
    }
    return outputChannel;
}

std::vector<Matrix> DigitRecog::reluLayer(std::vector<Matrix>& inputChannel) {
    std::vector<Matrix> outputChannel;
    outputChannel.reserve(inputChannel.size());

    for (Matrix eachInputMatrix: inputChannel) {
        eachInputMatrix.nonLinearActivation(Matrix::relu);
        // deepcopy the matrix with result
        // nonLinearActivation returns void i.e inplace
        outputChannel.push_back(eachInputMatrix.copy());
    }
    return outputChannel;
}

void DigitRecog::recognizeDigit(std::string imgFileName, std::ostream& toThisOStream, 
std::vector<std::string> parameterFileNames, Matrix::convolMethod convMethod, int numThreads) {
    std::vector<Matrix> channel1;
    std::vector<Matrix> channel2;
    channel1.push_back(pngToMatrix(imgFileName));

    // Conv_1
    channel2 = convLayer(channel1, 5, 20, parameterFileNames[0], convMethod, numThreads);

    // Pool_1
    channel1 = maxPoolLayer(channel2, 2, 2);

    // Conv_2
    channel2 = convLayer(channel1, 5, 50, parameterFileNames[1], convMethod, numThreads);

    // Pool_2
    channel1 = maxPoolLayer(channel2, 2, 2);

    // FC_1
    channel2 = convLayer(channel1, 4, 500, parameterFileNames[2], convMethod, numThreads);

    // Relu
    channel1 = reluLayer(channel2);

    // FC_2
    channel2 = convLayer(channel1, 1, 10, parameterFileNames[3], convMethod, numThreads);

    std::vector<float> finalVector;
    finalVector.reserve(10);
    for (int i=0; i<10; i++) {
        finalVector.push_back(channel2[i].matrix[0][0]);
    }
    std::vector<std::vector<float>> finalVectorInFormat;
    finalVectorInFormat.push_back(finalVector);
    Matrix finalMatrix = Matrix(finalVectorInFormat);
    finalMatrix.nonLinearActivation(Matrix::softmax);

    std::vector<std::tuple<int,float>> probabilityPairVector;
    probabilityPairVector.reserve(10);
    for (int i=0; i<10; i++) {
        probabilityPairVector.push_back(std::make_tuple(i, finalMatrix.matrix[0][i]));
    }

    std::sort(probabilityPairVector.begin(), probabilityPairVector.end(), [](auto const &t1, auto const &t2) {
        return std::get<1>(t1) > std::get<1>(t2);} // descending order sort 
        );

    for (int i=0; i<5; i++) {
        toThisOStream << std::get<0>(probabilityPairVector[i]) << ": " 
        << std::get<1>(probabilityPairVector[i]) << std::endl;
    }
}
