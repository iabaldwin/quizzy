// Note: Data models are common and reusable in multiple contexts. They also
// tend to have common interfaces. I opted to write the polynomial model as if
// it were derived from a Model class, something like
//
//   class Model {
//    public:
//     std::string asString() const = 0;
//     float evaluate(float x) const = 0;
//     const std::vector<float>& coefficients() const = 0;
//   };
//
// ...but opted not to include the base class here for the sake of clarity, and
// because there's not a clear need for polymorphism in the use case.

#ifndef POLYNOMIAL_MODEL_H_
#define POLYNOMIAL_MODEL_H_

#include <string>
#include <vector>

class PolynomialModel {
 public:
  // The coefficients are in descending order of powers. For example, the
  // polynomial 
  //
  //   p(x) = 3x^2 + 2.5x - 1
  //
  // would be represented by std::vector<float> coefficients{3, 2.5, -1}. An
  // empty vector of coefficients is allowed, but evaluates to zero.
  PolynomialModel(const std::vector<float>& coefficients);

  // Returns the polynomial as a pretty-printed string, and using 'x' as the
  // argument.
  std::string asString() const;

  // Evaluates the polynomial, p(x), for a specific argument value, x.
  float evaluate(float x) const;

  const std::vector<float>& coefficients() const { return coefficients_; }

 private:
  const std::vector<float> coefficients_;
};

#endif  // POLYNOMIAL_MODEL_H_