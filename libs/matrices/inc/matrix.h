#ifndef MATRIX_H
#define MATRIX_H
#include <initializer_list>
#define GPU_THREADS 512
class matrix
{
private:
    int rows;
    int cols;

    double *data;

public:
    matrix();
    matrix(int r, int c);

    ~matrix();

    double &operator()(int r, int c);
    double operator()(int r, int c) const;

    matrix(const matrix &other);
    matrix &operator=(const matrix &other);

    matrix(matrix &&other);
    matrix &operator=(matrix &&other);

    int getRows() const;
    int getCols() const;

    double *device_ptr();
    const double *device_ptr() const;

    void set(int r, int c, double value);
    double get(int r, int c) const;

    // matrix operations
    matrix operator*(const matrix &other) const;
    matrix operator+(const matrix &other) const;
    matrix operator*(const double scalar) const;
    matrix operator-(const matrix &other) const;
    // matrix operator/(const double scalar) const;

    matrix hadamard(const matrix &other) const;
    matrix transpose() const;
    matrix sigmoid() const;
    matrix sigmoid_derivative() const;
    matrix relu() const;
    double sum_of_squares() const;
    void randomize();
    void print() const;
};

struct DataPoint
{
    matrix input;
    matrix target;
};

matrix make_matrix(std::initializer_list<double> values);
matrix one_hot(int index, int size);

#endif