/** Description: Let's make a cache friendly matrix multiplication and see how much improvement we can get over the naive implementation
 *
 */

#include "Timer/Timer.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

struct Size
{
    size_t rows, columns;
};

class Matrix
{
private:
    float *data;
    size_t rows;
    size_t columns;

public:
    Matrix(size_t row, size_t col) : rows(row), columns(col), data(new float[row * col]) { std::fill(data, data + row * col, 0.0f); }
    Matrix(Matrix &&m) noexcept : data(m.data), rows(m.rows), columns(m.columns)
    {
        m.data = nullptr;
        puts("moved");
    }
    Matrix(Matrix const &) = default;
    Matrix &operator=(Matrix &&m) = default;
    Matrix &operator=(Matrix const &) = default;
    ~Matrix() { delete[] data; }
    [[nodiscard]] float &operator()(size_t row, size_t col) { return data[col + row * columns]; }
    [[nodiscard]] const float &operator()(size_t row, size_t col) const { return data[col + row * columns]; }
    [[nodiscard]] size_t get_rows() const { return rows; }
    [[nodiscard]] size_t get_columns() const { return columns; }
    [[nodiscard]] Size get_size() const { return {rows, columns}; };
    [[nodiscard]] Matrix transpose() const
    {
        Timer t{true};
        Matrix temp{columns, rows};
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < columns; ++j)
                temp(j, i) = (*this)(i, j);
        }
        return temp;
    }
    static Matrix make_random_matrix(size_t row, size_t col)
    {
        Matrix m{row, col};
        std::generate(m.data, m.data + row * col, [] { return static_cast<float>(rand()) / RAND_MAX; });
        return m;
    }
    static Matrix make_test_matrix(size_t row, size_t col)
    {
        Matrix m{row, col};
        std::generate(m.data, m.data + row * col, [] { static auto i = 0; return static_cast<float>(i++); });
        return m;
    }
    [[nodiscard]] auto cbegin() const { return data; }
    friend std::ostream &operator<<(std::ostream &os, Matrix const &m)
    {
        os << '[';
        auto iter = m.data;
        for (size_t i = 0; i < m.rows; ++i)
        {
            os << '[';

            std::copy_n(iter, m.columns, std::ostream_iterator<float>{os, ", "});
            os << "]\n";
            std::advance(iter, m.columns);
        }
        os << "]\n";
        return os;
    }
    class MatrixMultiplicationException : std::exception
    {
    public:
        virtual const char *what() const noexcept override
        {
            return "Matrix size mismatch!";
        }
    };
};

/**
 * @brief: The naive matrix multiplication
*/
[[nodiscard]] Matrix naive_mul(Matrix const &l, Matrix const &r)
{
    if (l.get_columns() != r.get_rows())
        throw Matrix::MatrixMultiplicationException{};
    Matrix m{l.get_rows(), r.get_columns()};
    for (size_t i = 0; i < m.get_rows(); ++i)
    {
        for (size_t j = 0; j < m.get_columns(); ++j)
        {
            //m[i, j] = Sum(l[i, 0-k] * r[0-k, j]) , where k == l.get_columns()
            for (size_t k = 0; k < l.get_columns(); ++k)
                m(i, j) += l(i, k) * r(k, j);
        }
    }
    return m;
}

/**
 * @brief: First transpose the right-hand-side matrix, then do the multiplication
 */
[[nodiscard]] Matrix transpose_and_mul(Matrix const &l, Matrix const &r)
{
    if (l.get_columns() != r.get_rows())
        throw Matrix::MatrixMultiplicationException{};
    const auto r_transposed = r.transpose();
    
    Matrix m{l.get_rows(), r.get_columns()};
    //If we only measured the time without transpose
    {
        Timer t{true};
        auto l_iter = l.cbegin();

        for (size_t i = 0; i < m.get_rows(); ++i)
        {
            auto r_transposed_iter = r_transposed.cbegin();
            for (size_t j = 0; j < m.get_columns(); ++j)
            {
                //m[i, j] = Sum(l[i, 0-k] * r[0-k, j]) = Sum(l[i, 0-k] * r_T[j, 0-k]), where k == l.get_columns()
                //There is a function for that in STD library
                m(i, j) = std::inner_product(l_iter, l_iter + l.get_columns(), r_transposed_iter, 0.0f);
                std::advance(r_transposed_iter, r_transposed.get_columns());
            }
            std::advance(l_iter, l.get_columns());
        }
    }
    return m;
}

