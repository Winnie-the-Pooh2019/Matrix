#pragma once

#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cmath>

typedef unsigned int uint;

using namespace std;

template<typename T>
struct Iterator {
public:
    T* data;
    uint capacity;

    Iterator(uint capacity) {
        this->capacity = capacity;
        data = new T[capacity];
    }

    Iterator() {
        capacity = 0;
        data = nullptr;
    }

    T& operator [] (const uint& index) {
        return data[index];
    }
};

template<typename T>
struct Matrix {
public:
    Iterator<T>* values;
    uint width;
    uint height;
    
    Matrix(uint width, uint height) {
        this->width = width;
        this->height = height;

        values = new Iterator<T>[height];

        for (uint i = 0; i < height; i++) {
            values[i] = {width};
        }
    }

    Matrix() {
        width = 0;
        height = 0;
        values = nullptr;
    }

    Matrix(const Matrix<T>& original) {
        this->height = original.height;
        this->width = original.width;

        values = new Iterator<T>[height];
        for (uint i = 0; i < height; i++) {
            values[i] = {width};

            for (uint j = 0; j < width; j++) {
                values[i][j] = original.values[i][j];
            }
        }
    }

    ~Matrix() {
        if (values != nullptr) {
            for (uint i = 0; i < height; i++) {
                delete[] values[i].data;
            }

            delete[] values;
        }

        values = nullptr;
    }

    Matrix<T>& operator = (const Matrix<T>& source) {
        if (this == &source)
            return *this;

        height = source.height;
        width = source.width;

        if (values != nullptr)
            free(values);

        values = nullptr;

        if (height != 0 && width != 0) {
            values = new Iterator<T>[height];

            for (uint i = 0; i < height; i++) {
                values[i] = {width};

                for (uint j = 0; j < width; j++) {
                    values[i][j] = source.values[i][j];
                }
            }
        }

        return *this;
    }

    Matrix<T>& operator - () {
        for (uint i = 0; i < height; i++) {
            for (uint j = 0; j < width; j++) {
                values[i][j] = -values[i][j];
            }
        }

        return *this;
    }

    Matrix<T>& operator += (Matrix<T>& source) {
        *this = *this + source;
        return *this;
    }

    Matrix<T>& operator -= (Matrix<T>& source) {
        *this = *this - source;
        return *this;
    }

    Matrix<T>& operator *= (Matrix<T>& source) {
        *this = *this * source;
        return *this;
    }

    Matrix<T>& operator *= (const T& source) {
        *this = source * *this;
        return *this;
    }

    Iterator<T>& operator [] (const uint& hindex) const {
        return values[hindex];
    }

    static const Matrix<T>& fromArray(T** pointer, uint height, uint width) {
        Matrix<T> result = {width, height};

        if (pointer == nullptr)
            return result;

        for (uint i = 0; i < height; i++) {
            for (uint j = 0; j < width; j++) {
                result[i][j] = pointer[i][j];
            }
        }

        free(pointer);

        return result;
    }

    T minor(uint currentH, uint currentW, uint mainH, uint mainW) {
        if (currentH >= height || currentW >= width || mainH >= height || mainW >= width)
            return T();

        return values[mainH][mainW] * values[currentH][currentW] - values[currentH][mainW] * values[mainH][currentW];
    }

    T determinat() {
        Matrix<T> changed = gaussTransform();
        return changed[height - 1][height - 1];
    }

