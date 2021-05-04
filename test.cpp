#include "Matrix.h"
#include <iostream>

using namespace std;

inline void echo(Matrix<double>& mat) {
    for (int i = 0; i < mat.height; i++) {
        for (int j = 0; j < mat.width; j++) {
            cout << " " << mat[i][j];
        }

        cout << "\n";
    }
}

int main() {
    int h, w, hh, ww;
    // cout << "input capacity of the first matrix\n";
    // cin >> h >> w;

    Matrix<double> m1 = {(unsigned long long int)w, (unsigned long long int) h};

    // cout << "input values\n";
    // for (int i = 0; i < h; i++) {
    //     for (int j = 0; j < w; j++) {
    //         m1[i][j] = j + 1;
    //         cout << " m1[" << i << "][" << j << "] = " << m1[i][j] << " ";
    //     }
    //     cout << "\n";
    // }

    // cout << "input capacity of the second matrix\n";
    // cin >> hh >> ww;

    // Matrix<double> m2 = {(unsigned long long int)ww, (unsigned long long int) hh};

    // cout << "input values\n";
    // for (int i = 0; i < hh; i++) {
    //     for (int j = 0; j < ww; j++) {
    //         m2[i][j] = j + 1;
    //     }
    // }

    // m1 += m2;
    // m1 -= m2;
    // m1 = -m1;

    // m1 *= m2;
    // m1 *= -1.0;


    // cout << "input capacity of the second matrix\n";
    cin >> hh >> ww;
    m1 = {(unsigned long long int)ww, (unsigned long long int) hh};
    //cout << "input values\n";
    for (int i = 0; i < hh; i++) {
        for (int j = 0; j < ww; j++) {
            cin >> m1[i][j];
        }
    }

    //echo(m1);

    cout << m1.determinat() << endl;
    m1 = m1.negativeMatrix();

    echo(m1);

    return 0;
}