//
// Created by Edoardo on 16/04/2018.
//

#ifndef QUIZZY_MATRIX_H
#define QUIZZY_MATRIX_H

#include <valarray>
#include "gtest/gtest_prod.h"

using NumericType = double;
using Index = int;
using Size = int;
using NestedVector = std::vector<std::vector<NumericType>>;

class Matrix {
public:
    Matrix();
    Matrix(Size, Size);

    Matrix(Size, Size, const NestedVector &);
    ~Matrix() = default;

    NumericType &operator()(Index, Index);
    bool operator==(const Matrix &) const;
    bool operator!=(const Matrix &) const;

    inline Index getRowsN() const { return mRows; };
    inline Index getColsN() const { return mCols; };
    inline Size getSize() const { return mRows * mCols; };
    inline const std::valarray<NumericType> & getData() const { return mData;};

    std::slice_array<NumericType> row(Index);
    std::valarray<NumericType> crow(Index) const;
    std::slice_array<NumericType> col(Index);
    std::valarray<NumericType> ccol(Index) const;

    void print();
    Matrix transposed();
    Matrix augment(const Matrix &);
    Matrix RREF();
    Matrix dot(const Matrix &);
    Matrix inverse();

    static Matrix eye(Size);
    static Matrix augment(const Matrix &, const Matrix &);
    static Matrix dot(const Matrix &, const Matrix &);

private:
    Size mRows, mCols;
    std::valarray<NumericType> mData;

    std::slice getRowSlice(Index) const;
    std::slice getColSlice(Index) const;
    void allocate();

    void checkRowIndex(Index row) const;
    void checkColIndex(Index col) const;
    void checkNestedVecSize(const NestedVector &);

    FRIEND_TEST(MatrixConstructor, Default);
    FRIEND_TEST(MatrixConstructor, DefinedSize);
};


#endif //QUIZZY_MATRIX_H
