#include <array>
#include <vector>
#include <cmath>
#include <iostream>
#include <numeric>
#include <cassert>
#include <algorithm>
#include <functional>

constexpr int kPatchSize = 5;

template<typename NumericType, size_t N>
using Matrix = std::array<std::array<NumericType, N>, N>;

template<size_t N>
using FloatMatrix = Matrix<float, N>;

using Patch = FloatMatrix<kPatchSize>;

constexpr Patch referencePatch = {{{0.13562204, 0.27552594, 0.35980544, 0.94924713, 0.34222606},
                                          {0.94931411, 0.27460949, 0.75677189, 0.24251008, 0.50926451},
                                          {0.97589259, 0.46464161, 0.52612093, 0.95127515, 0.37320958},
                                          {0.10335494, 0.22910981, 0.44470971, 0.28351396, 0.65846204},
                                          {0.94594718, 0.52795235, 0.98536625, 0.09142491, 0.99676918}}};

constexpr Patch patchCandidate1 = {{{0.65051287, 0.0858077, 0.62365821, 0.93836648, 0.50158388},
                                           {0.82328337, 0.82281573, 0.05730619, 0.44645271, 0.96427283},
                                           {0.82971349, 0.80185606, 0.96409221, 0.98732598, 0.28638456},
                                           {0.07682913, 0.26305113, 0.75847402, 0.80500383, 0.28722148},
                                           {0.20879353, 0.24539516, 0.71978387, 0.66240491, 0.08076461}}};

constexpr Patch patchCandidate2 = {{{0.13290402, 0.96803355, 0.8971992, 0.76052644, 0.30922267},
                                           {0.30115749, 0.78518142, 0.30251533, 0.69725331, 0.58646293},
                                           {0.03114041, 0.15132543, 0.60837695, 0.18235618, 0.74499181},
                                           {0.19450344, 0.93216069, 0.5751807, 0.38489764, 0.5703268},
                                           {0.92990664, 0.22307124, 0.63934838, 0.38695049, 0.21440734}}};

template<typename NumericType>
void flatten(const Patch &iMat, std::vector<NumericType> &oVec) {
    oVec.reserve(iMat.size() * iMat[0].size());

    std::for_each(iMat.begin(), iMat.end(), [&oVec](auto &aRow) {
        oVec.insert(oVec.end(), aRow.begin(), aRow.end());
    });
}

template<typename NumericType>
NumericType mean(const std::vector<NumericType> &iVec) {
    using namespace std;

    return accumulate(iVec.begin(), iVec.end(), 0.0) / iVec.size();
}

template<typename NumericType>
NumericType standardDev(const std::vector<NumericType> &iVec) {
    using namespace std;

    auto vecMean = mean(iVec);
    auto sum = inner_product(iVec.begin(), iVec.end(), iVec.begin(), 0.0,
                             plus<NumericType>(),
                             [&](const auto a, const auto b) { return (a - vecMean) * (a - vecMean); });
    return sqrt(sum / iVec.size());
}

//template<typename NumericType>
//NumericType l2_norm(const std::vector<NumericType> &iVecX, const std::vector<NumericType> &iVecY) {
//    using namespace std;
//
//    NumericType sum = inner_product(iVecX.begin(), iVecX.end(), iVecY.begin(), 0.0,
//                                    plus<float>(),
//                                    [](const auto a, const auto b) {
//                                        return std::pow((a - b), 2);
//                                    });
//
//    return sqrt(sum);
//}
//
//template<typename NumericType>
//NumericType linf_norm(const std::vector<NumericType> &iVecX, const std::vector<NumericType> &iVecY) {
//    using namespace std;
//
//    return inner_product(iVecX.begin(), iVecX.end(), iVecY.begin(), 0.0,
//                         [](const double a, const double b) { return max(a, b); },
//                         [](const double a, const double b) { return abs(a - b); });
//}

template<typename NumericType>
NumericType zncc(const std::vector<NumericType> &iVecX, const std::vector<NumericType> &iVecY) {
    using namespace std;

    auto n = iVecX.size();
    auto xMean = mean(iVecX);
    auto yMean = mean(iVecY);
    auto xStd = standardDev(iVecX);
    auto yStd = standardDev(iVecY);

    auto sum = inner_product(iVecX.begin(), iVecX.end(), iVecY.begin(), 0.0,
                             plus<NumericType>(),
                             [&](const auto a, const auto b) { return (a - xMean) * (b - yMean); });

    return sum / (n * xStd * yStd);
}

double similarity(const Patch &iPatchRef, const Patch &iPatchCand) {
    // flatten matrix into vector
    std::vector<double> refVec, candVec;
    flatten(iPatchRef, refVec);
    flatten(iPatchCand, candVec);

    // Assert matching sizes
    assert(refVec.size() == candVec.size());

    auto similarity = zncc<double>(refVec, candVec);

    return similarity;
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
