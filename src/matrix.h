/**
 * @file matrix.h
 * @author Lei Peng
 * @brief Matrix class for storing arithmetic value
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SRC_MATRIX_H_
#define SRC_MATRIX_H_

#include <assert.h>

#include <iostream>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "initializer.h"
#include "types.h"

#define CACHELINE_SIZE (64)

template <typename V,
          typename std::enable_if<std::is_arithmetic<V>::value, bool>::type = 0>
class Matrix {
 public:
  using value_type = V;
  Matrix() {}
  Matrix(int height, int width) : height_(height), width_(width) {
    data_.resize(height_);
    for (int i = 0; i < height_; i++) {
      // Since `V` is restricted to be arithmetic, resize will init with zero
      data_[i] = std::vector<V>(width_);
    }
  }
  explicit Matrix(std::vector<std::vector<V>>&& vec) {
    data_ = vec;
    height_ = vec.size();
    width_ = vec[0].size();
  }
  explicit Matrix(const std::vector<std::vector<V>>& vec) {
    data_ = vec;
    height_ = vec.size();
    width_ = vec[0].size();
  }
  virtual ~Matrix() {}
  const int Height() const { return height_; }
  const int Width() const { return width_; }
  void reserve(int size) { data_.reserve(size); }
  void clear() {
    std::vector<std::vector<V>>().swap(data_);
    height_ = 0;
    width_ = 0;
  }
  const std::vector<V> GetRow(int i) const { return data_[i]; }
  const std::vector<V> GetCol(int i) const {
    std::vector<V> res(height_);
    for (int r = 0; r < height_; r++) {
      res.push_back(data_[r][i]);
    }
    return res;
  }
  void hTruncate(int new_height) {
    assert(new_height <= height_ &&
           "Matrix: cannot truncate to height greater than original");
    data_.resize(new_height);
    data_.shrink_to_fit();
    height_ = new_height;
  }
  void wTruncate(int new_width) {
    assert(new_width <= width_ &&
           "Matrix: cannot truncate to width greater than original");
    for (auto& v : data_) {
      v.resize(new_width);
      v.shrink_to_fit();
    }
    width_ = new_width;
  }
  Matrix<V> ithRowMatrix(int i) { return Matrix({(*this)[i]}); }
  virtual std::vector<V>& operator[](int i) {
    assert(i < height_ && "Matrix: Index out of range");
    return data_[i];
  }
  virtual const std::vector<V>& operator[](int i) const {
    assert(i < height_ && "Matrix: Index out of range");
    return data_[i];
  }
  friend inline bool operator==(const Matrix<V>& a, const Matrix<V>& b) {
    if (a.Height() != b.Height() || a.Width() != b.Width()) {
      return false;
    }
    if (a.data_ != b.data_) {
      return false;
    }
    return true;
  }

  friend std::ostream& operator<<(std::ostream& os, const Matrix<V>& mat) {
    os << std::endl << "---------" << std::endl;
    os << "-- height " << mat.Height() << " width " << mat.Width() << std::endl;
    for (int r = 0; r < mat.Height(); r++) {
      os << "[";
      os.width(2);
      os.fill(' ');
      os << r;
      os << "]";
      for (int w = 0; w < mat.Width(); w++) {
        if (std::is_same<unsigned char, V>::value) {
          os.width(3);
          os.fill(' ');
          os << static_cast<int>(mat[r][w]) << " ";
        } else {
          os.width(3);
          os.fill(' ');
          os << (mat[r][w]) << " ";
        }
      }
      if (r == mat.Height() - 1) {
        os << std::endl << "---------";
      }
      os << std::endl;
    }
    os << std::endl;
    return os;
  }
  CommaInitializer<Matrix<V>> operator<<(const V& s) {
    return CommaInitializer<Matrix<V>>(*static_cast<Matrix<V>*>(this), s);
  }

  void AddRow(const octetVec& row) {
    if (height_ == 0) {
      width_ = row.size();
    } else if (width_ != row.size()) {
      throw std::runtime_error("cant inser a row with a different width");
    }
    data_.push_back(row);
    height_++;
  }

  void AddRow(octetVec&& row) {
    if (height_ == 0) {
      width_ = row.size();
    } else if (width_ != row.size()) {
      throw std::runtime_error("cant inser a row with a different width");
    }
    data_.push_back(std::move(row));
    height_++;
  }
  void SwapRows(int i, int j) { std::swap(data_[i], data_[j]); }
  std::vector<std::vector<V>> takeData() { return std::move(data_); }

  void dumpEquations(const Matrix<V>& y, const std::vector<int>& vars) {
    std::cout << std::endl << "---------Equations Print --------";
    std::cout << "height " << this->Height() << " width " << this->Width()
              << std::endl;
    assert(vars.size() == this->Width());
    assert(y.Height() == this->Height());
    std::cout << "Var:";
    for (int i = 0; i < vars.size(); i++) {
      std::string tittle = "x" + std::to_string(vars[i]);
      std::cout.width(3);
      std::cout.fill(' ');
      std::cout << tittle << " ";
    }
    std::cout << std::endl;
    for (int r = 0; r < this->Height(); r++) {
      std::cout << "[";
      std::cout.width(2);
      std::cout.fill(' ');
      std::cout << r;
      std::cout << "]";
      for (int w = 0; w < this->Width(); w++) {
        if (std::is_same<unsigned char, V>::value) {
          std::cout.width(3);
          std::cout.fill(' ');
          std::cout << static_cast<int>((*this)[r][w]) << " ";
        } else {
          std::cout.width(3);
          std::cout.fill(' ');
          std::cout << ((*this)[r][w]) << " ";
        }
        if (this->Width() - 1 == w) {
          std::cout << " = " << y[r];
        }
      }
      if (r == this->Height() - 1) {
        std::cout << std::endl
                  << "---------Equations Print --------" << std::endl;
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

 protected:
  int height_ = 0;
  int width_ = 0;
  alignas(CACHELINE_SIZE) std::vector<std::vector<V>> data_;
};

using MatrixOctet = Matrix<octet>;

#endif  // SRC_MATRIX_H_
