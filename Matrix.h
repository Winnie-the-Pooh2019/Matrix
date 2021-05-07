#pragma once

#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cmath>

typedef unsigned int uint;

using namespace std;

/**
 * @author @Winnie-the-Pooh2019
 * 
 * email: gregsonjackk@gmail.com
 */

/**
 * subsidiary type contains array
 * of generic data
 * 
 * @tparam T 
 */
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

    /**
     * @brief redefines operator []
     * to simplify data access
     * 
     * @param index 
     * @return T& 
     */
    T& operator [] (const uint& index) {
        return data[index];
    }
};

/**
 * @brief double-dimention array, contains 
 * array of Iterator<T>, its capacity and capacity
 * of each Iterator<T>.
 * 
 * @tparam T 
 */
template<typename T>
struct Matrix {
public:
    Iterator<T>* values;
    uint width;
    uint height;
    
    /**
     * @brief Construct a new Matrix object
     * 
     * @param width 
     * @param height 
     */
    Matrix(uint width, uint height) {
        this->width = width;
        this->height = height;

        values = new Iterator<T>[height];

        for (uint i = 0; i < height; i++) {
            values[i] = {width};
        }
    }

    /**
     * @brief empty construct a new Matrix object.
     * Makes pointer @code nullptr and capacities 0
     */
    Matrix() {
        width = 0;
        height = 0;
        values = nullptr;
    }

    /**
     * @brief Copy construct a Matrix object.
     * Copies object by each element
     * 
     * @param original 
     */
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

    /**
     * @brief Destroy the Matrix object.
     * Clears data and makes pointer @code nullprt
     * 
     */
    ~Matrix() {
        if (values != nullptr) {
            for (uint i = 0; i < height; i++) {
                delete[] values[i].data;
            }

            delete[] values;
        }

        values = nullptr;
    }

    /**
     * @brief Redefines @code operator=.
     * Copies data to lvalue only of all capacity is equal.
     * 
     * @param source 
     * @return Matrix<T>& 
     */
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

    /**
     * @brief redefines @code operator-. Makes every item
     * a negative copy of it.
     * 
     * @return Matrix<T>& 
     */
    Matrix<T>& operator - () {
        for (uint i = 0; i < height; i++) {
            for (uint j = 0; j < width; j++) {
                values[i][j] = -values[i][j];
            }
        }

        return *this;
    }

    /**
     * @brief redefines @code operator+=.
     * assigns the result of addition to @code *this
     * 
     * @param source 
     * @return Matrix<T>& 
     */
    Matrix<T>& operator += (Matrix<T>& source) {
        *this = *this + source;
        return *this;
    }

    /**
     * @brief redefines @code operator-=.
     * assigns the result of substruction to @code *this
     * 
     * @param source 
     * @return Matrix<T>& 
     */
    Matrix<T>& operator -= (Matrix<T>& source) {
        *this = *this - source;
        return *this;
    }

    /**
     * @brief redefines @code operator-=.
     * assign the result of matrix multiplication to @code *this
     * 
     * @param source 
     * @return Matrix<T>& 
     */
    Matrix<T>& operator *= (Matrix<T>& source) {
        *this = *this * source;
        return *this;
    }

    /**
     * @brief redefines @code operator-=.
     * assign the result of multiplication generic item to matrix 
     * to @code *this
     * 
     * @param source 
     * @return Matrix<T>& 
     */
    Matrix<T>& operator *= (const T& source) {
        *this = source * *this;
        return *this;
    }

    /**
     * @brief redefines @code operator[]
     * 
     * @param hindex 
     * @return Iterator<T>& 
     */
    Iterator<T>& operator [] (const uint& hindex) const {
        return values[hindex];
    }

    /**
     * @brief generate a matrix from double-dimention array
     * 
     * @param pointer 
     * @param height 
     * @param width 
     * @return const Matrix<T>& 
     * 
     * @deprecated isn't tested
     */
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

    /**
     * @brief calculates a second-level determinant from this matrix
     * 
     * @param currentH 
     * @param currentW 
     * @param mainH 
     * @param mainW 
     * @return T 
     */
    T minor(uint currentH, uint currentW, uint mainH, uint mainW) {
        if (currentH >= height || currentW >= width || mainH >= height || mainW >= width)
            return T();

        return values[mainH][mainW] * values[currentH][currentW] - values[currentH][mainW] * values[mainH][currentW];
    }

