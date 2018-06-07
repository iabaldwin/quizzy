#include <array>
#include <cmath>
#include <iostream>
#include <numeric>

constexpr int kPatchSize = 5;

constexpr std::array<std::array<float, kPatchSize>, kPatchSize> referencePatch = {{{0.13562204,0.27552594,0.35980544,0.94924713,0.34222606},
                                                                                   {0.94931411,0.27460949,0.75677189,0.24251008,0.50926451},
                                                                                   {0.97589259,0.46464161,0.52612093,0.95127515,0.37320958},
                                                                                   {0.10335494,0.22910981,0.44470971,0.28351396,0.65846204},
                                                                                   {0.94594718,0.52795235,0.98536625,0.09142491,0.99676918}}};

constexpr std::array<std::array<float, kPatchSize>, kPatchSize> patchCandidate1 = {{{0.65051287,0.0858077 ,0.62365821,0.93836648,0.50158388},
                                                                                    {0.82328337,0.82281573,0.05730619,0.44645271,0.96427283},
                                                                                    {0.82971349,0.80185606,0.96409221,0.98732598,0.28638456},
                                                                                    {0.07682913,0.26305113,0.75847402,0.80500383,0.28722148},
                                                                                    {0.20879353,0.24539516,0.71978387,0.66240491,0.08076461}}};

constexpr std::array<std::array<float, kPatchSize>, kPatchSize> patchCandidate2 = {{ {0.13290402,0.96803355,0.8971992 ,0.76052644,0.30922267},
                                                                                     {0.30115749,0.78518142,0.30251533,0.69725331,0.58646293},
                                                                                     {0.03114041,0.15132543,0.60837695,0.18235618,0.74499181},
                                                                                     {0.19450344,0.93216069,0.5751807 ,0.38489764,0.5703268},
                                                                                     {0.92990664,0.22307124,0.63934838,0.38695049,0.21440734}}};

// The similarity is measured by correlation coefficient:
// https://en.wikipedia.org/wiki/Pearson_correlation_coefficient
float similarity(const std::array<std::array<float, kPatchSize>, kPatchSize> &r,
                 const std::array<std::array<float, kPatchSize>, kPatchSize> &c) {
    float s_r, s_c, s_r2, s_c2, s_rc;
    s_r = s_c = s_r2 = s_c2 = s_rc;
    for (int i = 0; i < kPatchSize; i++) {
        for (int j = 0; j < kPatchSize; j++) {
            s_rc += r[i][j]*c[i][j];
            s_r += r[i][j];
            s_c += c[i][j];
            s_r2 += r[i][j]*r[i][j];
            s_c2 += c[i][j]*c[i][j];
        }
    }
    int n = kPatchSize*kPatchSize;
    float s = (n*s_rc - s_r*s_c) / sqrt((n*s_r2-s_r*s_r)*(n*s_c2-s_c*s_c));
    
    return s;
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
