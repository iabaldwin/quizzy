#include <array>
#include <cmath>
#include <iostream>
#include <numeric>

constexpr int kPatchSize = 5;

typedef std::array<float, kPatchSize> PatchRow;
typedef std::array<PatchRow, kPatchSize> Patch;

constexpr Patch referencePatch = {{{0.13562204,0.27552594,0.35980544,0.94924713,0.34222606},
                                   {0.94931411,0.27460949,0.75677189,0.24251008,0.50926451},
                                   {0.97589259,0.46464161,0.52612093,0.95127515,0.37320958},
                                   {0.10335494,0.22910981,0.44470971,0.28351396,0.65846204},
                                   {0.94594718,0.52795235,0.98536625,0.09142491,0.99676918}}};

constexpr Patch patchCandidate1 = {{{0.65051287,0.0858077 ,0.62365821,0.93836648,0.50158388},
                                    {0.82328337,0.82281573,0.05730619,0.44645271,0.96427283},
                                    {0.82971349,0.80185606,0.96409221,0.98732598,0.28638456},
                                    {0.07682913,0.26305113,0.75847402,0.80500383,0.28722148},
                                    {0.20879353,0.24539516,0.71978387,0.66240491,0.08076461}}};

constexpr Patch patchCandidate2 = {{{ 0.13290402,0.96803355,0.8971992 ,0.76052644,0.30922267},
                                    {0.30115749,0.78518142,0.30251533,0.69725331,0.58646293},
                                    {0.03114041,0.15132543,0.60837695,0.18235618,0.74499181},
                                    {0.19450344,0.93216069,0.5751807 ,0.38489764,0.5703268},
                                    {0.92990664,0.22307124,0.63934838,0.38695049,0.21440734}}};

// Functor which returns the squared difference between two values.
struct QuadraticLoss {
  float operator()(float x, float y) {
    float difference = x - y;
    return difference * difference;
  }
};

// Functor which gives the absolute difference between two values.
struct AbsoluteLoss {
  float operator()(float x, float y) {
    return fabs(x - y);
  }
};

// Functor which returns the difference between two values, moderated by a Huber
// loss function to reduce the influence of outliers. Huber has a quadratic loss
// for small errors, and a linear loss for large errors.
struct HuberLoss {
  // The parameter 'c' gives the threshold above which the loss function behaves
  // linearly instead of quadratically. A value of c = 1.34*sigma, where sigma
  // is the expected standard deviation of the errors, tends to work OK in the
  // absence of specialized tuning.
  HuberLoss(float c) : c(c), half_c_squared(0.5 * c * c) {}

  float operator()(float x, float y) {
    float error = x - y;
    if (fabs(error) <= c)
      return 0.5 * error * error;
    return fabs(error) * c - half_c_squared;
  }

  const float c;
  const float half_c_squared;
};

// Returns the similarity between two patches. The similarity is computed as the
// sum of an error function between each pair of corresponding pixels in each
// patch. The error function is expected to take two floats and return a float
// describing the difference.
//
// Note: "Similarity" will depend on the use case. This implementation is for
// a point-wise similarity comparison using whatever loss function makes sense
// for the application. The choice of a functor allows the compiler to inline
// the functor as needed, and lets the user build a functor with internal
// parameters.
template <typename BinaryFunc>
float similarity(const Patch& patch1,
                 const Patch& patch2,
                 BinaryFunc& binary_func) {
  float accumulator = 0;
  for (int i = 0; i < patch1.size(); ++i) {
    for (int j = 0; j < patch1[i].size(); ++j) {
      accumulator += binary_func(patch1[i][j], patch2[i][j]);
    }
  }
  return accumulator;
}

int main() {
  QuadraticLoss quadratic_loss;
  float patch1_quadratic =
    similarity(referencePatch, patchCandidate1, quadratic_loss);
  float patch2_quadratic =
    similarity(referencePatch, patchCandidate2, quadratic_loss);
  std::cout << "Using a quadratic measure (smaller is better):" << std::endl;
  std::cout << "  Ref<->Patch1 has similarity of "
            << patch1_quadratic << std::endl;
  std::cout << "  Ref<->Patch2 has similarity of " 
            << patch2_quadratic << std::endl << std::endl;

  AbsoluteLoss absolute_loss;
  float patch1_absolute =
    similarity(referencePatch, patchCandidate1, absolute_loss);
  float patch2_absolute =
    similarity(referencePatch, patchCandidate2, absolute_loss);
  std::cout << "Using an absolute measure (smaller is better):" << std::endl;
  std::cout << "  Ref<->Patch1 has similarity of "
            << patch1_absolute << std::endl;
  std::cout << "  Ref<->Patch2 ahs similarity of "
            << patch2_absolute << std::endl << std::endl;
  
  // We can also estimate the standard deviation of the errors using what's been
  // computed so far; it's not perfect (sample vs population and only for one
  // reference), but it gives us a starting point.
  int num_samples = 2 * kPatchSize * kPatchSize;
  float expected_squares_of_errors =
      (patch1_quadratic + patch2_quadratic) / num_samples;
  float mean_of_errors = (patch1_absolute + patch2_absolute) / num_samples;
  float estimated_standard_deviation =
      sqrt(expected_squares_of_errors - mean_of_errors * mean_of_errors);
  std::cout << "An estimate for the standard deviation of the errors is "
            << estimated_standard_deviation << std::endl;
  HuberLoss huber_loss(estimated_standard_deviation);
  std::cout << "Using a Huber loss-moderated measure (smaller is better):"
            << std::endl;
  std::cout << "  Ref<->Patch1 has similarity of " 
            << similarity(referencePatch, patchCandidate1, huber_loss) 
            << std::endl;
  std::cout << "  Ref<->Patch2 has similarity of " 
            << similarity(referencePatch, patchCandidate2, huber_loss)
            << std::endl;
}
