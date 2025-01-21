#include "vector_t.h"
#include <iostream>
#include <stdexcept>
#include <cmath> // Potentially for advanced operations

// Constructor: allocate memory for 'size' elements
Vector::Vector(size_t size)
    : m_size(size), m_data(nullptr)
{
    if (m_size == 0)
    {
        throw std::runtime_error("Vector size must be > 0.");
    }
    m_data = new double[m_size];
    // Initialize to zero or any default value
    for (size_t i = 0; i < m_size; ++i)
    {
        m_data[i] = 0.0;
    }
}

// Copy constructor
Vector::Vector(const Vector& other)
    : m_size(other.m_size), m_data(nullptr)
{
    m_data = new double[m_size];
    for (size_t i = 0; i < m_size; ++i)
    {
        m_data[i] = other.m_data[i];
    }
}

// Destructor
Vector::~Vector()
{
    delete[] m_data;
}

// Assignment operator
Vector& Vector::operator=(const Vector& other)
{
    if (this == &other)
        return *this; // Avoid self-assignment

    // 1) Deallocate current memory
    delete[] m_data;

    // 2) Copy size
    m_size = other.m_size;

    // 3) Allocate new memory and copy
    m_data = new double[m_size];
    for (size_t i = 0; i < m_size; ++i)
    {
        m_data[i] = other.m_data[i];
    }

    return *this;
}

// Access operators
double& Vector::operator[](size_t index)
{
    if (index >= m_size)
    {
        throw std::out_of_range("Index out of range");
    }
    return m_data[index];
}

const double& Vector::operator[](size_t index) const
{
    if (index >= m_size)
    {
        throw std::out_of_range("Index out of range");
    }
    return m_data[index];
}

// Vector + Vector
Vector Vector::operator+(const Vector& other) const
{
    if (m_size != other.m_size)
    {
        throw std::runtime_error("Vectors must have the same dimension for addition.");
    }

    Vector result(m_size);
    for (size_t i = 0; i < m_size; ++i)
    {
        result.m_data[i] = m_data[i] + other.m_data[i];
    }
    return result;
}

// Vector + scalar
Vector Vector::operator+(double scalar) const
{
    Vector result(m_size);
    for (size_t i = 0; i < m_size; ++i)
    {
        result.m_data[i] = m_data[i] + scalar;
    }
    return result;
}

// Vector * scalar
Vector Vector::operator*(double scalar) const
{
    Vector result(m_size);
    for (size_t i = 0; i < m_size; ++i)
    {
        result.m_data[i] = m_data[i] * scalar;
    }
    return result;
}

// Dot product
double Vector::dot(const Vector& other) const
{
    if (m_size != other.m_size)
    {
        throw std::runtime_error("Vectors must have the same dimension for dot product.");
    }

    double sum = 0.0;
    for (size_t i = 0; i < m_size; ++i)
    {
        sum += m_data[i] * other.m_data[i];
    }
    return sum;
}

// Cross product (only valid for 3D)
Vector Vector::cross(const Vector& other) const
{
    if (m_size != 3 || other.m_size != 3)
    {
        throw std::runtime_error("Cross product is only defined for 3-dimensional vectors.");
    }

    Vector result(3);
    result[0] = m_data[1] * other.m_data[2] - m_data[2] * other.m_data[1];
    result[1] = m_data[2] * other.m_data[0] - m_data[0] * other.m_data[2];
    result[2] = m_data[0] * other.m_data[1] - m_data[1] * other.m_data[0];

    return result;
}
