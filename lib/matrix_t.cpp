#include "matrix_t.h"
#include <stdexcept>
#include <iostream>
#include <cstring> 

// Constructor
Matrix::Matrix(size_t rows, size_t cols)
    : m_rows(rows), m_cols(cols), m_data(nullptr)
{
    if (rows == 0 || cols == 0)
    {
        throw std::runtime_error("Matrix dimensions must be greater than 0.");
    }

    // Allocate a single block of memory for (rows * cols) doubles
    m_data = new double[m_rows * m_cols];

    // Initialize all elements to 0.0
    for (size_t i = 0; i < m_rows * m_cols; ++i)
    {
        m_data[i] = 0.0;
    }
}

// Copy constructor
Matrix::Matrix(const Matrix& other)
    : m_rows(other.m_rows), m_cols(other.m_cols), m_data(nullptr)
{
    // Allocate new memory block
    m_data = new double[m_rows * m_cols];

    std::memcpy(m_data, other.m_data, sizeof(double) * m_rows * m_cols);
}

// Destructor
Matrix::~Matrix()
{
    delete[] m_data; // clean up
}

// Assignment operator
Matrix& Matrix::operator=(const Matrix& other)
{
    if (this == &other)
        return *this; 

    delete[] m_data;

    m_rows = other.m_rows;
    m_cols = other.m_cols;

    m_data = new double[m_rows * m_cols];
    std::memcpy(m_data, other.m_data, sizeof(double) * m_rows * m_cols);

    return *this;
}

// Element access
double& Matrix::operator()(size_t row, size_t col)
{
    if (row >= m_rows || col >= m_cols)
    {
        throw std::out_of_range("Matrix subscript out of range.");
    }
    // row-major index
    return m_data[row * m_cols + col];
}

const double& Matrix::operator()(size_t row, size_t col) const
{
    if (row >= m_rows || col >= m_cols)
    {
        throw std::out_of_range("Matrix subscript out of range.");
    }
    return m_data[row * m_cols + col];
}

// Matrix + Matrix
Matrix Matrix::operator+(const Matrix& other) const
{
    if (m_rows != other.m_rows || m_cols != other.m_cols)
    {
        throw std::runtime_error("Matrix dimensions must match for addition.");
    }

    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows * m_cols; ++i)
    {
        result.m_data[i] = m_data[i] + other.m_data[i];
    }
    return result;
}

// Matrix * Matrix (matrix multiplication)
Matrix Matrix::operator*(const Matrix& other) const
{

    if (m_cols != other.m_rows)
    {
        throw std::runtime_error(
            "Matrix dimensions are not compatible for multiplication. "
            "(A.cols must equal B.rows)");
    }

    Matrix result(m_rows, other.m_cols);

    // Standard matrix multiplication O(r1 * c2 * c1)
    for (size_t r = 0; r < m_rows; ++r)
    {
        for (size_t c = 0; c < other.m_cols; ++c)
        {
            double sum = 0.0;
            for (size_t k = 0; k < m_cols; ++k)
            {
                sum += (*this)(r, k) * other(k, c);
            }
            result(r, c) = sum;
        }
    }
    return result;
}

// Matrix + scalar
Matrix Matrix::operator+(double scalar) const
{
    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows * m_cols; ++i)
    {
        result.m_data[i] = m_data[i] + scalar;
    }
    return result;
}

// Matrix * scalar
Matrix Matrix::operator*(double scalar) const
{
    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows * m_cols; ++i)
    {
        result.m_data[i] = m_data[i] * scalar;
    }
    return result;
}
