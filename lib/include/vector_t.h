#ifndef VECTOR_T_H
#define VECTOR_T_H

#include <cstddef>   
#include <stdexcept> 

class Vector
{
private:
    size_t m_size;   
    double* m_data;  

public:
    // Constructors and Destructor
    explicit Vector(size_t size);              
    Vector(const Vector& other);               
    ~Vector();                                 

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
