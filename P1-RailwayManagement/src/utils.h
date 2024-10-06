#ifndef DA_RAILWAYMANAGEMENT_UTILS_H
#define DA_RAILWAYMANAGEMENT_UTILS_H

#include <vector>

/**
 * Computes the weights for each value in the vector using a linear scaling
 * Time Complexity: O(n) being n the number of values
 * @param values Vector of values
 * @param weights Vector of weights
 * @return The sum of the weights to be used to compute the average
 */
float computeWeights(const std::vector<int> &values, std::vector<float> &weights);

#endif //DA_RAILWAYMANAGEMENT_UTILS_H
