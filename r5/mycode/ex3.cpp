#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
    int n, m;  // dimensions of the matrix
    vector<vector<double>> matrix;
};

void multiplication(matrix_data A, matrix_data B, matrix_data &C, int threads);
void read_input(matrix_data &A, matrix_data &B, string filename);

int main(int argc, char *argv[]) {
    // int total_itr = 0;
    string filename;
    matrix_data A;
    matrix_data B;
    matrix_data C;
    timespec start, end;
    int threads = THREADS;

    if (argc == 1) {
        cout << "enter num threads " << endl;
        cin >> threads;
        cout << "enter the dimensions for A in the format <n> <m>" << endl;
        cin >> A.n >> A.m;
        A.matrix.resize(A.n, vector<double>(A.m));

        cout << "enter one element at a time" << endl;
        for (int i = 0; i < A.n; i++) {
            for (int j = 0; j < A.m; j++) {
                cout << "element " << i << " " << j << ": ";
                cin >> A.matrix[i][j];
            }
        }

        cout << "enter the dimensions for A in the format <n> <m>" << endl;
        cin >> B.n >> B.m;

        if (A.m != B.n) {
            cerr << "the cols for A must be the same number as rows for B" << endl;
            exit(-1);
        }
        cout << "enter one element at a time" << endl;
        B.matrix.resize(B.n, vector<double>(B.m));
        for (int i = 0; i < B.n; i++) {
            for (int j = 0; j < B.m; j++) {
                cout << "element " << i << " " << j << ": ";
                cin >> B.matrix[i][j];
            }
        }
    } else {
        filename = argv[1];
        threads = atoi(argv[2]);
        read_input(A, B, filename);
    }

    // cout << "A num cols: " << A.m << "\nB num rows: " << B.n << endl;
    if (A.m != B.n) {
        cerr << "this math isn't mathing.\nA must have the same number of columns as B does rows" << endl;
        exit(-1);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    multiplication(A, B, C, threads);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    cout << "time for multiplication: " << time_taken << endl;

    // cout << "Matrix A: " << endl;
    // for (int i = 0; i < A.n; i++) {
    //     for (int j = 0; j < A.m; j++) {
    //         cout << A.matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

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
void read_input(matrix_data &A, matrix_data &B, string filename) {
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

    istringstream aDims(line);  // delimit each of the strings on the line into a single word (aDims)
    if (!(aDims >> A.n >> A.m)) {
        cerr << "cant grab dims for A" << endl;
    }

    A.matrix.resize(A.n, vector<double>(A.m));  // resizing for given dimensions from .dat file

    /* grabbing the line that holds the NxM dimensions for B */
    if (!getline(infile, line)) {
        cerr << "can't grab second line" << endl;
    }

    istringstream bDims(line);
    if (!(bDims >> B.n >> B.m)) {
        cerr << "cant grab dims for B" << endl;
    }

    B.matrix.resize(B.n, vector<double>(B.m));

    // #pragma omp parallel for num_threads(threads)
    for (int i = 0; i < A.n; i++) {
        getline(infile, line);
        istringstream coefficients(line);
        for (int j = 0; j < A.m; j++) {
            coefficients >> A.matrix[i][j];
        }
    }

    // #pragma omp parallel for num_threads(threads)
    for (int i = 0; i < B.n; i++) {
        getline(infile, line);
        istringstream coefficients(line);
        for (int j = 0; j < B.m; j++) {
            coefficients >> B.matrix[i][j];
        }
    }
}

void multiplication(matrix_data A, matrix_data B, matrix_data &C, int threads) {
    // the product's dimensions can be seen as the corresponding n & m from the matrices being multiplied
    C.n = A.n;
    C.m = B.m;
    C.matrix.resize(C.n, vector<double>(C.m));

#pragma omp parallel for num_threads(threads) collapse(3)
    for (int i = 0; i < A.n; i++) {
        for (int j = 0; j < B.m; j++) {
            for (int k = 0; k < A.m; k++) {
                C.matrix[i][j] += A.matrix[i][k] * B.matrix[k][j];
            }
        }
    }
}
