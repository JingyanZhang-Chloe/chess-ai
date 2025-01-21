#ifndef VECTOR_T_H
#define VECTOR_T_H

#include <cstddef>   // For size_t
#include <stdexcept> // For std::runtime_error

class Vector
{
private:
    size_t m_size;   // Dimension of the vector
    double* m_data;  // Pointer to the elements

public:
    // Constructors and Destructor
    explicit Vector(size_t size);              // Create vector of given size
    Vector(const Vector& other);               // Copy constructor
    ~Vector();                                 // Destructor

    // Assignment Operator
    Vector& operator=(const Vector& other);

    // Accessors
    size_t size() const { return m_size; }
    double& operator[](size_t index);
    const double& operator[](size_t index) const;

    // Vector-Vector operations
    Vector operator+(const Vector& other) const; // Vector + Vector
    // Vector-Scalar operations
    Vector operator+(double scalar) const;       // Vector + scalar
    Vector operator*(double scalar) const;       // Vector * scalar
    // Dot product
    double dot(const Vector& other) const;
    // Cross product (only valid for 3D vectors)
    Vector cross(const Vector& other) const;
};

#endif // VECTOR_T_H
