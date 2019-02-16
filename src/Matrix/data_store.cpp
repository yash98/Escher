#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <cmath>
#include "Matrix/Matrix.h"

float meanVal(float arr[], int size){
	float mean = 0.0;
	for(int i=0;i<size;i++)
		mean += arr[i];
	mean /= float(size);
	return mean;
}

float std_deviation(float arr[], int size){
	float mean = meanVal(arr, size);
	float deviation[size];
	for(int i=0;i<size;i++)
		deviation[i] = (arr[i]-mean )*(arr[i] - mean);
	float std_dev = meanVal(deviation, size);
	std_dev = sqrt(std_dev);
	return std_dev;
}

void Matrix::data_store(){
	// first randomly generate kernel and images
	int min_image_size, max_image_size;
	int kernel_size;
	bool doPad;
	int numofThreads;
	std::cin >> min_image_size >> max_image_size >> kernel_size;
	std::cin >> numofThreads;

	int secret_num;
	srand(time(NULL));
	std::vector<std::vector<float>> kernel;
	for(int i=0;i<kernel_size;i++){
		std::vector<float> rowVector;
		rowVector.reserve(kernel_size);
		for(int j=0;j<kernel_size;j++){
			secret_num = rand()%256;
			rowVector.push_back(float(secret_num));
		}
		kernel.push_back(rowVector);	
	}
	Matrix ker = Matrix(kernel);

	std::ofstream myfile;
	myfile.open ("comparison.dat");


	float PthreadTime[10];
	float OpenBLASTime[10];
	float MKLTime[10];

	for(int i=min_image_size; i<=max_image_size;i = i*2){
		myfile << i << " ";
		for(int l=0;l<10;l++){
			std::vector<std::vector<float>> image;
			for(int j=0;j<i;j++){
				std::vector<float> rowInputVector;
				rowInputVector.reserve(i);
				for(int k=0;k<i;k++){
					secret_num = rand()%256;
					rowInputVector.push_back(float(secret_num));
				}
				image.push_back(rowInputVector);
			}
			Matrix img = Matrix(image);

			int start_s = clock();
			Matrix convol = img.convolution(ker,false,Matrix::matrixMultPthread,numofThreads);
			int stop_s = clock();
			double exe_time = (stop_s - start_s)/ double (CLOCKS_PER_SEC) * 1000000;
			PthreadTime[l] = exe_time;
			// myfile << exe_time << " ";

			start_s = clock();
			convol = img.convolution(ker,false,Matrix::matrixMultOpenBLAS,numofThreads);
			stop_s = clock();
			exe_time = (stop_s - start_s) / double (CLOCKS_PER_SEC) * 1000000;
			OpenBLASTime[l] = exe_time; 
			// myfile << exe_time << " ";

			start_s = clock();
			convol = img.convolution(ker,false,Matrix::matrixMultMKL,numofThreads);
			stop_s = clock();
			exe_time = (stop_s - start_s) / double (CLOCKS_PER_SEC) * 1000000;
			MKLTime[l] = exe_time;
			// myfile << exe_time << "\n";
		}
		myfile << meanVal(PthreadTime, 10) << " " << std_deviation(PthreadTime, 10) << " ";
		myfile << meanVal(OpenBLASTime, 10) << " " << std_deviation(OpenBLASTime, 10) << " ";
		myfile << meanVal(MKLTime, 10) << " " << std_deviation(MKLTime, 10) << "\n";

	}
	myfile.close();
}