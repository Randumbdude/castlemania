#pragma once

#include <initializer_list>
#include <vector>

class __matrix_
{
private:
    int rows, cols;
    std::vector<double> data;

public:
    // Constructor
    __matrix_(int r, int c);
    __matrix_() : rows(0), cols(0), data() {}

    // Element access
    double &operator()(int r, int c);
    double operator()(int r, int c) const;

    // Dimensions
    int getRows() const;
    int getCols() const;

    void set(int r, int c, double value);
    double get(int r, int c) const;

    // Operations
    __matrix_ operator+(const __matrix_ &other) const;
    __matrix_ operator*(const __matrix_ &other) const;
    __matrix_ operator*(double scalar) const;

    // Utility
    __matrix_ transpose() const;
    __matrix_ hadamard(const __matrix_ &other) const;
    __matrix_ apply(double (*func)(double)) const;

    void print() const;
};

struct DataPoint_
{
    __matrix_ input;
    __matrix_ target;
};

__matrix_ make_matrix_(std::initializer_list<double> values);
__matrix_ one_hot_(int index, int size);
