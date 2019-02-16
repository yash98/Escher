#include "DigitRecognition/DigitRecog.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

Matrix DigitRecog::pngToMatrix(std::string filename) {
    cv::Mat img = cv::imread(filename, cv::IMREAD_GRAYSCALE);
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

int main (int agrc, char* argv[]) {
    DigitRecog::pngToMatrix("../1/1.png");
}