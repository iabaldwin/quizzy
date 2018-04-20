//
// Created by Edoardo on 16/04/2018.
//

#include <memory>
#include <vector>
#include <valarray>
#include <cassert>
#include <exception>
#include <iostream>

#include "include/Matrix.h"


Matrix::Matrix() : mRows(1), mCols(1) {
    allocate();
}

Matrix::Matrix(Size rows, Size cols) : mRows(rows), mCols(cols) {
    checkRowIndex(rows - 1);
    checkColIndex(cols - 1);
    allocate();
}

Matrix::Matrix(Size rows, Size cols, const NestedVector &iNestedVec)
        : mRows(rows), mCols(cols) {
    checkRowIndex(rows - 1);
    checkColIndex(cols - 1);
    checkNestedVecSize(iNestedVec);

    allocate();

    std::size_t idx(0);
    for (auto &row : iNestedVec) {
        for (auto &col : row) {
            mData[idx++] = col;
        }
    }
}

void Matrix::allocate() {
    mData = std::valarray<NumericType>(0.0, static_cast<size_t>(mCols * mRows));
}

void Matrix::checkRowIndex(Index row) const {
    if (row >= mRows) {
        throw std::out_of_range("Row out-of-range");
    } else if (row < 0) {
        throw std::out_of_range("Argument can't be lower than 1");
    };
}

void Matrix::checkColIndex(Index col) const {
    if (col >= mCols) {
        throw std::out_of_range("Column out-of-range");
    } else if (col < 0) {
        throw std::out_of_range("Argument can't be lower than 1");
    };
}

void Matrix::checkNestedVecSize(const NestedVector &iNestedVec) {
    if (mRows != iNestedVec.size()) {
        throw std::invalid_argument("Dimension mismatch between "
                                    "nested vector and matrix rows");
    }

    for (const auto &row : iNestedVec) {
        if (mCols != row.size()) {
            throw std::invalid_argument("Dimension mismatch between "
                                        "nested vector and matrix cols");
        }
    }
}

NumericType &Matrix::operator()(Index row, Index col) {
    checkRowIndex(row);
    checkColIndex(col);

    auto idx = col + row * mCols;
    assert(idx < this->getSize());
    return mData[idx];
}

bool Matrix::operator==(const Matrix &iMat) const {
    if (this->mCols != iMat.mCols || this->mRows != iMat.mRows)
        return false;

    std::valarray<bool> res = this->mData == iMat.mData;
    return std::all_of(std::begin(res), std::end(res),
                       [](auto &i) { return i; });
}

bool Matrix::operator!=(const Matrix &iMat) const {
    return !(*this == iMat);
}

std::slice Matrix::getRowSlice(Index row) const {
    checkRowIndex(row);
    return {static_cast<size_t>(mCols * row), static_cast<size_t >(mCols), 1};
}

std::slice Matrix::getColSlice(Index col) const {
    checkColIndex(col);
    return {static_cast<size_t>(col), static_cast<size_t>(mRows),
            static_cast<size_t>(mCols)};
}

std::slice_array<NumericType> Matrix::row(Index row) {
    return mData[this->getRowSlice(row)];
}

std::valarray<NumericType> Matrix::crow(Index row) const {
    return mData[this->getRowSlice(row)];
}

std::slice_array<NumericType> Matrix::col(Index col) {
    return mData[this->getColSlice(col)];
}

std::valarray<NumericType> Matrix::ccol(Index col) const {
    return mData[this->getColSlice(col)];
}

Matrix Matrix::transposed() {
    Matrix t(mCols, mRows);

    for (Index r = 0; r < t.getRowsN(); r++) {
        t.row(r) = this->col(r);
    }

    return t;
}

Matrix Matrix::eye(Size s) {
    Matrix m(s, s);

    for (Index i = 0; i < s; i++) {
        m(i, i) = static_cast<NumericType>(1.0);
    }

    return m;
}

Matrix Matrix::augment(const Matrix &iMatA, const Matrix &iMatB) {

    if (iMatA.getRowsN() != iMatB.getRowsN()) {
        throw std::invalid_argument("Row dimension of input matrices must be equal");
    }

    Matrix ab{iMatA.getRowsN(), iMatA.getColsN() + iMatB.getColsN()};

    for (Index i = 0; i < ab.getRowsN(); i++) {
        auto first = std::slice(i * ab.mCols, iMatA.getColsN(), 1);
        auto second = std::slice(i * ab.mCols + iMatA.getColsN(), iMatB.getColsN(), 1);
        ab.mData[first] = iMatA.crow(i);
        ab.mData[second] = iMatB.crow(i);
    }

    return ab;
}

Matrix Matrix::RREF() {

    auto absCompare = [](const auto &a, const auto &b) {
        return (std::abs(a) < std::abs(b));
    };

    auto argMax = [absCompare](const auto &a, auto start) {
        return std::distance(std::begin(a),
                             std::max_element(std::begin(a) + start, std::end(a),
                                              absCompare));
    };

    Matrix mat(*this);

    int h = 0, k = 0;
    int m = mat.getRowsN();
    int n = mat.getColsN();
    while (h < m && k < n) {
        // Find k-th pivot
        std::valarray<NumericType> currentCol = mat.col(k);
        auto i_max = argMax(currentCol, h);
        if (mat(i_max, k) == 0) {
            k++;
        } else {
            // Swap rows
            std::valarray<NumericType> tmpRow = mat.row(i_max);
            mat.row(i_max) = mat.row(h);
            mat.row(h) = tmpRow;

            // Divide row by pivot
            mat.row(h) = mat.crow(h) / static_cast<NumericType>(mat(h, k));

            // Zero column k by subtracting other rows
            for (int i = 0; i < m; i++) {
                if (i != h) {
                    mat.row(i) = mat.crow(i) -
                                 (static_cast<NumericType>(mat(i, k)) * mat.crow(h));
                }
            }

            h++;
            k++;
        }
    }

    return mat;
}

void Matrix::print() {
    using namespace std;

    cout << endl << endl;
    for (Index i = 0; i < this->getRowsN(); i++) {
        for (Index j = 0; j < this->getColsN(); j++) {
            cout << (*this)(i, j) << ", ";
        }
        cout << endl;
    }
}

Matrix Matrix::dot(const Matrix &iMat1, const Matrix &iMat2) {
    if (iMat1.getColsN() != iMat2.getRowsN()) {
        throw std::invalid_argument("Matrix 1 column number must be equal to Matrix 2 row number");
    }

    Matrix d(iMat1.getRowsN(), iMat2.getColsN());

    for (Index i = 0; i < d.getRowsN(); i++) {
        for (Index j = 0; j < d.getColsN(); j++) {
            auto r = iMat1.crow(i);
            auto c = iMat2.ccol(j);
            d(i, j) = (r * c).sum();
        }
    }

    return d;
}

Matrix Matrix::augment(const Matrix &iMat) {
    return Matrix::augment(*this, iMat);
}

Matrix Matrix::dot(const Matrix & iMat) {
    return Matrix::dot(*this, iMat);
}

Matrix Matrix::inverse() {
    if (mRows != mCols) {
        throw std::domain_error("Matrix must be square");
    }

    auto ai = this->augment(eye(mRows));
    auto aiRref = ai.RREF();

    for (Index i=0; i < aiRref.getRowsN(); i++) {
        if (aiRref(i, i) != 1){
            throw std::domain_error("Singular matrix");
        }
    }

    Matrix res(mRows, mCols);
    for (Index j=0; j < res.getColsN(); j++) {
        res.col(j) = aiRref.ccol(j+mRows);
    }

    return res;
}
