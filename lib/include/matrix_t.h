#ifndef MATRIX_T_H
#define MATRIX_T_H

#include <cstddef>   // For size_t
#include <stdexcept> // For exceptions

class Matrix
{
private:
    size_t m_rows;     // Number of rows
    size_t m_cols;     // Number of columns
    double* m_data;    // Pointer to data in row-major order

public:
    // Constructors and Destructor
    Matrix(size_t rows, size_t cols);       // Creates a rows x cols matrix, initialized to 0
    Matrix(const Matrix& other);           // Copy constructor
    ~Matrix();                             // Destructor

    // Assignment Operator
    Matrix& operator=(const Matrix& other);

    // Accessors
    size_t rows() const { return m_rows; }
    size_t cols() const { return m_cols; }

    // Element access (row, col)
    // Using operator() for 2D access:
    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;

    // Matrix-Matrix operations
    Matrix operator+(const Matrix& other) const; // Matrix + Matrix
    Matrix operator*(const Matrix& other) const; // Matrix * Matrix (matrix multiplication)

    // Matrix-Scalar operations
    Matrix operator+(double scalar) const;       // Matrix + scalar
    Matrix operator*(double scalar) const;       // Matrix * scalar
};

#endif // MATRIX_T_H
