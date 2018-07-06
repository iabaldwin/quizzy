#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <limits> //for epsilon to compare floats

constexpr int kPatchSize = 5;
constexpr int64_t kPatchSizeSqr = std::pow(kPatchSize, 2);

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

constexpr std::array<std::array<float, kPatchSize>, kPatchSize> patchCandidate2 = {{{ 0.13290402,0.96803355,0.8971992 ,0.76052644,0.30922267},
                                                                                     {0.30115749,0.78518142,0.30251533,0.69725331,0.58646293},
                                                                                     {0.03114041,0.15132543,0.60837695,0.18235618,0.74499181},
                                                                                     {0.19450344,0.93216069,0.5751807 ,0.38489764,0.5703268},
                                                                                     {0.92990664,0.22307124,0.63934838,0.38695049,0.21440734}}};
namespace images_comparison {

enum ComparisonAlgorithm {
    COMP_SSD = 0,
    COMP_NORM_CORR //Pearson's metric
};

///basic metric for images comparison
float imagesDiffSSD(const std::array<std::array<float, kPatchSize>, kPatchSize>& patch0
        , const std::array<std::array<float, kPatchSize>, kPatchSize>& patch1) {
    float diff(0.f);
    for(int iY = 0; iY < kPatchSize; ++iY) {
        for(int iX = 0; iX < kPatchSize; ++iX) {
            diff += std::pow(patch0[iY][iX] - patch1[iY][iX], 2);
        }
    }
    return std::sqrt(diff);
}

///Improved metric to handle brightness levels - Pearson's metric
float imagesDiffNormalizedCorr(const std::array<std::array<float, kPatchSize>, kPatchSize>& patch0
        , const std::array<std::array<float, kPatchSize>, kPatchSize>& patch1) {
    float patch0Mean(0.f);
    float patch1Mean(0.f);
    for(int iY = 0; iY < kPatchSize; ++iY) {
        for(int iX = 0; iX < kPatchSize; ++iX) {
            patch0Mean += patch0[iY][iX];
            patch1Mean += patch1[iY][iX];
        }
    }
    patch0Mean /= kPatchSizeSqr;
    patch1Mean /= kPatchSizeSqr;

    float dotsProduct(0.f);
    float patch0Norm(0.f);
    float patch1Norm(0.f);
    for(int iY = 0; iY < kPatchSize; ++iY) {
        for(int iX = 0; iX < kPatchSize; ++iX) {
            float patch0PixelAdj = patch0[iY][iX] - patch0Mean;
            float patch1PixelAdj = patch1[iY][iX] - patch1Mean;
            dotsProduct += (patch0PixelAdj) * (patch1PixelAdj);
            patch0Norm += std::pow(patch0PixelAdj, 2);
            patch1Norm += std::pow(patch1PixelAdj, 2);
        }
    }
    float patchesNormProd = std::sqrt(patch0Norm) * std::sqrt(patch1Norm);
    dotsProduct = dotsProduct / patchesNormProd;
    return dotsProduct;
}

//Choose which algorithm to use and calculate according patches difference
float imagesDiff(const std::array<std::array<float, kPatchSize>, kPatchSize>& patch0
        , const std::array<std::array<float, kPatchSize>, kPatchSize>& patch1, const ComparisonAlgorithm& algo) {
    switch(algo) {
    case COMP_SSD:
        return imagesDiffSSD(patch0, patch1);

    case COMP_NORM_CORR:
    default:
        return imagesDiffNormalizedCorr(patch0, patch1);
    }
}

///return 0 in case patches marks are the equally close using "algo" metric;
///1 in case fist patch(patch0) is closer; 2 in case second patch(patch1) is closer
int compareDiff(const float& patch0Diff, const float& patch1Diff, const ComparisonAlgorithm& algo) {
    if(std::fabs(patch0Diff - patch1Diff) < std::numeric_limits<float>::epsilon()) {
        return 0;
    }

    switch(algo) {
    case COMP_SSD: //for SSD 0 is a perfect match and bigger numbers is worse match
        return patch0Diff > patch1Diff ? 2 : 1;

    case COMP_NORM_CORR: //for normalized cross corr 1 is a perfect match and -1 is opposite to perfect
    default:
        return ((1.f - patch0Diff) > (1.f - patch1Diff)) ? 2 : 1;
    }
}

} //images_comparison

namespace ic = images_comparison;

int main() {
    ic::ComparisonAlgorithm comparisonMethod(ic::COMP_NORM_CORR);
    float patch1Diff = ic::imagesDiff(referencePatch, patchCandidate1, comparisonMethod);
    float patch2Diff = ic::imagesDiff(referencePatch, patchCandidate2, comparisonMethod);
    int closerPatch = ic::compareDiff(patch1Diff, patch2Diff, comparisonMethod);
    std::cout << "Using selected comparison method distances(measure of closeness) are:" << std::endl;
    std::cout << "patch1 dist is: " << patch1Diff << std::endl;
    std::cout << "patch2 dist is: " << patch2Diff << std::endl;
    if(closerPatch == 0) {
        std::cout << "Both patches are equally close to the reference patch using current comparison metric" << std::endl;
    } else {
        std::cout << "Patch" << closerPatch << " is closer to the reference patch" << std::endl;
    }

    return 0;
}
