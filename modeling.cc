#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <valarray>

#include "Matrix.h"

constexpr int kDimensions = 2;
constexpr int kDataPoints = 20;
using FloatArray = std::array<std::array<float, kDimensions>, kDataPoints>;
constexpr FloatArray points = {{{3.8686992545234347, 1.3559682753431659},
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

Matrix arrayToMatrix(const FloatArray &iArr) {
    int dataPoints = iArr.size();
    int dimensions = iArr.at(0).size();
    Matrix data(dataPoints, dimensions);

    for (Index i = 0; i < data.getRowsN(); i++) {
        for (Index j = 0; j < data.getColsN(); j++) {
            data(i, j) = iArr.at(i).at(j);
        }
    }

    return data;
}

std::vector<float> model(const FloatArray &iArr, int order) {
    if (order < 1)
        throw std::invalid_argument("Order must be greater than 0");

    Matrix data(arrayToMatrix(iArr));
    int dataPointsN = data.getRowsN();

    // Measurement matrix
    Matrix y(dataPointsN, 1);
    y.col(0) = data.ccol(1);

    // Regressors
    Matrix x(dataPointsN, order + 1);
    x.col(0) = std::valarray<NumericType>(1, dataPointsN);
    for (int o = 1; o <= order; o++) {
        x.col(o) = std::pow(data.ccol(0), static_cast<NumericType>(o));
    }

    // Parameter vector computation
    Matrix beta = (((x.transposed().dot(x)).inverse()).dot(x.transposed())).dot(y);

    std::vector<float> coeffs;
    std::for_each(std::begin(beta.getData()), std::end(beta.getData()),
                  [&coeffs](auto c) { coeffs.push_back(c); });

    return coeffs;
}

int main() {
    /*
     *Use a model of your choice to approximate the relationship between the
     independent/dependent variable pairs above. Model choice is up to you - just
     print out coefficients!
     */
    for (auto coeff : model(points, 2)) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;
    /*
     *Note: this is a suggested function signature, feel free to implement
     whatever you see fit!
     */
}
