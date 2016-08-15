/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "Matrix.h"

#include <cassert>

#ifdef UNIT_TEST
#include "gtest/gtest.h"
#endif

namespace psa {

template<typename ElemType>
std::ostream& operator << (std::ostream& out, const Matrix<ElemType>& matrix)
{
    for (unsigned int i = 0; i < matrix.nrow(); ++i) {
        for (unsigned int j = 0; j < matrix.ncolumn(); ++j) {
            out << matrix.valueAt(i, j) << "\t";
        }
        out << std::endl;
    }
    out << std::endl;

    return out;
}

#ifdef UNIT_TEST

TEST(MatrixTest, Creation)
{
    bool passed = false;
    try {
        Matrix<double> matrix(10, 0);
    } catch (const AlgoException& /*e*/) {
        //std::cout << e.what() << std::endl;
        passed = true;
    }
    EXPECT_TRUE(passed) << "Matrix with 0 columns should throw an exception!";

    passed = false;
    try {
        Matrix<double> matrix(0, 3);
    } catch (const AlgoException& /*e*/) {
        //std::cout << e.what() << std::endl;
        passed = true;
    }
    EXPECT_TRUE(passed) << "Matrix with 0 rows should throw an exception!";
}

TEST(MatrixTest, Equality)
{
    Matrix<float> matrix1x1(1, 1);
    EXPECT_TRUE(matrix1x1 == matrix1x1) << "Equality test for same matrix (1x1) should succeed.";

    Matrix<float> matrix2x2(2, 2);
    matrix2x2.setValueAt(1, 0, 2.7e-2f);
    EXPECT_TRUE(matrix2x2 == matrix2x2) << "Equality test for same matrix (2x2) should succeed.";

    Matrix<float> matrix5x3(5, 3);
    matrix5x3.setValueAt(3, 2, 34.23f);
    matrix5x3.setValueAt(1, 2, 56.21f);
    EXPECT_TRUE(matrix5x3 == matrix5x3) << "Equality test for same matrix (5x3) should succeed.";
}

TEST(MatrixTest, Zero)
{
    Matrix<double> expected1x1(1, 1);
    expected1x1.setValueAt(0, 0, 0.0);
    Matrix<double> actual1x1 = Matrix<double>::zero(1, 1);
    EXPECT_EQ(expected1x1, actual1x1);

    Matrix<double> expected2x2(2, 2);
    expected2x2.setValueAt(0, 0, 0.0);
    expected2x2.setValueAt(0, 1, 0.0);
    expected2x2.setValueAt(1, 0, 0.0);
    expected2x2.setValueAt(1, 1, 0.0);
    Matrix<double> actual2x2 = Matrix<double>::zero(2, 2);
    EXPECT_EQ(expected2x2, actual2x2);
}

TEST(MatrixTest, Identity)
{
    Matrix<double> expected1x1(1, 1);
    expected1x1.setValueAt(0, 0, 1.0);
    Matrix<double> actual1x1 = Matrix<double>::identity(1);
    EXPECT_EQ(expected1x1, actual1x1);

    expected1x1.setValueAt(0, 0, 23.0);
    EXPECT_FALSE(expected1x1 == actual1x1);

    Matrix<double> expected2x2(2, 2);
    expected2x2.setValueAt(0, 0, 1.0);
    expected2x2.setValueAt(0, 1, 0.0);
    expected2x2.setValueAt(1, 0, 0.0);
    expected2x2.setValueAt(1, 1, 1.0);
    Matrix<double> actual2x2 = Matrix<double>::identity(2);
    EXPECT_EQ(expected2x2, actual2x2);
}

TEST(MatrixTest, Addition)
{
    float aValues[] = { 0.1419f, 0.4218f, 0.9157f, 0.7922f, 0.9595f, 0.6557f };
    Matrix<float> a3x2(3, 2, aValues);
    float bValues[] = { 0.0357f, -0.8491f, 0.9340f, 0.6787f, 0.7577f, 0.7431f };
    Matrix<float> b3x2(3, 2, bValues);
    float e3x2Values[] = { 0.1776f, -0.4273f, 1.8497f, 1.4709f, 1.7172f, 1.3988f };
    Matrix<float> expected3x2(3, 2, e3x2Values);
    Matrix<float> actual3x2 = a3x2 + b3x2;
    EXPECT_EQ(expected3x2, actual3x2);
}

TEST(MatrixTest, Multiplication)
{
    float aValues[] = {
        0.392227019534168f, 0.655477890177557f, 0.171186687811562f,
        0.706046088019609f, 0.031832846377421f, 0.276922984960890f
    };
    Matrix<float> a2x3(2, 3, aValues);
    float bValues[] = {
        0.046171390631154f, 0.097131781235848f, 0.823457828327293f,
        0.694828622975817f, 0.317099480060861f, 0.950222048838355f,
        0.034446080502909f, 0.438744359656398f, 0.381558457093008f,
        0.765516788149002f, 0.795199901137063f, 0.186872604554379f
    };
    Matrix<float> b3x4(3, 4, bValues);
    float e2x4Values[] = {
        0.060950341393434f, 0.326878239691225f, 0.357062042080331f, 0.942469973373158f,
        0.100763968964521f, 0.438014689824269f, 0.442332692355716f, 1.114976428043425f
    };
    Matrix<float> expected2x4(2, 4, e2x4Values);
    Matrix<float> actual2x4 = a2x3 * b3x4;
    EXPECT_EQ(expected2x4, actual2x4);
}

TEST(MatrixTest, Rotate)
{
    float values[] = { 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16 };
    Matrix<float> mat{4, 4, values};
    mat.rotate();
    float expectedValues[] = {13, 14, 15, 16, 9, 10, 11, 12, 5, 6, 7, 8, 1, 2, 3, 4};
    Matrix<float> expected{4, 4, expectedValues};
    EXPECT_EQ(expected, mat);
}

#endif // UNIT_TEST

} // namespace psa
