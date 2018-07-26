#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "polynomial_model.h"

constexpr int kDimensions = 2;
constexpr int kDataPoints = 20;
typedef std::array<float, kDimensions> Point2d;
constexpr std::array<Point2d, kDataPoints> points =
    {{{3.8686992545234347, 1.3559682753431659},
      {0.5180993375153686, 5.117486709676035},
      {3.5386436080165806, 1.5271125008781592},
      {4.728401339990676, 1.114753166423772},
      {9.023721046916886, 4.338066212680383},
      {0.01162719298319903, 6.076772652356936},
      {5.991635210658819, 1.2966680782036724},
      {6.065836085221114, 1.3272013121118933},
      {8.116481067645926, 3.0424908489990976},
      {3.4596129148699015, 1.5745584744071195},
      {3.311476661626893, 1.6702222128461326},
      {5.877037623043535, 1.2538389984467715},
      {1.292978422219696, 3.8484017956257546},
      {4.721539277238149, 1.11550807482421},
      {8.506311207767968, 3.5588436571438553},
      {0.6057636006285494, 4.961862706712194},
      {7.104743838802512, 1.985989325395428},
      {4.479413884955205, 1.1542019806354862},
      {3.0128605724511037, 1.8897446209038709},
      {1.9357873494944466, 2.9778798335036534}}};

// Returns the residuals (the difference between the dependent data value and
// the model's estimate) as an array in the same order as the data set.
//
// TODO(b/future): Residuals are useful for any model, not just polynomials. As
// additional models are added, this function could take a Model as an input and
// let new models derive from the base Model.
std::array<float, kDataPoints> residuals(
  const std::array<Point2d, kDataPoints>& points,
  const PolynomialModel& model) {
  std::array<float, kDataPoints> differences;
  std::transform(points.begin(), points.end(), differences.begin(),
                 [&model](const Point2d& point) {
                   return point[1] - model.evaluate(point[0]); 
                 });
  return differences;
}

// Returns the sum of the squared errors (SSE) in a model's estimate:
//
//   sum( (y_data - model(x_data))^2 ).
//
// The SSE is used commonly in statistics for estimating the variance of the
// error, the likelihood, model fit, etc.
float sumOfSquaredErrors(const std::array<Point2d, kDataPoints>& points,
                         const PolynomialModel& model) {
  std::array<float, kDataPoints> errors = residuals(points, model);
  std::array<float, kDataPoints> squared_errors;
  std::transform(errors.begin(), errors.end(), squared_errors.begin(),
                 [](float error) { return error * error; });
  return std::accumulate(squared_errors.begin(), squared_errors.end(), 0.0f);
}

// Returns the relative Akaike Information Criterion (AIC) value for a model
// with normally-distributed errors. Lower AIC indicates that the model fits the
// data better without adding undue complexity (eg, without overfitting). The
// AIC calculated here leaves out constant terms which don't affect its use as
// a comparison tool between potential models.
float AkaikeInformationCriterion(float sum_of_squared_errors,
                                 const PolynomialModel& model) {
  float estimated_variance = sum_of_squared_errors / kDataPoints;
  // +1 is because the AIC is measuring a model which includes an error term.
  int number_of_model_parameters = model.coefficients().size() + 1;
  float aic = 2.0f * number_of_model_parameters + 
      kDataPoints * std::log(estimated_variance);
  return aic;
}

int main() {
  // A model which fits the data exactly. The coefficients were found using a
  // least-squares fit.
  std::vector<float> model_coefficients{0.2, -2, 6.1};
  PolynomialModel model(model_coefficients);
  std::cout << "Polynomial model is: p(x) = " << model.asString() << "\n";
  float sse = sumOfSquaredErrors(points, model);
  std::cout << "SSE = " << sse << "\n";
  std::cout << "AIC = " << AkaikeInformationCriterion(sse, model) << "\n";

  // A model which doesn't fit the data as well will have a larger SSE and a
  // higher AIC.
  PolynomialModel model2(std::vector<float>({0.2001, -2, 6.1}));
  float sse2 = sumOfSquaredErrors(points, model2);
  std::cout << "AIC for model2 = " << AkaikeInformationCriterion(sse2, model2)
            << "\n";

  // A model which has more complexity but which doesn't result in lower error
  // will have a higher AIC.
  PolynomialModel model3(std::vector<float>({0, 0.2, -2, 6.1}));
  std::cout << "AIC for model3 = " << AkaikeInformationCriterion(sse, model3) 
            << "\n";
}
