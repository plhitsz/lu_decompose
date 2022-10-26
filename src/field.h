/**
 * @file feild.h
 * @author peng lei (plhitsz@outlook.com)
 * @brief define operations on algebraic number field
 * @version 0.1
 * @date 2022-10-26
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SRC_FIELD_H_
#define SRC_FIELD_H_

#include <type_traits>

#include "types.h"

template <typename V,
          typename std::enable_if<std::is_arithmetic<V>::value, bool>::type = 0>
class AlgebraicField {
 public:
  AlgebraicField() {}
  virtual ~AlgebraicField() {}
  virtual V add(V v1, V v2) const = 0;
  virtual V sub(V v1, V v2) const = 0;
  virtual V mul(V v1, V v2) const = 0;
  virtual V div(V v1, V v2) const = 0;
};

/**
 * @brief Finite Field
 *
 * @tparam V
 */
template <typename V>
class FiniteField : public AlgebraicField<V> {
 public:
  FiniteField() {}
  virtual ~FiniteField() {}
  virtual V add(V v1, V v2) const { return v1 ^ v2; }
  virtual V sub(V v1, V v2) const { return v1 ^ v2; }
  virtual V mul(V v1, V v2) const {
    // TODO(peng.lei): table based multiplication
    return 0;
  }
  virtual V div(V v1, V v2) const { return 0; }
};

// FiniteField based on unsigned char
using OctetFiniteField = FiniteField<octet>;

#endif  // SRC_FIELD_H_
