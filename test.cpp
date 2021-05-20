#include "Matrix.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

inline void echo(Matrix<double>& mat) {
    for (int i = 0; i < mat.height; i++) {
        for (int j = 0; j < mat.width; j++) {
            cout << " " << mat[i][j];
        }

        cout << "\n";
    }
}

Matrix<double> hilbertMatrix(uint h) {
    Matrix<double> result = {h, h};

    for (uint i = 0; i < h; i++) {
        for (uint j = 0; j < h; j++) {
            result[i][j] = 1.0 / (i + j + 1);
        }
    }

    return result;
}

Matrix<double> hilbertMatrixEquation(uint h) {
    Matrix<double> result = hilbertMatrix(h);

    result.resizeW(1);

    for (uint i = 0; i < result.height; i++) {
        result[i][result.width - 1] = 0;
    }

    return result;
}

void excep(Matrix<double>& equation) {
    Matrix<double> a = equation;
    a.resizeW(-1);
    Matrix<string> sanswers = equation.equation();
    
    sanswers.echoo();

    if (a.determinat() != 0) {
        Matrix<double> answers = {sanswers.width, sanswers.height};

        for (uint i = 0; i < answers.height; i++) {
            answers[i][0] = atof(sanswers[i][0].c_str());
        }

        Matrix<double> b1 = a * answers;
        Matrix<double> b2 = equation;
        b2.switcherW(equation.width - 1, 0);
        b2.resizeW(b2.width - 1);

        double max = 0;
        for (uint i = 0; i < b1.height; i++) {
            if (fabs(b1[i][0] - b2[i][0]) > max)
                max = fabs(b1[i][0] - b2[i][0]);
        }

        cout << "\ninfelicity = " << max << endl;
    }

    cout << "\nanswers = \n";
    sanswers.echoo();
}

int main() {
    srand(time(NULL));
    // first matrix
    cout << "\nMATRIX 1:\n";

    Matrix<double> ex = {4, 3};

    ex[0][0] = 2; ex[0][1] = -1; ex[0][2] = 1; ex[0][3] = 2;
    ex[1][0] = 1; ex[1][1] = -1; ex[1][2] = 1; ex[1][3] = -1;
    ex[2][0] = 1; ex[2][1] = -1; ex[2][2] = 2; ex[2][3] = -5;
    ex.echoo();
    cout << endl;

    // first task
    cout << "\nTASK 1\n";
    excep(ex);

    // second task
    cout << "\nTASK 2\n";
    ex.resizeW(-1);
    double det = ex.determinat();
    cout << "\ndeterminant = " << det << endl;

    // third task
    cout << "\nTASK 3\n";
    ex = ex.negativeMatrix();
    if (det != 0) {
        cout << "\nnegative matrix:\n";
        ex.echoo();
    }
    else {
        cout << "\nnegative matrix doesn't exist\n";
    }


    // random generated matrix
    cout << "\nRANDON GENERATED MATRIX:\n";
    ex = {4, 3};
    for (uint i = 0; i < ex.height; i++) {
        for (uint j = 0; j < ex.width; j++) {
            ex[i][j] = (double) (rand() % 10000) / 1000 - 4;
        }
    }
    ex.echoo();

    // task1
    cout << "\nTASK 1\n";
    excep(ex);

    // second task
    cout << "\nTASK 2\n";
    ex.resizeW(-1);
    det = ex.determinat();
    cout << "\ndeterminat = " << det << endl;

    // third task
    cout << "\nTASK 3\n";
    ex = ex.negativeMatrix();
    if (det != 0) {
        cout << "\nnegative matrix:\n";
        ex.echoo();
    }
    else {
        cout << "\nnegative matrix doesn't exist\n";
    }


    // 1-matrix
    cout << "\nIDENTITY MATRIX\n";
    ex = {4, 3};
    ex[0][0] = 1; ex[0][1] = 0; ex[0][2] = 0; ex[0][3] = 2;
    ex[1][0] = 0; ex[1][1] = 1; ex[1][2] = 0; ex[1][3] = -1;
    ex[2][0] = 0; ex[2][1] = 0; ex[2][2] = 1; ex[2][3] = -5;
    ex.echoo();

    // first task
    cout << "\nTASK 1\n";
    excep(ex);

    // second task
    cout << "\nTASK 2\n";
    ex.resizeW(-1);
    det = ex.determinat();
    cout << "\ndeterminant = " << det << endl;

    // third task
    cout << "\nTASK 3\n";
    ex = ex.negativeMatrix();
    if (det != 0) {
        cout << "\nnegative matrix:\n";
        ex.echoo();
    }
    else {
        cout << "\nnegative matrix doesn't exist\n";
    }


    // forth matrix
    cout << "\nDEGENERATIVE MATRIX\n";
    ex = {4, 3};
    ex[0][0] = 1; ex[0][1] = 1; ex[0][2] = -1; ex[0][3] = 1;
    ex[1][0] = 2; ex[1][1] = -1; ex[1][2] = 5; ex[1][3] = 3;
    ex[2][0] = 3; ex[2][1] = 0; ex[2][2] = 4; ex[2][3] = 4;
    ex.echoo();

    // first task
    cout << "\nTASK 1\n";
    excep(ex);

    // second task
    cout << "\nTASK 2\n";
    ex.resizeW(-1);
    det = ex.determinat();
    cout << "\ndeterminant = " << det << endl;

    // third task
    cout << "\nTASK 3\n";
    ex = ex.negativeMatrix();
    if (det != 0) {
        cout << "\nnegative matrix:\n";
        ex.echoo();
    }
    else {
        cout << "\nnegative matrix doesn't exist\n";
    }


    // Hilbert Matrix
    cout << "\nHILBERT MATRIX\n";
    ex = {4, 3};
    ex = hilbertMatrix(ex.height);
    ex.resizeW(1);
    for (uint i = 0; i < ex.height; i++) {
        ex[i][ex.width - 1] = (double) (rand() % 10000) / 1000 - 4;
    }
    ex.echoo();

    // task 1
    cout << "\nTASK 1\n";
    excep(ex);

    // task 2
    cout << "\nTASK 2\n";
    ex.resizeW(-1);
    det = ex.determinat();
    cout << "\ndeterminat = " << det << endl;

    // third task
    cout << "\nTASK 3\n";
    ex = ex.negativeMatrix();
    if (det != 0) {
        cout << "\nnegative matrix:\n";
        ex.echoo();
    }
    else {
        cout << "\nnegative matrix doesn't exist\n";
    }

    cout << "!!! Do u want to sugest ur task? y/n\n";

    char answer;
    cin >> answer;

    if (answer == 'y') {
        cout << "!!! What type of issue do u wanna do? e/d/n\n";

        cin >> answer;

        uint n, m;
        cout << "Input height and width of matrix\n";
        cin >> n >> m;
        Matrix<double> matrix = {m, n};

        for (uint i = 0; i < n; i++) {
            cout << "Input " << m << " numbers in a row: ";

            for (uint j = 0; j < m; j++) {
                cin >> matrix[i][j];
            }
        }

        if (answer == 'e') {
            excep(matrix);
        }
        else if (answer == 'd') {
            double det = matrix.determinat();
            cout << "Determinant = " << det << endl;
        }
        else if (answer == 'n') {
            matrix = matrix.negativeMatrix();
            matrix.echoo();
        }
        else {
            cout << "incorrect answer\n";
        }
    }

    return 0;
}