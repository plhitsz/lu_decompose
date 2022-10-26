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

#include <algorithm>
namespace math {
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

template <typename T>
std::tuple<MatrixOctet, SwapperOctetMatrix, Permutation> LuDecomposition(
    const MatrixOctet& mat, const AlgebraicField<T>& af) {
  auto u = SwapperMatrix(mat);
  // std::cout << "LuDecomposition input mat " << mat;
  auto l = MatrixOctet(u.Height(), u.Height());

  int irow = 0;
  int icol = 0;
  while (irow < u.Height() && icol < u.Width()) {
    // find pivot and swap it on top
    for (int jrow : Range(irow, u.Height())) {
      // pivot found at location (jrow, icol).
      if (u[jrow][icol] != 0x00) {
        if (irow != jrow) {
          u.swapRows(irow, jrow);
          // diagonal of l is not loaded yet, swapping two rows doesn't affect
          // its diagonal
          l.SwapRows(irow, jrow);
        }
        break;
      }
    }

    // pivot moved to location (irow, icol)
    if (u[irow][icol] != 0x00) {
      // eliminating all pivot candidates below
      for (int jrow : Range(irow + 1, u.Height())) {
        auto e = af.div(u[jrow][icol], u[irow][icol]);

        // eliminating the `jrow`-th row, starting from the `icol`-th element.
        // because the previous elements are all zero.
        for (int jcol : Range(icol, u.Width())) {
          u[jrow][jcol] = af.sub(u[jrow][jcol], af.mul(u[irow][jcol], e));
        }

        l[jrow][irow] = e;
      }
      // pivot found in (irow, icol), moving to next row.
      irow++;
      icol++;
    } else {
      // pivot not found in icol
      icol++;
    }
  }
  auto n = std::min(mat.Height(), mat.Width());
  // loading l's diagonal
  for (int i : Range(n)) {
    l[i][i] = 1;
  }
  l.hTruncate(n);
  l.wTruncate(n);
  u.wTruncate(n);
  return {l, u, u.GetPermutation()};
}

bool isValidLU(const MatrixOctet& l, const SwapperOctetMatrix& u) {
  for (int i : Range(u.Width())) {
    if (u[i][i] == 0) {
      return false;
    }
  }
  return true;
}

}  // namespace math
