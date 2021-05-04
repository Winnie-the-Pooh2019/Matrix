#pragma once

#include <stdlib.h>
#include <iostream>

typedef unsigned long long int ulli;
typedef long long int loli;

using namespace std;

template<typename T>
struct Iterator {
public:
    T* data;
    ulli capacity;

    Iterator(ulli capacity) {
        this->capacity = capacity;
        data = new T[capacity];
    }

    Iterator() {
        capacity = 0;
        data = nullptr;
    }

    T& operator [] (const ulli& index) {
        return data[index];
    }
};

template<typename T>
struct Matrix {
public:
    Iterator<T>* values;
    ulli width;
    ulli height;
    
    Matrix(ulli width, ulli height) {
        this->width = width;
        this->height = height;

        values = new Iterator<T>[height];

        for (ulli i = 0; i < height; i++) {
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
        for (ulli i = 0; i < height; i++) {
            values[i] = {width};

            for (ulli j = 0; j < width; j++) {
                values[i][j] = original.values[i][j];
            }
        }
    }

    ~Matrix() {
        if (values != nullptr) {
            for (ulli i = 0; i < height; i++) {
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

            for (ulli i = 0; i < height; i++) {
                values[i] = {width};

                for (ulli j = 0; j < width; j++) {
                    values[i][j] = source.values[i][j];
                }
            }
        }

        return *this;
    }

    Matrix<T>& operator - () {
        for (ulli i = 0; i < height; i++) {
            for (ulli j = 0; j < width; j++) {
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

    Iterator<T>& operator [] (const ulli& hindex) const {
        return values[hindex];
    }

    static const Matrix<T>& fromArray(T** pointer, ulli height, ulli width) {
        Matrix<T> result = {width, height};

        if (pointer == nullptr)
            return result;

        for (ulli i = 0; i < height; i++) {
            for (ulli j = 0; j < width; j++) {
                result[i][j] = pointer[i][j];
            }
        }

        free(pointer);

        return result;
    }

    T minor(ulli currentH, ulli currentW, ulli mainH, ulli mainW) {
        if (currentH >= height || currentW >= width || mainH >= height || mainW >= width)
            return T();

        return values[mainH][mainW] * values[currentH][currentW] - values[currentH][mainW] * values[mainH][currentW];
    }

    T determinat() {
        Matrix<T> changed = gaussTransform();
        return changed[height - 1][height - 1];
    }

    Matrix<T> equation() {
        gaussTransform();

        Matrix<T> result = {1, height};

        for (ulli i = 0; i < height; i++)
            result[i][0] = values[i][width - 1] / values[i][i];

        return result;
    }

    Matrix<T> negativeMatrix() {
        if (determinat() == 0)
            return {};

        Matrix<T> result = *this;
        result.resizeW(height);

        for (ulli i = 0; i < result.height; i++) {
            for (ulli j = result.height; j < result.width; j++) {
                if (j - result.height == i)
                    result[i][j] = 1;
                else
                    result[i][j] = 0;
            }
        }

        result = result.gaussTransform();

        Matrix<T> r = {result.height, result.height};

        for (ulli i = 0; i < result.height; i++) {
            for (ulli j = 0; j < result.height; j++) {
                r[i][j] = result[i][j + height] / result[i][i];
            }
        }

        return r;
    }

    Matrix<T> gaussTransform() {
        Matrix<T> result = *this;

        T det = result[0][0] / result[0][0];

        for (ulli i = 0; i < result.height; i++) {
            Matrix<T> buff = result;

            for (ulli h = 0; h < result.height; h++) {
                if (h != i) {
                    T current;

                    for (ulli w = 0; w < result.width; w++) {
                        current = result[i][i] * result[h][w];
                        current -= result[h][i] * result[i][w];

                        buff[h][w] = current / det;
                    }
                }
            }

            result = buff;
            det = result[i][i];
        }

        return result;
    }

    

private:
    void echoo() {
        for (ulli i = 0; i < height; i++) {
            for (ulli j = 0; j < width; j++) {
                cout << " " << values[i][j];
            }

            cout << endl;
        }
    }

    bool resize(ulli deltaH, ulli deltaW) {
        return resizeH(deltaH) && resizeW(deltaW);
    }

    bool resizeH(loli delta) {
        if (delta + height <= 0 || delta == 0)
            return false;

        Iterator<T>* newP = new Iterator<T>[height + delta];

        for (ulli i = 0; i < (delta > 0 ? height : height + delta); i++) {
            newP[i] = {width};

            for (loli j = 0; j < width; j++) {
                newP[i][j] = values[i][j];
            }
        }

        for (ulli i = height; i < (delta > 0 ? height + delta : height); i++)
            newP = {width};

        height += delta;
        free(values);
        values = newP;

        return true;
    }

    bool resizeW(loli delta) {
        if (delta + width <= 0 || delta == 0)
            return false;

        Iterator<T>* newP = new Iterator<T>[height];

        for (ulli i = 0; i < height; i++) {
            newP[i] = {width + delta};

            for (ulli j = 0; j < (delta > 0 ? width : delta + width); j++)
                newP[i][j] = values[i][j];
        }

        free(values);
        values = newP;
        width += delta;

        return true;
    }
};

template<typename T>
Matrix<T> operator + (Matrix<T>& left, Matrix<T>& right) {
    Matrix<T> result = {};

    if (left.height != right.height || left.width != right.width) {
        return  result;
    }

    result = {right.width, right.height};

    for (ulli i = 0; i < right.height; i++) {
        
        for (ulli j = 0; j < right.width; j++) {
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
    for (ulli i = 0; i < result.height; i++) {
        for (ulli j = 0; j < result.width; j++) {
            result[i][j] = item * matrix[i][j];
        }
    }
    return result;
}

template<typename T>
Matrix<T> operator * (const T& item, Matrix<T>& matrix) {
    Matrix<T> result = {matrix.width, matrix.height};
    for (ulli i = 0; i < result.height; i++) {
        for (ulli j = 0; j < result.width; j++) {
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
    for (ulli i = 0; i < result.height; i++) {
        for (ulli j = 0; j < result.width; j++) {
            result[i][j] = left[i][0] * right[0][j];

            for (ulli k = 1; k < right.height; k++) {
                result[i][j] += left[i][k] * right[k][j];
            }
        }
    }

    return result;
}