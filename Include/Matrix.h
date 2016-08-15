/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_MATRIX_H
#define PSA_MATRIX_H

#include <ostream>

#include "AlgoBase.h"
#include "AlgoException.h"

namespace psa {

/**
 * This class abstracts a column major matrix.
 */
template<typename ElemType>
class Matrix
{
public:
    Matrix(unsigned int nrow, unsigned int ncolumn)
        : m_nrow(nrow),
        m_ncol(ncolumn),
        m_matrix(nullptr)
    {
        if (0 == m_nrow || 0 == m_ncol)
            throw AlgoException{AlgoException::MatrixZeroDimension};
        m_matrix = new ElemType[m_nrow * m_ncol];
    }
    Matrix(unsigned int nrow, unsigned int ncolumn, ElemType* values)
        : m_nrow(nrow),
        m_ncol(ncolumn),
        m_matrix(nullptr)
    {
        if (0 == m_nrow || 0 == m_ncol)
            throw AlgoException{AlgoException::MatrixZeroDimension};
        m_matrix = new ElemType[m_nrow * m_ncol];
        std::copy(&values[0], &values[m_nrow * m_ncol], &m_matrix[0]);
    }
    Matrix(const Matrix& rhs)
        : m_nrow(rhs.m_nrow),
        m_ncol(rhs.m_ncol),
        m_matrix(nullptr)
    {
        m_matrix = new ElemType[m_nrow * m_ncol];
        std::copy(&rhs.m_matrix[0], &rhs.m_matrix[m_nrow * m_ncol], &m_matrix[0]);
    }
    ~Matrix() { delete [] m_matrix; }

    Matrix &operator = (const Matrix& rhs)
    {
        if (&rhs != this) {
            m_nrow = rhs.m_nrow;
            m_ncol = rhs.m_ncol;
            m_matrix = new ElemType[m_nrow * m_ncol];
            std::copy(&rhs.m_matrix[0], &rhs.m_matrix[m_nrow * m_ncol], &m_matrix[0]);
        }
        return *this;
    }

    // ops
    void makeZero();
    void makeIdentity();

    Matrix& operator += (const Matrix& rhs) { *this = *this + rhs; return *this; }
    Matrix& operator *= (const Matrix& rhs) { *this = *this * rhs; return *this; }

    double determinant() const;

    bool operator == (const Matrix& rhs) const;
    bool operator != (const Matrix& rhs) const;
    Matrix operator + (const Matrix& rhs) const;
    Matrix operator * (const Matrix& rhs) const;

    void rotate();

    // access
    unsigned int nrow() const { return m_nrow; }
    unsigned int ncolumn() const { return m_ncol; }
    ElemType valueAt(unsigned int irow, unsigned int icolumn) const
    {
        return m_matrix[icolumn*m_nrow + irow];
    }
    const ElemType* data() const { return m_matrix; }

    // setters
    void setValueAt(unsigned int irow, unsigned int icolumn, ElemType value)
    {
        m_matrix[icolumn*m_nrow + irow] = value;
    }
    void setColumn(unsigned int icolumn, const ElemType* values);

    // others
    static Matrix zero(unsigned int nrow, unsigned int ncolumn);
    static Matrix identity(unsigned int dimension);

private:
    void resize(unsigned int nrow, unsigned int ncol)
    {
        if (nrow == m_nrow && ncol == m_ncol)
            return;

        delete [] m_matrix;

        m_nrow = nrow;
        m_ncol = ncol;
        m_matrix = new ElemType[m_nrow * m_ncol];
    }

    Matrix determinantHelper(unsigned int irow, unsigned int icol);

