#include "matrix.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

#include <cuda_runtime.h>

// Constructor
matrix::matrix()
{
    rows = 0;
    cols = 0;
    data = nullptr;
}

matrix::matrix(int r, int c)
{
    rows = r;
    cols = c;

    size_t bytes = rows * cols * sizeof(double);

    cudaError_t err =
        cudaMallocManaged(&data, bytes);

    if (err != cudaSuccess)
    {
        throw std::runtime_error(
            cudaGetErrorString(err));
    }

    cudaMemset(
        data,
        0,
        bytes);
}

matrix::matrix(const matrix &other)
{
    rows = other.rows;
    cols = other.cols;

    size_t bytes = rows * cols * sizeof(double);

    cudaError_t err =
        cudaMallocManaged(&data, bytes);

    if (err != cudaSuccess)
    {
        throw std::runtime_error(
            cudaGetErrorString(err));
    }

    cudaMemcpy(
        data,
        other.data,
        bytes,
        cudaMemcpyDefault);
}

matrix::matrix(matrix &&other)
{
    rows = other.rows;
    cols = other.cols;

    data = other.data;

    other.rows = 0;
    other.cols = 0;
    other.data = nullptr;
}

// destructor
matrix::~matrix()
{
    if (data)
        cudaFree(data);
}

double &matrix::operator()(int r, int c)
{
    return data[r * cols + c];
}
double matrix::operator()(int r, int c) const
{
    return data[r * cols + c];
}

matrix &matrix::operator=(const matrix &other)
{
    if (this == &other)
        return *this;

    if (data)
        cudaFree(data);

    rows = other.rows;
    cols = other.cols;

    size_t bytes = rows * cols * sizeof(double);

    // cudaMalloc(&data, bytes);
    cudaError_t err =
        cudaMallocManaged(&data, bytes);

    if (err != cudaSuccess)
    {
        throw std::runtime_error(
            cudaGetErrorString(err));
    }

    cudaMemcpy(
        data,
        other.data,
        bytes,
        cudaMemcpyDefault);

    return *this;
}

matrix &matrix::operator=(matrix &&other)
{
    if (this == &other)
        return *this;

    if (data)
        cudaFree(data);

    rows = other.rows;
    cols = other.cols;
    data = other.data;

    other.rows = 0;
    other.cols = 0;
    other.data = nullptr;

    return *this;
}

int matrix::getRows() const
{
    return rows;
}

int matrix::getCols() const
{
    return cols;
}

double *matrix::device_ptr()
{
    return data;
}
const double *matrix::device_ptr() const
{
    return data;
}

void matrix::set(int r, int c, double value)
{
    data[r * cols + c] = value;
}

double matrix::get(int r, int c) const
{
    return data[r * cols + c];
}

//
// CUDA GPU Kernels
//
__global__ void add_kernel(
    const double *a,
    const double *b,
    double *out,
    int size)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < size)
        out[idx] = a[idx] + b[idx];
}

__global__ void subtract_kernel(
    const double *a,
    const double *b,
    double *out,
    int size)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size)
        out[idx] = a[idx] - b[idx];
}

__global__ void scalar_kernel(
    const double *a,
    double scalar,
    double *out,
    int size)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < size)
        out[idx] = a[idx] * scalar;
}

__global__ void hadamard_kernel(
    const double *a,
    const double *b,
    double *out,
    int size)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < size)
        out[idx] = a[idx] * b[idx];
}

__global__ void transpose_kernel(
    const double *input,
    double *output,
    int rows,
    int cols)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < rows && col < cols)
    {
        output[col * rows + row] = input[row * cols + col];
    }
}

__global__ void matmul_kernel(
    const double *A,
    const double *B,
    double *C,
    int rowsA,
    int colsA,
    int colsB)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < rowsA && col < colsB)
    {
        double sum = 0.0;

        for (int k = 0; k < colsA; k++)
        {
            sum +=
                A[row * colsA + k] *
                B[k * colsB + col];
        }

        C[row * colsB + col] = sum;
    }
}

__global__ void sigmoid_kernel(
    const double *input,
    double *output,
    int size)
{
    int idx =
        blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < size)
    {
        output[idx] =
            1.0 / (1.0 + exp(-input[idx]));
    }
}

__global__ void sigmoid_derivative_kernel(
    const double *input,
    double *output,
    int size)
{
    int idx =
        blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < size)
    {
        double s = input[idx];

        output[idx] =
            s * (1.0 - s);
    }
}

__global__ void relu_kernel(
    const double *input,
    double *output,
    int size)
{
    int idx =
        blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < size)
    {
        output[idx] =
            input[idx] > 0.0
                ? input[idx]
                : 0.0;
    }
}

