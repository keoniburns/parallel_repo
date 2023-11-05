#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
// #include <omp>
#include <time.h>

#include <sstream>
#include <vector>

#define THREADS (4)  // default 4 threads

using namespace std;

struct matrix_data {
    int n;  // dimensions of the matrix
    int m;
    vector<vector<double> > matrix;
};

void read_input(matrix_data &A, string filename);
// void matrixPrint(matrix_data &A);
void rowSwap(matrix_data &A, int n, int m);
int forwardStep(matrix_data &A, int &flag);
void substitution(matrix_data &A);
void gauss(matrix_data &A);

int main(int argc, char *argv[]) {
    // int total_itr = 0;
    string filename;
    matrix_data A;
    timespec start, end;
    int threads = THREADS;

    if (argc == 1) {
        cerr << "this cannot be done yet please include data file with ./ex2 and numthreads also" << endl;
        exit(-1);
    } else {
        filename = argv[1];
        // threads = argv[2];
    }

    read_input(A, filename);

    // Ai.n = A.n;
    // Ai.m = A.m;
    // Ai.matrix = A.matrix;

    cout << "A num rows: " << A.n << "\nA num cols: " << A.m << endl;

    cout << "Matrix A: " << endl;
    for (int i = 0; i < A.n; i++) {
        for (int j = 0; j < A.m; j++) {
            cout << A.matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    gauss(A);
    // total_itr = A.n * B.m * A.m;
    // cout << "total number of iterations is: " << total_itr << endl;

    // clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    // multiplication(A, RHS, sol);
    // clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    // double time_taken;
    // time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    // time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    // cout << "time for multiplication: " << time_taken << endl;

    // cout << "Matrix B: " << endl;
    // for (int i = 0; i < B.n; i++) {
    //     for (int j = 0; j < B.m; j++) {
    //         cout << B.matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    // cout << "Matrix C: " << endl;
    // cout << "Dimensions in NxM form: " << C.n << "x" << C.m << endl;
    // for (int i = 0; i < C.n; i++) {
    //     for (int j = 0; j < C.m; j++) {
    //         cout << C.matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    return 0;
}

/**
 * @brief
 *
 * @param A (matrix struct)
 * @param B (matrix struct)
 * @param filename
 */
void read_input(matrix_data &A, string filename) {
    ifstream infile(filename, ios::in);
    if (!infile.is_open()) {
        cerr << "cannot open file: " << filename << endl;
        exit(-1);
    }

    /* grabbing the line that holds the NxM dimensions for A*/
    string line;
    if (!getline(infile, line)) {
        cerr << "can't grab first line" << endl;
    }
    cout << line << endl;

    getline(infile, line);
    istringstream aDims(line);  // delimit each of the strings on the line into a single word (aDims)
    if (!(aDims >> A.n >> A.m)) {
        cerr << "cant grab dims for A" << endl;
    }

    A.matrix.resize(A.n, vector<double>(A.m));  // resizing for given dimensions from .dat file

    // #pragma omp parallel for num_threads(threads)
    for (int i = 0; i < A.n; i++) {
        getline(infile, line);
        istringstream coefficients(line);
        for (int j = 0; j < A.m; j++) {
            coefficients >> A.matrix[i][j];
        }
    }
}

void rowSwap(matrix_data &A, int n, int m) {
    for (int i = 0; i <= A.n; i++) {  // iterate through the rows
        double tmp = A.matrix[n][i];  // tmp to hold initial row val
        A.matrix[n][m] = A.matrix[m][i];
        A.matrix[m][i] = tmp;
    }
}

// this only works for matrixes with dimensions NxN
void gauss(matrix_data &A) {
    int flag;
    forwardStep(A, flag);
    if (flag != 0 && A.matrix[flag][A.m]) {
        cerr << "inconsistent system" << endl;
    } else if (flag != 0) {
        cout << "infinite solutions" << endl;
        return;
    }

    substitution(A);
}

int forwardStep(matrix_data &A, int &flag) {
    double maxVal;
    int maxPos;

    for (int k = 0; k < A.n; k++) {
        maxPos = k;                    // init current max
        maxVal = A.matrix[maxPos][k];  // grab value

#pragma omp parallel for num_threads(THREADS)
        for (int i = k + 1; i < A.n; i++) {  // search rows for the largest val and pivot
            if (abs(A.matrix[i][k]) > maxVal) {
                maxVal = A.matrix[i][k];
            }

            if (!A.matrix[k][maxPos]) {
                return k;
            }

            if (maxPos != k) {
                rowSwap(A, k, maxPos);
            }

            for (int i = k + 1; i < A.n; i++) {
                double reduce = A.matrix[i][k] / A.matrix[k][k];
                for (int j = k + 1; j <= A.n; j++) {
                    A.matrix[i][j] -= (A.matrix[k][j] * reduce);
                }
                A.matrix[i][k] = 0;
            }
        }
    }
    return 0;
}

void substitution(matrix_data &A) {
    vector<double> sol(A.n);

#pragma omp parallel for
    for (int i = A.n - 1; i >= 0; i--) {
        sol[i] = A.matrix[i][A.n];
        for (int j = i + 1; j < A.m; j++) {
            sol[i] -= A.matrix[i][j] * sol[j];
        }
        sol[i] = sol[i] / A.matrix[i][i];
    }
    cout << "solution is: " << endl;
    for (int i = 0; i < A.n; i++) {
        cout << sol[i] << endl;
    }
}