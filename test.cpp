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

int main() {
    int h, w, hh, ww;
    // cout << "input capacity of the first matrix\n";
    // cin >> h >> w;

    //Matrix<double> m1 = {(unsigned int)w, (unsigned int) h};

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
    Matrix<double> m1 = {(unsigned int)ww, (unsigned int) hh};
    //cout << "input values\n";
    for (int i = 0; i < hh; i++) {
        for (int j = 0; j < ww; j++) {
            cin >> m1[i][j];
        }
    }

    echo(m1);

    // cout << m1.determinat() << endl;
    // m1 = m1.gaussTransform();
    // m1.echoo();

    cout << "!!! isJoint = " << m1.isJoint() << endl;

    Matrix<string> answer = m1.equation();

    answer.echoo();

    m1 = hilbertMatrixEquation(4);

    answer = m1.equation();

    answer.echoo();

    return 0;
}



// int n, i, j, k;

// double d, s;

//  int main()

// {

// cout << "Poryadok: " << endl;

// cin >> n;

// double **a = new double *[n];

// for (i = 0; i <= n; i++)

// a[i] = new double [n];

// double **a1 = new double *[n];

// for (i = 0; i <= n; i++)

// a1[i] = new double [n];

// double *b = new double [n];

// double *x = new double [n];

// cout << "Vvedite koefficienty i svobodnye chleny " << endl;

// for (i = 1; i <= n; i++)

// {

// for (j = 1; j <= n; j++)

// {

// cout << "a[ " << i << "," << j << "]= ";

// cin >> a[i][j];

// a1[i][j] = a[i][j];

// }

// cout << "b,[ " << i << "]= ";

// cin >> b[i];

// }

// for (k = 1; k <= n; k++) // прямой ход

// {

// for (j = k + 1; j <= n; j++)

// {

// d = a[j][k] / a[k][k]; // формула (1)

// for (i = k; i <= n; i++)

// {

// a[j][i] = a[j][i] - d * a[k][i]; // формула (2)

// }

// b[j] = b[j] - d * b[k]; // формула (3)

// }

// }

// for (k = n; k >= 1; k--) // обратный ход

// {

// d = 0;

// for (j = k + 1; j <= n; j++)

// {

// s = a[k][j] * x[j]; // формула (4)

// d = d + s; // формула (4)

// }

// x[k] = (b[k] - d) / a[k][k]; // формула (4)

// }

// cout << "Korni sistemy: " << endl;

// for( i = 1; i <= n; i++)

// cout << "x[" << i << "]=" << x[i] << " " << endl;

// return 0;

// }