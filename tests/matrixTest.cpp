//
// Created by Edoardo on 16/04/2018.
//

#include "gtest/gtest.h"

#include "Matrix.h"


TEST(MatrixConstructor, Default)
{
    Matrix m;

    EXPECT_EQ(1, m.getRowsN());
    EXPECT_EQ(1, m.getColsN());
    EXPECT_EQ(1, m.mData.size());
}

TEST(MatrixConstructor, DefinedSize)
{
    Matrix m(4, 2);

    EXPECT_EQ(4, m.getRowsN());
    EXPECT_EQ(2, m.getColsN());
    EXPECT_EQ(8, m.mData.size());
}

TEST(MatrixConstructor, WrongSizes) {
    EXPECT_THROW(Matrix(0, 0), std::out_of_range);
    EXPECT_THROW(Matrix(1, 0), std::out_of_range);
    EXPECT_THROW(Matrix(0, 1), std::out_of_range);
    EXPECT_THROW(Matrix(-1, 2), std::out_of_range);
    EXPECT_THROW(Matrix(3, -1), std::out_of_range);
    EXPECT_THROW(Matrix(-4, -1), std::out_of_range);
}

TEST(MatrixConstructor, ExistentNestedVec)
{
    NestedVector v{{1,2}, {3, 4}};
    Matrix m(2, 2, v);

    EXPECT_EQ(1, m(0, 0));
    EXPECT_EQ(2, m(0, 1));
    EXPECT_EQ(3, m(1, 0));
    EXPECT_EQ(4, m(1, 1));

    v.at(0).at(0) = 10;
    v.at(0).at(1) = 20;
    v.at(1).at(0) = 30;
    v.at(1).at(1) = 40;

    EXPECT_EQ(1, m(0, 0));
    EXPECT_EQ(2, m(0, 1));
    EXPECT_EQ(3, m(1, 0));
    EXPECT_EQ(4, m(1, 1));
}

TEST(MatrixConstructor, WrongExistentNestedVec)
{
    NestedVector v1{{1,2}, {3, 4}, {5, 6}};
    EXPECT_THROW(Matrix m(2, 2, v1), std::invalid_argument);

    NestedVector v2{{1,2}, {3, 4}, {5}};
    EXPECT_THROW(Matrix m(3, 2, v2), std::invalid_argument);
}

TEST(MatrixConstructor, CopyConstructor)
{
    Matrix m1(4, 2);
    m1(1, 1) = 5;

    Matrix m2(m1);
    EXPECT_EQ(5, m2(1,1));

    m1(1, 1) = 10;
    EXPECT_EQ(5, m2(1,1));
}

TEST(MatrixGetters, RowsCols)
{
    Matrix m(4, 2);
    EXPECT_EQ(4, m.getRowsN());
    EXPECT_EQ(2, m.getColsN());
}

TEST(MatrixOperators, At)
{
    Matrix m(4, 2);

    EXPECT_EQ(0, m(0, 0));
    m(0, 0) = 5;
    EXPECT_EQ(5, m(0, 0));
}

TEST(MatrixOperators, OutOfRangeAccess)
{
    Matrix m(4, 2);

    EXPECT_THROW(m(4, 0), std::out_of_range);
    EXPECT_THROW(m(0, 2), std::out_of_range);
    EXPECT_THROW(m(4, 2), std::out_of_range);
    EXPECT_THROW(m(5, 2), std::out_of_range);
    EXPECT_THROW(m(2, 4), std::out_of_range);
    EXPECT_THROW(m(10, 10), std::out_of_range);

    EXPECT_THROW(m(3, -1), std::out_of_range);
    EXPECT_THROW(m(-2, 1), std::out_of_range);
    EXPECT_THROW(m(-10, -10), std::out_of_range);
}

TEST(MatrixOperators, Equality)
{
    Matrix m1(3, 2, {{1, 2}, {3, 4}, {5, 6}});
    Matrix m2(3, 2, {{10, 20}, {30, 40}, {50, 60}});
    Matrix m3(3, 2, {{1, 2}, {3, 4}, {5, 6}});
    Matrix m4(m1);

    EXPECT_TRUE(m1 == m3);
    EXPECT_TRUE(m3 == m1);
    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m1 == m4);

    Matrix m5(5, 10);
    Matrix m6(5, 8);
    Matrix m7(2, 10);
    Matrix m8(20, 20);

    EXPECT_FALSE(m5 == m6);
    EXPECT_FALSE(m5 == m7);
    EXPECT_FALSE(m5 == m8);
}

