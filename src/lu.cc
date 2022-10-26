/**
 * @file lu.cc
 * @author peng lei (plhitsz@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-10-26
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "lu.h"

#include "field.h"
#include "matrix.h"
#include "swapper_matrix.h"

int OctetFiniteField_test(int a, int b) {
  OctetFiniteField ff;
  ff.add(1, 1);
  ff.sub(1, 1);
  ff.mul(1, 1);
  ff.div(1, 1);

  MatrixOctet mat(2, 2);
  mat << 1, 2, 3, 4;

  SwapperOctetMatrix test_mat;
  return 0;
}