    Matrix<string> equation() {
        Matrix<T> changed = gaussTransform();
        changed.zip();

        if (!changed.isJoint()) {
            cout << "!!! NOT JOINT !!!\n";
            return {};
        }

        Matrix<string> answers = {1, changed.width - 1};

        for (uint i = 0; i < changed.width - 1; i++)
            answers[i][0] = "-";
        
        for (uint i = 0; i < changed.width - 1; i++) {
            cout << "\n!!! NOW I = " << i << endl; 

            int first = -1;

            if (i < changed.height) {
                for (uint j = i; j < changed.width - 1; j++) {
                    if (first == -1 && changed[i][j] == 0 && answers[j][0] == "-") {
                        answers[j][0] = "x" + to_string(j + 1);
                    }
                    if (changed[i][j] != 0 && first == -1 && answers[j][0] == "-") {
                        first = j;

                        answers[j][0] = (changed[i][width - 1] != 0) 
                            ? to_string(changed[i][width - 1] / changed[i][j]) + " "
                            : "";

                        for (uint k = j + 1; k < changed.width - 1; k++) {
                            if (changed[i][k] != 0) {
                                T pc = changed[i][k] / changed[i][j];

                                answers[j][0] += (pc > 0) ? " - " : " + ";
                                answers[j][0] +=  to_string(fabs(pc));
                                answers[j][0] += " * x" + to_string(k + 1) + " ";
                            }
                        }

                        if (answers[j][0] == "")
                            answers[j][0] = "0";
                    }

                    cout << "!!! ANSWERS = " << answers[j][0] << endl;
                }
            }
            else {
                if (answers[i][0] == "-")
                    answers[i][0] = "x" + to_string(i + 1);
            }
        }

        return answers;
    }

    Matrix<T> negativeMatrix() {
        if (determinat() == 0)
            return {};

        Matrix<T> result = *this;
        result.resizeW(height);

        for (uint i = 0; i < result.height; i++) {
            for (uint j = result.height; j < result.width; j++) {
                if (j - result.height == i)
                    result[i][j] = 1;
                else
                    result[i][j] = 0;
            }
        }

        result = result.gaussTransform();

        Matrix<T> r = {result.height, result.height};

        for (uint i = 0; i < result.height; i++) {
            for (uint j = 0; j < result.height; j++) {
                r[i][j] = result[i][j + height] / result[i][i];
            }
        }

        return r;
    }

    Matrix<T> gaussTransform() {
        Matrix<T> result = *this;

        T det = 1;

        cout << "!!! det = " << det << endl;

        uint j = 0;
        for (uint i = 0; i < result.height; i++) {
            


            if (result[i][j] == 0) {
                uint jj = j;

                while (result[i][jj] == 0) {
                    int goodRow = result.findNotNullRow(i, jj);

                    cout << "!!! column = " << jj << endl;
                    
                    cout << "!!! godrow = " << goodRow << endl;

                    if (goodRow != -1) {
                        result.switcherH(i, (uint) goodRow);
                        j = jj;

                        break;
                    }
                    else {
                        if (jj < result.width - 2)
                            jj++;
                        else
                            break;
                    }
                }

                j = jj;
            }

            result.echoo();
            cout << "i = " << i << " j = " << j << endl;

            Matrix<T> buff = result;

            cout << "current element = " << buff[i][j] << endl;

            if (result[i][j] != 0) {
                for (uint h = 0; h < result.height; h++) {
                    cout << "   h = " << h << endl;

                    if (h != i) {
                        T current;

                        for (uint w = 0; w < result.width; w++) {
                            cout << "       w = " << w << endl;

                            current = result[i][j] * result[h][w];
                            current -= result[h][j] * result[i][w];

                            buff[h][w] = current / det;

                            cout << "           new one = " << buff[h][w] << endl;
                        }
                    }
                }
            }

            result = buff;
            det = result[i][j];

            j = (j < result.width - 2) ? j + 1 : j;
        }

        return result;
    }

    bool isJoint() {
        bool result = true;

        for (uint i = 0; i < height; i++) {
            cout << "!!! i before = " << i << endl;
            bool isRight = values[i][width - 1] != 0;
            bool isLeft = false;

            for (uint j = 0; j < width - 1; j++) {
                if (values[i][j] != 0)
                    isLeft = values[i][j] != 0;
            }

            cout << "!!! !isLeft && isRight = " << (!isLeft && isRight) << endl;
            cout << "!!! last = " << (values[i][width - 1] != 0) << endl;
            echoo();
            cout << "!!! i after = " << i << endl;
            if (!isLeft && isRight)
                return false;
            else
                result = true;
        }

        return result;
    }

    void zip() {
        cout << "!!! in zip\n";


        for (uint i = 0; i < height; i++) {

            bool notEmpty = false;

            for (uint j = 0; j < width; j++) {
                if (values[i][j] == -0) {
                    cout << "!!! ZERO INDEXES : " << i << " " << j << endl;
                }
                if (values[i][j] != 0 && values[i][j] != -0)
                    notEmpty = true;
            }

            if (!notEmpty) {
                cout << "!!! EMPTY\n"; 
                switcherH(i, height - 1);
                resizeH(-1);
            }
        }
    }

