#include <vector>
#include <algorithm>

float computeWeights(const std::vector<int> &values, std::vector<float> &weights) {
    int maxVal = *std::max_element(values.begin(), values.end());
    int minVal = *std::min_element(values.begin(), values.end());

    float weightSum = 0;

    for (int i = 0; i < values.size(); i++) {
        if (maxVal == minVal) {
            weights[i] = 1.0 / values.size();
        } else {
            weights[i] = (values[i] - minVal) / static_cast<float>(maxVal - minVal);
        }
        weightSum += weights[i];
    }

    return weightSum;
}