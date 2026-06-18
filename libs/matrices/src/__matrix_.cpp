#include "__matrix_.h"
#include <iostream>
#include <stdexcept>

// Constructor
__matrix_::__matrix_(int r, int c) : rows(r), cols(c), data(r * c, 0.0) {}

// Element access
double &__matrix_::operator()(int r, int c)
{
    return data[r * cols + c];
}

double __matrix_::operator()(int r, int c) const
{
    return data[r * cols + c];
}

void __matrix_::set(int r, int c, double value)
{
    data[r * cols + c] = value;
}
double __matrix_::get(int r, int c) const
{
    return data[r * cols + c];
}

// Dimensions
int __matrix_::getRows() const { return rows; }
int __matrix_::getCols() const { return cols; }

// matrix addition
__matrix_ __matrix_::operator+(const __matrix_ &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("Size mismatch in addition");

    __matrix_ result(rows, cols);
    for (int i = 0; i < rows * cols; i++)
        result.data[i] = data[i] + other.data[i];

    return result;
}

// matrix multiplication
__matrix_ __matrix_::operator*(const __matrix_ &other) const
{
    if (cols != other.rows)
        throw std::runtime_error("Invalid multiply dimensions");

    __matrix_ result(rows, other.cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < other.cols; j++)
        {
            for (int k = 0; k < cols; k++)
            {
                result(i, j) += (*this)(i, k) * other(k, j);
            }
        }
    }

    return result;
}

// Scalar multiply
__matrix_ __matrix_::operator*(double scalar) const
{
    __matrix_ result(rows, cols);
    for (int i = 0; i < rows * cols; i++)
        result.data[i] = data[i] * scalar;
    return result;
}

// Transpose
__matrix_ __matrix_::transpose() const
{
    __matrix_ result(cols, rows);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result(j, i) = (*this)(i, j);
    return result;
}

// Element-wise (Hadamard) multiplication
__matrix_ __matrix_::hadamard(const __matrix_ &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("Size mismatch in Hadamard multiply");

    __matrix_ result(rows, cols);
    for (int i = 0; i < rows * cols; i++)
        result.data[i] = data[i] * other.data[i];

    return result;
}

// Apply function (for activation functions)
__matrix_ __matrix_::apply(double (*func)(double)) const
{
    __matrix_ result(rows, cols);
    for (int i = 0; i < rows * cols; i++)
        result.data[i] = func(data[i]);
    return result;
}

// Print for debugging
void __matrix_::print() const
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << (*this)(i, j) << " ";
        }
        std::cout << "\n";
    }
}

__matrix_ make_matrix_(std::initializer_list<double> values)
{
    __matrix_ m(values.size(), 1);

    int i = 0;
    for (double v : values)
    {
        m(i, 0) = v;
        i++;
    }

    return m;
}

__matrix_ one_hot_(int index, int size)
{
    __matrix_ m(size, 1);

    for (int i = 0; i < size; i++)
        m(i, 0) = 0;

    m(index, 0) = 1;
    return m;
}