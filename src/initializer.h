/**
 * @file initializer.h
 * @author peng.lei
 * @brief Comma Initializer
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SRC_INITIALIZER_H_
#define SRC_INITIALIZER_H_

template <typename Mat>
struct CommaInitializer {
  typedef typename Mat::value_type Scalar;
  inline CommaInitializer(Mat& xpr, const Scalar& s)
      : xpr_(xpr), row_(0), col_(1) {
    xpr_[0][0] = s;
  }

  CommaInitializer& operator,(const Scalar& s) {
    if (col_ >= xpr_.Width()) {
      row_++;
      col_ = 0;
    }
    xpr_[row_][col_++] = s;
    return *this;
  }

  Mat& xpr_;
  int row_;  // current row id
  int col_;  // current col id
};

#endif  // SRC_INITIALIZER_H_
