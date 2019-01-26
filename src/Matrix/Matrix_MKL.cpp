#include "../../include/Matrix/Matrix.h"

#include <mkl_cblas.h>

void Matrix::mklMatrixMult(float* a, float* b, float* c, 
int m, int n, int numOfThreads) {
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
        m, 1, n, 
        1.0, a, n, b, 1, 0.0, c, 1);
}