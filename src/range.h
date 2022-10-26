/**
 * @file range.h
 * @author plhitsz@outlook.com
 * @brief
 * @version 0.1
 * @date 2022-10-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SRC_RANGE_H_
#define SRC_RANGE_H_
#include <iostream>
#include <limits>
#include <ostream>
#include <type_traits>
#include <vector>

#if __GNUC__ >= 3
#define likely(x) (__builtin_expect((x), 1))
#define unlikely(x) (__builtin_expect((x), 0))
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

template <typename T,
          typename std::enable_if<std::is_arithmetic<T>::value, bool>::type = 0>
class range_iterator {
 public:
  range_iterator(T value, T step) : value_{value}, step_{step} {}
  bool operator!=(const range_iterator& other) {
    return (step_ == other.step_ && (step_ > 0 && value_ < other.value_ ||
                                     step_ < 0 && value_ > other.value_));
  }
  bool operator==(const range_iterator& other) { return !operator!=(other); }

  range_iterator& operator++() {
    value_ += step_;
    return *this;
  }
  T& operator*() { return value_; }
  const T& operator*() const { return value_; }

 private:
  T value_, step_;
};

// limit type to int, double, float ...
template <typename T,
          typename std::enable_if<std::is_arithmetic<T>::value, bool>::type = 0>
class Range {
 public:
  Range() : start_{0}, stop_{0}, step_{0} {}
  explicit Range(T stop) : start_{0}, stop_{stop}, step_{1} {}
  Range(T start, T stop, T step = 1) : start_{start}, stop_{stop}, step_{step} {
    int cnt = count();
    cnt = cnt > 0 ? cnt : 1;
    T checkvalue = start_ + (cnt - 1) * step_;
    if (unlikely(step_ > 0 &&
                 checkvalue > std::numeric_limits<T>::max() - step_)) {
      throw std::runtime_error("Risk of out of range");
    }
    if (unlikely(step_ < 0 &&
                 checkvalue < std::numeric_limits<T>::min() - step_)) {
      throw std::runtime_error("Risk of out of range");
    }
  }
  range_iterator<T> begin() { return range_iterator<T>(start_, step_); }
  range_iterator<T> end() { return range_iterator<T>(stop_, step_); }
  const range_iterator<T> begin() const {
    return range_iterator<T>(start_, step_);
  }
  const range_iterator<T> end() const {
    return range_iterator<T>(stop_, step_);
  }
  bool operator==(const Range& other) const {
    return (start_ == other.start_ && stop_ == other.stop_ &&
            step_ == other.step_);
  }
  friend std::ostream& operator<<(std::ostream& os, const Range<T>& r) {
    os << "start " << r.start_;
    os << " stop " << r.stop_;
    os << " step " << r.step_;
    return os;
  }

  std::vector<T> as_vec() {
    std::vector<T> vec = std::vector<T>();
    vec.reserve(count());
    for (auto i : (*this)) {
      vec.push_back(i);
    }
    return vec;
  }
  // only for step =1
  bool contains(const Range<T>& other) const {
    return (step_ == 1 && start_ <= other.start() && stop_ >= other.stop());
  }

  bool contains(const T v) const {
    /* Check if v inside the bounds */
    if (v < start_ || v >= stop_) return false;

    /* Check if v skipped by step_ */
    if ((v - start_) % step_ == 0) return true;
    return false;
  }
  /**
   * @brief Counting the number of elements in the range
   *
   * @return int
   */
  int count() const { return step_ ? (stop_ - start_) / step_ : 0; }
  const T start() const { return start_; }
  const T stop() const { return stop_; }
  const T step() const { return step_; }

 private:
  T start_, stop_, step_;
};

template <typename T>
struct rCompare {
  bool operator()(const Range<T>& lhs, const Range<T>& rhs) const {
    return lhs.stop() <= rhs.start();
  }
};

#endif  // SRC_RANGE_H_
