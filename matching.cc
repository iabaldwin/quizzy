#include <array>
#include <cmath>
#include <iostream>
#include <numeric>

#define SQUARE(n) pow(n,2)

constexpr int kPatchSize = 5;
typedef std::array<std::array<float, kPatchSize>, kPatchSize> patch_t;
constexpr patch_t referencePatch = {{{0.13562204,0.27552594,0.35980544,0.94924713,0.34222606},
                                                                                   {0.94931411,0.27460949,0.75677189,0.24251008,0.50926451},
                                                                                   {0.97589259,0.46464161,0.52612093,0.95127515,0.37320958},
                                                                                   {0.10335494,0.22910981,0.44470971,0.28351396,0.65846204},
                                                                                   {0.94594718,0.52795235,0.98536625,0.09142491,0.99676918}}};

constexpr patch_t patchCandidate1 = {{{0.65051287,0.0858077 ,0.62365821,0.93836648,0.50158388},
                                                                                    {0.82328337,0.82281573,0.05730619,0.44645271,0.96427283},
                                                                                    {0.82971349,0.80185606,0.96409221,0.98732598,0.28638456},
                                                                                    {0.07682913,0.26305113,0.75847402,0.80500383,0.28722148},
                                                                                    {0.20879353,0.24539516,0.71978387,0.66240491,0.08076461}}};

constexpr patch_t patchCandidate2 = {{{ 0.13290402,0.96803355,0.8971992 ,0.76052644,0.30922267},
                                                                                     {0.30115749,0.78518142,0.30251533,0.69725331,0.58646293},
                                                                                     {0.03114041,0.15132543,0.60837695,0.18235618,0.74499181},
                                                                                     {0.19450344,0.93216069,0.5751807 ,0.38489764,0.5703268},
                                                                                     {0.92990664,0.22307124,0.63934838,0.38695049,0.21440734}}};
// Reference material
// http://ttic.uchicago.edu/~gregory/thesis/thesisChapter6.pdf
float similarity(patch_t a, patch_t b)
{
  // pixel base difference
  /*
  float similarity_sum;
  for(int h=0;h<kPatchSize;++h)
  {
    for(int w=0;w<kPatchSize;++w)
    {

      float difference = a[h][w] - b[h][w];
      //abs
      if(difference<0) difference = -difference;
      similarity_sum += 1.0 - difference;

    }
  }
  return similarity_sum;
  */

  // correlation
  float mean_a = 0;
  float mean_b = 0;
  for(int h = 0; h < kPatchSize; ++h)
  {
    for(int w = 0; w < kPatchSize; ++w)
    {
      mean_a += a[h][w];
      mean_b += b[h][w];
    }
  }
  float stddev_a = 0;
  float stddev_b = 0;
  float sum_of_products = 0;
  for(int h = 0; h < kPatchSize; ++h)
  {
    for(int w = 0; w < kPatchSize; ++w)
    {
      stddev_a += SQUARE(a[h][w] - mean_a);
      stddev_b += SQUARE(b[h][w] - mean_b);
      float difference_prod = (a[h][w] - mean_a) * (b[h][w] - mean_b);
      // abs
      if(difference_prod < 0) difference_prod = -difference_prod;
      sum_of_products += difference_prod;
    }
  }
  stddev_a /= SQUARE(kPatchSize);
  stddev_b /= SQUARE(kPatchSize);
  return sum_of_products / stddev_a / stddev_b;
}

int main() {
  /*
   *Print out the similarity between the reference patch, and candidate 1
   */
  std::cout << similarity(referencePatch, patchCandidate1) << std::endl;
  /*
   *Print out the similarity between the reference patch and candidate 2
   */
  std::cout << similarity(referencePatch, patchCandidate2) << std::endl;
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
}
