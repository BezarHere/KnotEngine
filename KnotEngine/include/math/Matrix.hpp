#pragma once
#include "Vector.hpp"

namespace kt
{
  template <typename T, size_t W, size_t H = W>
  struct TMatrix
  {
    typedef T value_type;
    typedef TMatrix<T, W, H> this_type;

    static constexpr size_t width = W;
    static constexpr size_t height = H;

    static constexpr bool is_square = (width == height);

    typedef value_type row_type[width];
    typedef value_type column_type[height];

    typedef column_type values_array[width];

    template <size_t W2, size_t H2>
    using ResizedMatrix = TMatrix<T, W2, H2>;

    inline column_type &operator[](const size_t index) {
      return values[index];
    }

    inline const column_type &operator[](const size_t index) const {
      return values[index];
    }

    template <size_t W2, size_t H2>
    inline ResizedMatrix<W2, H> operator*(const ResizedMatrix<W2, H2> &other) const;

    inline this_type operator+(const this_type &other) const;

    inline value_type determinant() const;

    inline this_type transposed() noexcept {
      static_assert(is_square, "only squared matrices can be transposed");

      // full copy
      this_type result = *this;

      // transpose
      for (uint32_t y = 0; y < (height - 1); y++)
      {
        for (uint32_t x = y + 1; x < width; x++)
        {
          result[x][y] = this->values[y][x];
          result[y][x] = this->values[x][y];
        }
      }

      // copy diagonals
      //for (uint32_t i = 0; i < width; i++)
      //{
      //  result[i][i] = this->values[i][i];
      //}

      return result;
    }

    inline this_type &convert_transposed() noexcept {
      return *this = this->transposed();
    }

    inline void scale(const value_type scaler) {
      for (size_t x = 0; x < width; x++)
      {
        for (size_t y = 0; y < height; y++)
        {
          values[x][y] *= scaler;
        }
      }
    }

    values_array values;
  };

  typedef TMatrix<real_t, 2, 2> Matrix2;
  typedef TMatrix<real_t, 3, 3> Matrix3;
  typedef TMatrix<real_t, 4, 4> Matrix4;

  template<typename T, size_t W, size_t H>
  template<size_t W2, size_t H2>
  inline TMatrix<T, W2, H> TMatrix<T, W, H>::operator*(const ResizedMatrix<W2, H2> &other) const {
    ResizedMatrix<W2, H> result = {};

    for (size_t y = 0; y < H; y++)
    {
      for (size_t x = 0; x < W2; x++)
      {
        for (size_t i = 0; i < W; i++)
        {
          result[x][y] += this->values[i][y] * other[x][i];
        }
      }
    }

    return result;
  }

  template<typename T, size_t W, size_t H>
  inline TMatrix<T, W, H> TMatrix<T, W, H>::operator+(const this_type &other) const {
    this_type result = *this;

    for (size_t x = 0; x < width; x++)
    {
      for (size_t y = 0; y < height; y++)
      {
        result[x][y] += other[x][y];
      }
    }

    return result;
  }

  template<>
  inline real_t TMatrix<real_t, 3, 3>::determinant() const {
    return
      (values[0][0] * values[1][1] * values[2][2])
      + (values[1][0] * values[2][1] * values[0][2])
      + (values[2][0] * values[0][1] * values[1][2])
      - (values[2][0] * values[1][1] * values[0][2])
      - (values[1][0] * values[0][1] * values[2][2])
      - (values[0][0] * values[2][1] * values[1][2]);
  }

  template<>
  inline real_t TMatrix<real_t, 2, 2>::determinant() const {
    return values[0][0] * values[1][1] - values[1][0] * values[0][1];
  }
}