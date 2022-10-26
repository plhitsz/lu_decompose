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

int OctetFiniteField_test(int a, int b) {
  OctetFiniteField ff;
  ff.add(1, 1);
  ff.sub(1, 1);
  ff.mul(1, 1);
  ff.div(1, 1);
  return 0;
}