TEST(MatrixOperators, Disequality)
{
    Matrix m1(3, 2, {{1, 2}, {3, 4}, {5, 6}});
    Matrix m2(3, 2, {{10, 20}, {30, 40}, {50, 60}});
    Matrix m3(3, 2, {{1, 2}, {3, 4}, {5, 6}});
    Matrix m4(m1);

    EXPECT_FALSE(m1 != m3);
    EXPECT_FALSE(m3 != m1);
    EXPECT_TRUE(m1 != m2);
    EXPECT_FALSE(m1 != m4);
}

TEST(MatrixMethods, RowAccess) {
    NestedVector nv{{1,2}, {3, 4}, {5, 6}};
    Matrix m1(3, 2, nv);

    std::valarray<NumericType> r2 = m1.row(2); // Get by value
    EXPECT_EQ(5, r2[0]);
    EXPECT_EQ(6, r2[1]);

    std::slice_array<NumericType> rs2 = m1.row(2);
    rs2 = 20;
    EXPECT_EQ(20, m1(2, 0));
    EXPECT_EQ(20, m1(2, 1));

    std::valarray<NumericType> a{10, 30};
    rs2 = a;
    EXPECT_EQ(10, m1(2, 0));
    EXPECT_EQ(30, m1(2, 1));
}

TEST(MatrixMethods, ColumnAccess) {
    NestedVector nv{{1,2}, {3, 4}, {5, 6}};
    Matrix m1(3, 2, nv);

    std::valarray<NumericType> c0 = m1.col(0); // Get by value
    EXPECT_EQ(1, c0[0]);
    EXPECT_EQ(3, c0[1]);
    EXPECT_EQ(5, c0[2]);

    std::valarray<NumericType> c1 = m1.col(1); // Get by value
    EXPECT_EQ(2, c1[0]);
    EXPECT_EQ(4, c1[1]);
    EXPECT_EQ(6, c1[2]);

    std::slice_array<NumericType> cs1 = m1.col(1);
    cs1 = 20;
    EXPECT_EQ(20, m1(0, 1));
    EXPECT_EQ(20, m1(1, 1));
    EXPECT_EQ(20, m1(2, 1));

    std::valarray<NumericType> a{10, 30, 50};
    cs1 = a;
    EXPECT_EQ(10, m1(0, 1));
    EXPECT_EQ(30, m1(1, 1));
    EXPECT_EQ(50, m1(2, 1));
}

TEST(MatrixMethods, WrongRowColAccess) {
    NestedVector nv{{1, 2},
                    {3, 4},
                    {5, 6}};
    Matrix m1(3, 2, nv);

    EXPECT_THROW(m1.row(3), std::out_of_range);
    EXPECT_THROW(m1.row(4), std::out_of_range);
    EXPECT_THROW(m1.row(-1), std::out_of_range);

    EXPECT_THROW(m1.col(2), std::out_of_range);
    EXPECT_THROW(m1.col(3), std::out_of_range);
    EXPECT_THROW(m1.col(-1), std::out_of_range);
}

TEST(MatrixMethods, Transpose)
{
    // Size of transposed
    Matrix m1(3, 2, {{1, 2}, {3, 4}, {5, 6}});
    EXPECT_EQ(3, m1.getRowsN());
    EXPECT_EQ(2, m1.getColsN());

    // Values of transposed
    Matrix m2(m1.transposed());
    EXPECT_EQ(2, m2.getRowsN());
    EXPECT_EQ(3, m2.getColsN());
    EXPECT_EQ(1, m2(0, 0));
    EXPECT_EQ(2, m2(1, 0));
    EXPECT_EQ(3, m2(0, 1));
    EXPECT_EQ(4, m2(1, 1));
    EXPECT_EQ(5, m2(0, 2));
    EXPECT_EQ(6, m2(1, 2));

    // Transpose of transposed
    Matrix m3(m2.transposed());
    EXPECT_EQ(3, m3.getRowsN());
    EXPECT_EQ(2, m3.getColsN());
    EXPECT_TRUE(m1 == m3);

    // Make sure transposed is a copy
    m2(0, 0) = 10;
    EXPECT_EQ(1, m1(0, 0));
    EXPECT_EQ(1, m3(0, 0));
}

