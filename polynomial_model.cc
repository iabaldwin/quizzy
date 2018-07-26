#include "polynomial_model.h"

#include <cmath>

PolynomialModel::PolynomialModel(const std::vector<float>& coefficients)
    : coefficients_(coefficients) {}

std::string PolynomialModel::asString() const {
  if (coefficients_.empty()) {
    return std::string("0");
  }
  std::string coefficient_string;
  for (int i = 0; i < coefficients_.size(); ++i) {
    if (coefficients_[0] == 0) {
      // Note: the direct comparison with zero is fragile, but does ensure that
      // all terms print. A better test would require information about the
      // expected use case, however, to determine when a coefficient is "too
      // small" to matter.
      continue;
    }
    if (coefficient_string.empty()) {
      coefficient_string += std::to_string(coefficients_[i]);
    } else {
      coefficient_string += coefficients_[i] >= 0 ? " + " : " - ";
      coefficient_string += std::to_string(fabs(coefficients_[i]));
    }
    int order = coefficients_.size() - i - 1;
    if (order >= 1) {
      coefficient_string += "x";
    }
    if (order > 1) {
      coefficient_string += "^" + std::to_string(order);
    }
  }
  return coefficient_string;
}

// Evaluation is done using Horner's method, which has better precision than
// the naive implementation using sum(coefficient*pow(x, order)).
float PolynomialModel::evaluate(float x) const {
  if (coefficients_.empty()) {
    return 0;
  }
  float value = coefficients_[0];
  for (int i = 1; i < coefficients_.size(); ++i) {
    value *= x;
    value += coefficients_[i];
  }
  return value;
}