/**
 * @brief: Block multiplication, divide l,r,result matrices into blocks (block_size * block_size) and compute the result block by block
 * @param block_size: The dimension of the block
*/
Matrix block_mul(Matrix const &l, Matrix const &r, size_t block_size)
{
    if (l.get_columns() != r.get_rows())
        throw Matrix::MatrixMultiplicationException{};
    Matrix m{l.get_rows(), r.get_columns()};

    const size_t n = l.get_columns();
    for (size_t i = 0; i < n; i += block_size)
    {
        for (size_t j = 0; j < n; j += block_size)
        {
            //One block: m[i~(i+B), j~(j+B)]
            for (size_t k = 0; k < n; k += block_size)
            {
                /*block mini matrix multiplication*/
                for (auto i1 = i; i1 < i + block_size; ++i1)
                {
                    for (auto j1 = j; j1 < j + block_size; ++j1)
                    {
                        for (auto k1 = k; k1 < k + block_size; ++k1)
                            m(i1, j1) += l(i1, k1) * r(k1, j1);
                    }
                }
            }
        }
    }
    return m;
}

/**
 * @brief: First cache the corresponding block into an array, then do block multiplication similar as above
*/
Matrix cache_block_mul(Matrix const &l, Matrix const &r, size_t block_size)
{
    if (l.get_columns() != r.get_rows())
        throw Matrix::MatrixMultiplicationException{};
    Matrix m{l.get_rows(), r.get_columns()};

    const size_t n = l.get_columns();

    //Allocate 2 blocks of size (B*B), one for l, another for r
    std::vector<float> l_block(block_size * block_size);
    std::vector<float> r_block(block_size * block_size);

    for (size_t i = 0; i < n; i += block_size)
    {
        for (size_t j = 0; j < n; j += block_size)
        {
            for (size_t k = 0; k < n; k += block_size)
            {
                //copy l[i~(i+B), j~(j+B)] -> l_block
                //copy r[]
                //copy m[]
                auto l_block_iter = l_block.begin();
                auto r_block_iter = r_block.begin();
                for (size_t i_block = i; i_block < i + block_size; ++i_block)
                {
                    std::copy(&l(i_block, k), &l(i_block, k + block_size), l_block_iter);
                    std::advance(l_block_iter, block_size);
                }
                for (size_t j_block = k; j_block < k + block_size; ++j_block)
                {
                    std::copy(&r(j_block, j), &r(j_block, j + block_size), r_block_iter);
                    std::advance(r_block_iter, block_size);
                }
                for (auto i1 = i, i2 = 0ull; i1 < i + block_size; ++i1, ++i2)
                {
                    for (auto j1 = j, j2 = 0ull; j1 < j + block_size; ++j1, ++j2)
                    {
                        for (auto k1 = k, k2 = 0ull; k1 < k + block_size; ++k1, ++k2)
                            m(i1, j1) += l_block[i2 * block_size + k2] * r_block[k2 * block_size + j2];
                    }
                }
            }
        }
    }
    return m;
}
int main()
{
    auto l = Matrix::make_random_matrix(2000, 2000);
    auto r = Matrix::make_random_matrix(2000, 2000);
    {
        Timer t{true};
        auto result = transpose_and_mul(l, r);

    }
    {
        Timer t{true};
        auto result = block_mul(l, r, 5);
    }
    {
        Timer t{true};
        auto result = cache_block_mul(l, r, 5);
    }
    {
        Timer t{true};
        auto result = naive_mul(l, r);
    }
}
/*
Possible output (under GCC 9.3 -O3/ Windows)
6851830 ms.
6464032 ms.
8698440 ms.
18052420 ms.
*/