//
// matrix operators
//
matrix matrix::operator+(const matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("Size mismatch in addition");

    matrix result(rows, cols);

    int size = rows * cols;

    int threads = GPU_THREADS;
    int blocks = (size + threads - 1) / threads;

    add_kernel<<<blocks, threads>>>(
        data,
        other.data,
        result.data,
        size);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        throw std::runtime_error(cudaGetErrorString(err));
    }

    cudaDeviceSynchronize();

    return result;
}

matrix matrix::operator-(const matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("Size mismatch in subtraction");

    matrix result(rows, cols);

    int size = rows * cols;

    int threads = GPU_THREADS;
    int blocks = (size + threads - 1) / threads;

    subtract_kernel<<<blocks, threads>>>(
        data,
        other.data,
        result.data,
        size);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        throw std::runtime_error(cudaGetErrorString(err));
    }

    cudaDeviceSynchronize();

    return result;
}

matrix matrix::operator*(double scalar) const
{
    matrix result(rows, cols);

    int size = rows * cols;

    int threads = GPU_THREADS;
    int blocks = (size + threads - 1) / threads;

    scalar_kernel<<<blocks, threads>>>(
        data,
        scalar,
        result.data,
        size);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        throw std::runtime_error(cudaGetErrorString(err));
    }

    cudaDeviceSynchronize();

    return result;
}

matrix matrix::operator*(const matrix &other) const
{
    if (cols != other.rows)
        throw std::runtime_error("Invalid multiply dimensions");

    matrix result(rows, other.cols);

    dim3 block(16, 16);

    dim3 grid(
        (other.cols + block.x - 1) / block.x,
        (rows + block.y - 1) / block.y);

    matmul_kernel<<<grid, block>>>(
        data,
        other.data,
        result.data,
        rows,
        cols,
        other.cols);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        throw std::runtime_error(cudaGetErrorString(err));
    }

    cudaDeviceSynchronize();

    return result;
}

matrix matrix::hadamard(const matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("Size mismatch in Hadamard multiply");

    matrix result(rows, cols);

    int size = rows * cols;

    int threads = GPU_THREADS;
    int blocks = (size + threads - 1) / threads;

    hadamard_kernel<<<blocks, threads>>>(
        data,
        other.data,
        result.data,
        size);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        throw std::runtime_error(cudaGetErrorString(err));
    }

    cudaDeviceSynchronize();

    return result;
}

matrix matrix::transpose() const
{
    matrix result(cols, rows);

    dim3 block(16, 16);

    dim3 grid(
        (cols + block.x - 1) / block.x,
        (rows + block.y - 1) / block.y);

    transpose_kernel<<<grid, block>>>(
        data,
        result.data,
        rows,
        cols);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        throw std::runtime_error(cudaGetErrorString(err));
    }

    cudaDeviceSynchronize();

    return result;
}

matrix matrix::sigmoid() const
{
    matrix result(rows, cols);

    int size = rows * cols;

    int threads = GPU_THREADS;

    int blocks =
        (size + threads - 1) / threads;

    sigmoid_kernel<<<blocks, threads>>>(
        data,
        result.data,
        size);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        throw std::runtime_error(cudaGetErrorString(err));
    }

    cudaDeviceSynchronize();

    return result;
}

matrix matrix::sigmoid_derivative() const
{
    matrix result(rows, cols);

    int size = rows * cols;

    int threads = GPU_THREADS;

    int blocks =
        (size + threads - 1) / threads;

    sigmoid_derivative_kernel<<<blocks, threads>>>(
        data,
        result.data,
        size);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        throw std::runtime_error(cudaGetErrorString(err));
    }

    cudaDeviceSynchronize();

    return result;
}

matrix matrix::relu() const
{
    matrix result(rows, cols);

    int size = rows * cols;

    int threads = GPU_THREADS;

    int blocks =
        (size + threads - 1) / threads;

    relu_kernel<<<blocks, threads>>>(
        data,
        result.data,
        size);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        throw std::runtime_error(cudaGetErrorString(err));
    }

    cudaDeviceSynchronize();

    return result;
}

//
// other
//
matrix make_matrix(std::initializer_list<double> values)
{
    matrix m(values.size(), 1);

    int i = 0;

    for (double v : values)
    {
        m.set(i, 0, v);
        i++;
    }

    return m;
}

matrix one_hot(int index, int size)
{
    if (index < 0 || index >= size)
        throw std::out_of_range("one_hot index out of range");

    matrix m(size, 1);

    m.set(index, 0, 1.0);

    return m;
}