    unsigned int m_nrow;
    unsigned int m_ncol;
    ElemType* m_matrix;
};

template<typename ElemType> std::ostream& operator<<(std::ostream& out,
                                                     const Matrix<ElemType>& matrix);

template<typename ElemType>
void Matrix<ElemType>::makeZero()
{
    unsigned int size = m_nrow * m_ncol;
    for (unsigned int i = 0; i < size; ++i)
        m_matrix[i] = 0;
}

template<typename ElemType>
void Matrix<ElemType>::makeIdentity()
{
    for (unsigned int j = 0; j < m_ncol; ++j) {
        unsigned int iColumnStart = j * m_nrow;
        for (unsigned int i = 0; i < m_nrow; ++i) {
            if (i == j)
                m_matrix[iColumnStart + i] = 1;
            else
                m_matrix[iColumnStart + i] = 0;
        }
    }
}

template<typename ElemType>
double Matrix<ElemType>::determinant() const
{
    double det = 0.0;
    if (m_nrow == 1) {
        det = m_matrix[0];
    }
    else {
        for (unsigned int i = 0; i < m_ncol; ++i) {
            if (0 == m_matrix[i * m_nrow])
                continue;
            Matrix tmp = this->determinantHelper(0, i);
            det += ((i%2 == 0) ? 1 : -1) * m_matrix[i * m_nrow] * tmp.determinant();
        }
    }
    return det;
}

template<typename ElemType>
bool Matrix<ElemType>::operator == (const Matrix& rhs) const
{
    bool equal = true;

    if (m_nrow != rhs.m_nrow || m_ncol != rhs.m_ncol)
        equal = false;

    unsigned int size = m_nrow * m_ncol;
    for (unsigned int i = 0; equal && i < size; ++i) {
        if (!isequal(m_matrix[i], rhs.m_matrix[i]))
            equal = false;
    }

    return equal;
}

template<typename ElemType>
bool Matrix<ElemType>::operator != (const Matrix& rhs) const
{
    bool notEqual = true;

    if (rhs.m_nrow == m_nrow || rhs.m_ncol == m_ncol)
        notEqual = false;

    unsigned int size = m_nrow * m_ncol;
    for (unsigned int i = 0; notEqual && i < size; ++i) {
        if (isequal(m_matrix[i], rhs.m_matrix[i]))
            notEqual = false;
    }

    return notEqual;
}

template<typename ElemType>
Matrix<ElemType> Matrix<ElemType>::operator + (const Matrix& rhs) const
{
    if (m_nrow != rhs.m_nrow || m_ncol != rhs.m_ncol)
        throw AlgoException{AlgoException::MatrixDimMismatchForAdd};

    Matrix<ElemType> result{m_nrow, m_ncol};
    result.makeZero();

    unsigned int size = m_nrow * m_ncol;
    for (unsigned int i = 0; i < size; ++i)
        result.m_matrix[i] = m_matrix[i] + rhs.m_matrix[i];

    return result;
}

template<typename ElemType>
Matrix<ElemType> Matrix<ElemType>::operator * (const Matrix& rhs) const
{
    if (m_ncol != rhs.m_nrow)
        throw AlgoException{AlgoException::MatrixDimMismatchForMult};

    Matrix<ElemType> result = Matrix<ElemType>{m_nrow, rhs.m_ncol};
    result.makeZero();

    for (unsigned int i = 0; i < result.m_nrow; ++i) {
        for (unsigned int j = 0; j < result.m_ncol; ++j) {
            for (unsigned int k = 0; k < m_ncol; ++k) {
                result.m_matrix[j*result.m_nrow + i]
                    += m_matrix[k*m_nrow + i] * rhs.m_matrix[j*rhs.m_nrow + k];
            }
        }
    }

    return result;
}

template<typename ElemType>
void Matrix<ElemType>::rotate()
{
    if (m_nrow != m_ncol)
        return;

    int i = 0;
    int j = m_nrow - 1;
    while (i < j) {
        int len = j - i;
        for (int k = 0; k < len; ++k) {
            ElemType tmp = this->valueAt(i, i + k);
            this->setValueAt(i, i + k, this->valueAt(j - k, i));
            this->setValueAt(j - k, i, this->valueAt(j, j - k));
            this->setValueAt(j, j - k, this->valueAt(i + k, j));
            this->setValueAt(i + k, j, tmp);
        }
        ++i;
        --j;
    }
}

template<typename ElemType>
void Matrix<ElemType>::setColumn(unsigned int icolumn, const ElemType* values)
{
    if (icolumn >= m_ncol)
        return;

    ElemType* matrix = &m_matrix[icolumn * m_nrow];

    for (unsigned int i = 0; i < m_nrow; ++i) {
        *matrix = *values;
        ++matrix;
        ++values;
    }
}

template<typename ElemType>
Matrix<ElemType> Matrix<ElemType>::zero(unsigned int nrow, unsigned int ncolumn)
{
    Matrix<ElemType> matrix{nrow, ncolumn};
    matrix.makeZero();
    return matrix;
}

template<typename ElemType>
Matrix<ElemType> Matrix<ElemType>::identity(unsigned int dimension)
{
    Matrix<ElemType> eye{dimension, dimension};
    eye.makeIdentity();
    return eye;
}

template<typename ElemType>
Matrix<ElemType> Matrix<ElemType>::determinantHelper(unsigned int irow, unsigned int icol)
{
    Matrix matrix(m_nrow - 1, m_ncol - 1);

    unsigned int k = 0;
    for (unsigned int i = 0; i < m_ncol; ++i) {
        if (i == icol)
            continue;
        for (int j = 0; j < m_nrow; ++j) {
            if (j == irow)
                continue;
            matrix.m_matrix[k++] = m_matrix[i * m_nrow + j];
        }
    }
    return matrix;
}

} // namespace psa

#endif // PSA_MATRIX_H