    /**
     * @brief calculates a determinant of this matrix.
     * Firstly checks if it is square. Then applies @code gaussTransform()
     * to this matrix and takes the last element of transformed matrix.
     * 
     * @return T 
     */
    T determinat() {
        if (height != width)
            return 0;

        Matrix<T> changed = gaussTransform();
        return changed[height - 1][height - 1];
    }

    /**
     * @brief It solves equation system. Transforms matrix by @code gaussTransform().
     * Then it creates matrix of answers and put them there. If coefficient matrix isn't
     * square or its determinant is 0, answer-vector contains free-variables. This method
     * will put in answer-vector free-variable. Other variable will contain theese variables in.
     * 
     * @return Matrix<string> 
     */
    Matrix<string> equation() {
        Matrix<T> changed = gaussTransform();
        // changed.echoo();
        changed.zip();

        if (!changed.isJoint()) {
            // cout << "!!! NOT JOINT !!!\n";
            return {};
        }

        Matrix<string> answers = {1, changed.width - 1};

        for (uint i = 0; i < changed.width - 1; i++)
            answers[i][0] = "-";
        
        for (uint i = 0; i < changed.width - 1; i++) {
            // cout << "\n!!! NOW I = " << i << endl; 

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

                    // cout << "!!! ANSWERS = " << answers[j][0] << endl;
                }
            }
            else {
                if (answers[i][0] == "-")
                    answers[i][0] = "x" + to_string(i + 1);
            }
        }

        return answers;
    }

    /**
     * @brief supplies gaussTransform to this matrix. If determinant = 0
     * returns empty matrix. In other way: to this matrix is added a identity matrix.
     * Applies @code gaussTransform(). In source matrix will be identity matrix and 
     * besides identity matrix will be a result
     * 
     * @return Matrix<T> 
     */
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

    /**
     * @brief it applies to source matrix a Bareiss algorithm
     * @link https://en.wikipedia.org/wiki/Bareiss_algorithm @endlink
     * 
     * @return Matrix<T> transformed matrix
     */
    Matrix<T> gaussTransform() {
        Matrix<T> result = *this;

        T det = 1;

        // cout << "!!! det = " << det << endl;

        uint j = 0;
        for (uint i = 0; i < result.height; i++) {
            


            if (result[i][j] == 0) {
                uint jj = j;

                while (result[i][jj] == 0) {
                    int goodRow = result.findNotNullRow(i, jj);

                    // cout << "!!! column = " << jj << endl;
                    
                    // cout << "!!! godrow = " << goodRow << endl;

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

            // result.echoo();
            // cout << "i = " << i << " j = " << j << endl;

            Matrix<T> buff = result;

            // cout << "current element = " << buff[i][j] << endl;

            if (result[i][j] != 0) {
                for (uint h = 0; h < result.height; h++) {
                    // cout << "   h = " << h << endl;

                    if (h != i) {
                        T current;

                        for (uint w = 0; w < result.width; w++) {
                            // cout << "       w = " << w << endl;

                            current = result[i][j] * result[h][w];
                            current -= result[h][j] * result[i][w];

                            buff[h][w] = current / det;

                            // cout << "           new one = " << buff[h][w] << endl;
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

    /**
     * @brief checks if matrix is joint 
     * 
     * @return true 
     * @return false 
     */
    bool isJoint() {
        bool result = true;

        for (uint i = 0; i < height; i++) {
            // cout << "!!! i before = " << i << endl;
            bool isRight = values[i][width - 1] != 0;
            bool isLeft = false;

            for (uint j = 0; j < width - 1; j++) {
                if (values[i][j] != 0)
                    isLeft = values[i][j] != 0;
            }

            // cout << "!!! !isLeft && isRight = " << (!isLeft && isRight) << endl;
            // cout << "!!! last = " << (values[i][width - 1] != 0) << endl;
            // echoo();
            // cout << "!!! i after = " << i << endl;
            if (!isLeft && isRight)
                return false;
            else
                result = true;
        }

        return result;
    }

    /**
     * @brief cuts zero-rows
     */
    void zip() {
        // cout << "!!! in zip\n";


        for (uint i = 0; i < height; i++) {

            bool notEmpty = false;

            for (uint j = 0; j < width; j++) {
                if (values[i][j] == -0) {
                    // cout << "!!! ZERO INDEXES : " << i << " " << j << endl;
                }
                if (values[i][j] != 0 && values[i][j] != -0)
                    notEmpty = true;
            }

            if (!notEmpty) {
                // cout << "!!! EMPTY\n"; 
                switcherH(i, height - 1);
                resizeH(-1);
            }
        }
    }

    /**
     * @brief outputs matrix content
     */
    void echoo() {
        for (uint i = 0; i < height; i++) {
            for (uint j = 0; j < width; j++) {
                cout << " " << values[i][j];
            }

            cout << endl;
        }
    }

    /**
     * @brief do @code resizeH() and @code resizeW() in a row
     * 
     * @param deltaH 
     * @param deltaW 
     * @return true if it is possible
     * @return false if it isn't possible
     */
    bool resize(uint deltaH, uint deltaW) {
        return resizeH(deltaH) && resizeW(deltaW);
    }

    /**
     * @brief increace height and adds new rows to the bottom of matrix
     * 
     * @param delta 
     * @return true if it is possible
     * @return false if it isn't possible
     */
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

    /**
     * @brief increace width and adds new columns to the right side of matrix
     * 
     * @param delta 
     * @return true if it is possible
     * @return false if it isn't possible
     */
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

    /**
     * @brief looks for a row containing not only nulls
     * 
     * @param h 
     * @param w 
     * @return int index of the first not-null row
     * if it doesn't exist returns -1
     */
    int findNotNullRow(uint h, uint w) {
        for (uint i = h + 1; i < height; i++) {
            if (values[i][w] != 0)
                return i;
        }

        return -1;
    }

    /**
     * @brief swaps rows
     * 
     * @param row1 
     * @param row2 
     */
    void switcherH(uint row1, uint row2) {
        // cout << "!!! in switcherH !!!" << endl;
        T temp;

        for (uint i = 0; i < width; i++) {
            temp = values[row1][i];
            values[row1][i] = values[row2][i];
            values[row2][i] = temp;
        }
    }

    /**
     * @brief swaps columns
     * 
     * @param column1 
     * @param column2 
     */
    void switcherW(uint column1, uint column2) {
        T temp;

        for (uint i = 0; i < height; i++) {
            temp = values[i][column1];
            values[i][column1] = values[i][column2];
            values[i][column2] = temp;
        }
    }

    /**
     * @brief shifts columns filling empty space in middle and 
     * frees space in the bottom for simple compression
     * 
     * @param nullColumn 
     */
    void shiftRightWidth(uint nullColumn) {
        for (uint w = nullColumn + 1; w < width; w++) {
            for (uint h = 0; h < height; h++) {
                values[h][w - 1] = values[h][w];
            }
        }
    }
};

/**
 * @brief redefines @code operator+. Adds one matrix to another
 * 
 * @tparam T 
 * @param left 
 * @param right 
 * @return Matrix<T> 
 */
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

/**
 * @brief redefines @code operator-. Substract one matrix from another
 * by using unary @code oprator- and @code operator+
 * 
 * @tparam T 
 * @param left 
 * @param right 
 * @return Matrix<T> 
 */
template<typename T>
Matrix<T> operator - (Matrix<T>& left, Matrix<T>& right) {
    return left + (-right);
}

/**
 * @brief redefines @code oprator* for an item and a matrix.
 * It multiplicates an item to each matrix members
 * 
 * @tparam T 
 * @param item 
 * @param matrix 
 * @return Matrix<T> 
 */
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

/**
 * @brief redefines @code oprator* for an item and a matrix.
 * It multiplicates an item to each matrix members
 * 
 * @tparam T 
 * @param item 
 * @param matrix 
 * @return Matrix<T> 
 */
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

/**
 * @brief redefines @code oprator* for 2 matrixes
 * It multiplicates if only left's columns count is equal to right's rows count
 * 
 * @tparam T 
 * @param left 
 * @param right 
 * @return Matrix<T> 
 */
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