/**
 * @file swapper_matrix.h
 * @author peng lei (plhitsz@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-10-26
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SRC_SWAPPER_MATRIX_H_
#define SRC_SWAPPER_MATRIX_H_
#include <utility>
#include <vector>

#include "matrix.h"
#include "range.h"

template <typename V>
class SwapperMatrix : public Matrix<V> {
 public:
  explicit SwapperMatrix(const Matrix<V>& mat)
      : Matrix<V>(mat), perm_(Range(mat.Height()).as_vec()) {}
  SwapperMatrix() : Matrix<V>() {}

  std::vector<V>& operator[](int i) {
    assert(perm_[i] < this->height_);
    return this->data_[perm_[i]];
  }
  const std::vector<V>& operator[](int i) const {
    assert(perm_[i] < this->height_);
    return this->data_[perm_[i]];
  }
  std::vector<V> GetRow(int i) const { return this->data_[perm_[i]]; }
  std::vector<V> GetCol(int i) const { throw std::logic_error("unimpl!"); }
  void swapRows(int i, int j) { std::swap(perm_[i], perm_[j]); }
  const std::vector<int>& GetPermutation() const { return perm_; }

 private:
  alignas(CACHELINE_SIZE) std::vector<int> perm_;
};

using SwapperOctetMatrix = SwapperMatrix<octet>;

#endif