    void echoo() {
        for (uint i = 0; i < height; i++) {
            for (uint j = 0; j < width; j++) {
                cout << " " << values[i][j];
            }

            cout << endl;
        }
    }

    bool resize(uint deltaH, uint deltaW) {
        return resizeH(deltaH) && resizeW(deltaW);
    }

    bool resizeH(int delta) {
        if (delta + height <= 0 || delta == 0)
            return false;

        Iterator<T>* newP = new Iterator<T>[height + delta];

        for (uint i = 0; i < (delta > 0 ? height : height + delta); i++) {
            newP[i] = {width};

            for (int j = 0; j < width; j++) {
                newP[i][j] = values[i][j];
            }
        }

        for (uint i = height; i < (delta > 0 ? height + delta : height); i++)
            newP[i] = {width};

        height += delta;
        free(values);
        values = newP;

        return true;
    }

    bool resizeW(int delta) {
        if (delta + width <= 0 || delta == 0)
            return false;

        Iterator<T>* newP = new Iterator<T>[height];

        for (uint i = 0; i < height; i++) {
            newP[i] = {width + delta};

            for (uint j = 0; j < (delta > 0 ? width : delta + width); j++)
                newP[i][j] = values[i][j];
        }

        free(values);
        values = newP;
        width += delta;

        return true;
    }

    int findNotNullRow(uint h, uint w) {
        for (uint i = h + 1; i < height; i++) {
            if (values[i][w] != 0)
                return i;
        }

        return -1;
    }

    void switcherH(uint row1, uint row2) {
        cout << "!!! in switcherH !!!" << endl;
        T temp;

        for (uint i = 0; i < width; i++) {
            temp = values[row1][i];
            values[row1][i] = values[row2][i];
            values[row2][i] = temp;
        }
    }

    void switcherW(uint column1, uint column2) {
        T temp;

        for (uint i = 0; i < height; i++) {
            temp = values[i][column1];
            values[i][column1] = values[i][column2];
            values[i][column2] = temp;
        }
    }

    void shiftRightWidth(uint nullColumn) {
        for (uint w = nullColumn + 1; w < width; w++) {
            for (uint h = 0; h < height; h++) {
                values[h][w - 1] = values[h][w];
            }
        }
    }
};

template<typename T>
Matrix<T> operator + (Matrix<T>& left, Matrix<T>& right) {
    Matrix<T> result = {};

    if (left.height != right.height || left.width != right.width) {
        return  result;
    }

    result = {right.width, right.height};

    for (uint i = 0; i < right.height; i++) {
        
        for (uint j = 0; j < right.width; j++) {
            result[i][j] = left[i][j] + right[i][j];
        }
    }
    return result;
}

template<typename T>
Matrix<T> operator - (Matrix<T>& left, Matrix<T>& right) {
    return left + (-right);
}

template<typename T>
Matrix<T> operator * (T&& item, Matrix<T>& matrix) {
    Matrix<T> result = {matrix.width, matrix.height};
    for (uint i = 0; i < result.height; i++) {
        for (uint j = 0; j < result.width; j++) {
            result[i][j] = item * matrix[i][j];
        }
    }
    return result;
}

template<typename T>
Matrix<T> operator * (const T& item, Matrix<T>& matrix) {
    Matrix<T> result = {matrix.width, matrix.height};
    for (uint i = 0; i < result.height; i++) {
        for (uint j = 0; j < result.width; j++) {
            result[i][j] = item * matrix[i][j];
        }
    }

    return result;
}

template<typename T>
Matrix<T> operator * (Matrix<T>& left, Matrix<T>& right) {
    if (left.width != right.height)
        return {};

    Matrix<T> result {right.width, left.height};
    for (uint i = 0; i < result.height; i++) {
        for (uint j = 0; j < result.width; j++) {
            result[i][j] = left[i][0] * right[0][j];

            for (uint k = 1; k < right.height; k++) {
                result[i][j] += left[i][k] * right[k][j];
            }
        }
    }

    return result;
}