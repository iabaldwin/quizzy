#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <limits>

constexpr int kPatchSize = 5;

typedef std::array<std::array<float,kPatchSize>, kPatchSize> Patch;

#if 1
constexpr Patch referencePatch = {{
    {0.13562204,0.27552594,0.35980544,0.94924713,0.34222606},
    {0.94931411,0.27460949,0.75677189,0.24251008,0.50926451},
    {0.97589259,0.46464161,0.52612093,0.95127515,0.37320958},
    {0.10335494,0.22910981,0.44470971,0.28351396,0.65846204},
    {0.94594718,0.52795235,0.98536625,0.09142491,0.99676918}
}};

constexpr Patch patchCandidate1 = {{
    {0.65051287,0.0858077 ,0.62365821,0.93836648,0.50158388},
    {0.82328337,0.82281573,0.05730619,0.44645271,0.96427283},
    {0.82971349,0.80185606,0.96409221,0.98732598,0.28638456},
    {0.07682913,0.26305113,0.75847402,0.80500383,0.28722148},
    {0.20879353,0.24539516,0.71978387,0.66240491,0.08076461}
}};

constexpr Patch patchCandidate2 = {{
    {0.13290402,0.96803355,0.8971992 ,0.76052644,0.30922267},
    {0.30115749,0.78518142,0.30251533,0.69725331,0.58646293},
    {0.03114041,0.15132543,0.60837695,0.18235618,0.74499181},
    {0.19450344,0.93216069,0.5751807 ,0.38489764,0.5703268},
    {0.92990664,0.22307124,0.63934838,0.38695049,0.21440734}
}};

// Debugging patches that will result in a 1 and 0 similarity.
#else 
constexpr Patch referencePatch = {{
    {1,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0}
}};

constexpr Patch patchCandidate1 = {{
    {1,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0}
}};

constexpr Patch patchCandidate2 = {{
    {0,1,1,1,1},
    {1,1,1,1,1},
    {1,1,1,1,1},
    {1,1,1,1,1},
    {1,1,1,1,1}
}};
#endif

/**
 * @brief Return true if the values are almost equal
 * @see std::numeric_limits::epsilon
 */
template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y, int ulp=5)
{
    return std::abs(x-y) <= std::numeric_limits<T>::epsilon() 
        * std::abs(x+y) * ulp || std::abs(x-y) < std::numeric_limits<T>::min();
}

/**
 * @brief Return a value [0,1] as a measure of similarity between two patches
 * where 0 is dissimilar and 1 is similar
 * @details Return the similarity between two patches. Similar patches will have
 * values closer to 1 while dissimilar patches will have values closer to 0.
 * The measure of similarity is the normalized cross correlation of the image
 * patch compared to the reference. The output of normalized cross correlation
 * is adjusted such that value is in the range [0,1].
 * 
 * If the reference or candidate patch has no variance (i.e. it is a single 
 * value for all pixels), then this calculates the euclidean distance as the 
 * square root of the sum of squared differences and return the similarity as 
 * 1/exp(sqrt(SSD))
 * 
 * @param reference Reference Patch
 * @param candidate Candidate Patch
 * @returns float in the range [0,1] representing similarity.
 */
float similarity(const Patch& reference, const Patch& candidate)
{
    unsigned int rows = kPatchSize;
    unsigned int cols = kPatchSize;
    unsigned int num_pixels = rows*cols;
    
    float sum_ref = 0;
    float sum_can = 0;
    float avg_ref = 0;
    float avg_can = 0;
    float var_ref = 0;
    float var_can = 0;
    
    for (unsigned int row = 0; row < rows; ++row){
        for (unsigned int col = 0; col < cols; ++col){
            sum_ref += reference[row][col];
            sum_can += candidate[row][col];
        }
    }
    avg_ref = sum_ref / num_pixels;
    avg_can = sum_can / num_pixels;
    
    float sum_combined = 0;
    for (unsigned int row = 0; row < rows; ++row){
        for (unsigned int col = 0; col < cols; ++col){
            float value_ref = reference[row][col] - avg_ref;
            float value_can = candidate[row][col] - avg_can;
            var_ref += value_ref*value_ref;
            var_can += value_can*value_can;
            
            sum_combined += value_ref * value_can;
        }
    }
    var_ref = var_ref/num_pixels;
    var_can = var_can/num_pixels;
    
    float stddev_ref = sqrt(var_ref);
    float stddev_can = sqrt(var_can);
    
    float numerator = sum_combined;
    float denominator = (num_pixels * stddev_ref * stddev_can);
    
    // Divide by 0 protection. The issue here is that the denominator can be
    // zero if any of num_pixels, stddev_ref, or stddev_can are 0 meaning that
    // the patch size is 0 (0 pixels) or the images have no variance. If the
    // patches are a single value, then a different metric must be used; in this
    // case, sum of square differences.
    if (almost_equal(denominator,0.0f)){
        // Sum of squared differences
        float sum_square_diff = 0.0f;
        for (unsigned int row = 0; row < rows; ++row){
            for (unsigned int col = 0; col < cols; ++col){
                float diff = reference[row][col] - candidate[row][col];
                sum_square_diff += diff*diff;
            }
        }
        // Adjust distance to be a similarity function from 0-1
        float value = 1.0f/exp(sqrt(sum_square_diff));
        return value;
    } else {
        // Normalized cross correlation
        float value = numerator / denominator;
        
        // Adjust the value so that instead of [-1,1] it's [0,1]
        value = (value + 1.0f)/2.0f;
        
        return value;
    }
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
}
