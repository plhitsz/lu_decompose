/**
 * @file lu.h
 * @author peng lei (plhitsz@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-10-26
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SRC_LU_H_
#define SRC_LU_H_

#include <tuple>

#include "field.h"
#include "matrix.h"
#include "swapper_matrix.h"

int OctetFiniteField_test(int a, int b);

namespace math {

using Permutation = std::vector<int>;

template <typename T>
std::tuple<MatrixOctet, SwapperOctetMatrix, Permutation> luDecomposition(
    const MatrixOctet& mat, const AlgebraicField<T>& af);
/**
 * @brief Check if the LU decomposition is ready to solve. Not to be confused
 * with checking the sanity of LU. This function ASSUMES LU from a valid lu
 * decomposition and only checks if the decomposition is ready to solve
 * (full rank).
 *
 * @param l the lower triangular matrix from LU decomposition.
 * @param u the upper triangular matrix from LU decomposition.
 * @return true
 * @return false
 */
bool isValidLU(const MatrixOctet& l, const SwapperOctetMatrix& u);

}  // namespace math
#endif  // SRC_LU_H_