TEST(MatrixMethods, RREF)
{
    auto m = Matrix(3, 4, {{2, 1, -1, 8}, {-3, -1, 2, -11}, {-2, 1, 2, -3}});
//    m.print();

    auto mRowEchFormTrue = Matrix(3, 4, {{1, 0, 0, 2}, {0, 1, 0, 3}, {0, 0, 1, -1}});
//    mRowEchFormTrue.print();

    auto mRowEchForm = m.RREF();
//    mRowEchForm.print();

    for (Index i=0; i<mRowEchFormTrue.getRowsN(); i++)
    {
        for (Index j=0; j<mRowEchFormTrue.getColsN(); j++)
        {
            EXPECT_DOUBLE_EQ(mRowEchFormTrue(i, j), mRowEchForm(i, j));
        }
    }
}

TEST(MatrixMethods, InverseExceptions)
{
    EXPECT_THROW(Matrix(3,4).inverse(), std::domain_error);
}

TEST(MatrixMethods, InverseResult)
{
    Matrix a(3, 3, {{1, 2, 3}, {2, 5, 3}, {1, 0, 8}});

    auto ainv = a.inverse();

    EXPECT_EQ(3, ainv.getRowsN());
    EXPECT_EQ(3, ainv.getColsN());

    auto adot = a.dot(ainv);
    auto ident = Matrix::eye(a.getRowsN());

    for (Index i=0; i<adot.getRowsN(); i++)
    {
        for (Index j=0; j<adot.getColsN(); j++)
        {
            EXPECT_FLOAT_EQ(adot(i, j), ident(i, j));
        }
    }
}

TEST(MatrixMethods, InverseSingular)
{
    Matrix a(3, 3, {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}});
    EXPECT_THROW(a.inverse(), std::domain_error);
}

TEST(MatrixStaticMethods, AugmentedSize)
{
    Matrix a(3, 2, {{1, 1}, {2, 2}, {3, 3}});
    Matrix b(3, 4, {{10, 10, 10, 10}, {20, 20, 20, 20}, {30, 30, 30, 30}});

    Matrix ab = Matrix::augment(a, b);
    EXPECT_EQ(3, ab.getRowsN());
    EXPECT_EQ(6, ab.getColsN());

    Matrix ab2 = a.augment(b);
    EXPECT_EQ(3, ab2.getRowsN());
    EXPECT_EQ(6, ab2.getColsN());
}

TEST(MatrixStaticMethods, AugmentedContent)
{
    Matrix a(3, 2, {{1, 1}, {2, 2}, {3, 3}});
    Matrix b(3, 4, {{10, 10, 10, 10}, {20, 20, 20, 20}, {30, 30, 30, 30}});

    Matrix ab = Matrix::augment(a, b);
    Matrix abTrue = Matrix(3, 6, {{1, 1, 10, 10, 10, 10}, {2, 2, 20, 20, 20, 20},
                                  {3, 3, 30, 30, 30, 30}});
    EXPECT_TRUE(ab == abTrue);
}

TEST(MatrixStaticMethods, AugmentedExcept)
{
    EXPECT_THROW(Matrix::augment(Matrix(2, 5), Matrix(5, 2)), std::invalid_argument);
}

TEST(MatrixStaticMethods, Identity)
{
    Matrix id1 = Matrix::eye(1);
    Matrix idTrue1 = Matrix(1, 1, {{1}});
    EXPECT_TRUE(idTrue1 == id1);

    Matrix id2 = Matrix::eye(3);
    Matrix idTrue2 = Matrix(3, 3, {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}});
    EXPECT_TRUE(idTrue2 == id2);
}

TEST(MatrixStaticMethods, DotProductSizes)
{
    Matrix m1(5, 3);
    Matrix m2(3, 7);
    Matrix m3(4, 7);

    auto m12 = Matrix::dot(m1, m2);
    EXPECT_EQ(5, m12.getRowsN());
    EXPECT_EQ(7, m12.getColsN());

    EXPECT_THROW(Matrix::dot(m1, m3), std::invalid_argument);
    EXPECT_THROW(Matrix::dot(m2, m3), std::invalid_argument);

    auto m23t = Matrix::dot(m2, m3.transposed());
    EXPECT_EQ(3, m23t.getRowsN());
    EXPECT_EQ(4, m23t.getColsN());

    auto m12b = m1.dot(m2);
    EXPECT_TRUE(m12 == m12b);
}

TEST(MatrixStaticMethods, DotProductResult) {
    Matrix m1(3, 3, {{0, 1, 2},
                     {3, 4, 5},
                     {6, 7, 8}});
    Matrix m2(3, 1, {{10},
                     {20},
                     {30}});
    Matrix m12True(3, 1, {{80},
                          {260},
                          {440}});

    auto m12 = Matrix::dot(m1, m2);
    EXPECT_TRUE(m12 == m12True);
}
