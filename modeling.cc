#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <limits>

constexpr int kDimensions = 2;
constexpr int kDataPoints = 20;
constexpr std::array<std::array<float, kDimensions>, kDataPoints> points = {{{3.8686992545234347, 1.3559682753431659},
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

namespace modeling {
//get predicted value for independent based on polynom coefficients
float getPredictedValue(const std::vector<float>& coeffs, const float& independentVal) {
    float predicted(0.f);
    int maxPolyPower = coeffs.size() - 1;
    for(int iX = 0; iX < coeffs.size(); ++iX) {
        predicted += coeffs[iX] * std::pow(independentVal, maxPolyPower - iX);
    }
    return predicted;
}

//sum of residuals is not very useful due to sign for each residual, sqrt of sum of squares used instead
float getResidualError(const std::vector<float>& coeffs) {
    float error(0.f);
    for(int iP = 0; iP < kDataPoints; ++iP) {
        error += std::pow(points[iP][1] - getPredictedValue(coeffs, points[iP][0]), 2);
    }
    error = std::sqrt(error);
    return error;
}

//use polynomial regression to get coefficients for polynom of selected max power
int fitPolyCoeffs(const int& polyPower, std::vector<float>& coeffs) {
    //calculate normal(augmented) matrix
    int matrixHeight = polyPower + 1;
    int matrixWidth = polyPower + 2;
    std::vector<std::vector<float> > matrix(matrixHeight, std::vector<float>(matrixWidth, 0));
    for(int iY = 0; iY < matrixHeight; ++iY) {
        for(int iP = 0; iP < kDataPoints; ++iP) {
            for(int iX = 0; iX < matrixWidth - 1; ++iX) { //this loop for all but one columns 
                matrix[iY][iX] += std::pow(points[iP][0], iY + iX);
            }
            matrix[iY][matrixWidth - 1] += points[iP][1] * std::pow(points[iP][0], iY);
        }
    }
    //solve equasion: invert, normalize row and zero-out column
    for(int iY = 0; iY < matrixHeight; ++iY) {
        float diagElem = matrix[iY][iY];
        for(int iX = 0; iX < matrixWidth; ++iX) {
            matrix[iY][iX] /= diagElem;
        }
        for(int y = 0; y < matrixHeight; ++y) {
            if(y == iY) {
                continue;
            }
            float pivot = matrix[y][iY];
            for(int x = 0; x < matrixWidth; ++x) {
                matrix[y][x] -= pivot * matrix[iY][x];
            }
        }
    }

    for(int iC = polyPower; iC >= 0; --iC) {
        coeffs.push_back(matrix[iC][polyPower + 1]);
    }

    return 0;
}

//compare errors for polynoms of different max power and select optimal coefficients
int findOptimalCoeffs(const int& maxPolyPower, std::vector<float>& coeffs) {
    float bestError = std::numeric_limits<float>::max();
    for(int iP = 0; iP <= maxPolyPower; ++iP) {
        std::vector<float> currPowerCoeffs;
        currPowerCoeffs.reserve(iP);
        fitPolyCoeffs(iP, currPowerCoeffs);
        float currError = getResidualError(currPowerCoeffs);
        if(currError < bestError) {
            bestError = currError;
            coeffs = currPowerCoeffs;
        }
    }
    return 0;
}
int printCoeffs(const std::vector<float>& coeffs) {
    std::cout.precision(16);
    std::cout << std::fixed;
    int maxPolyPower = coeffs.size() - 1;
    for(int iX = 0; iX < coeffs.size(); ++iX) {
        std::cout << std::fabs(coeffs[iX]);
        if(iX != maxPolyPower) {
            std::cout << " x^" << (maxPolyPower - iX);
        }
        if(iX != coeffs.size() - 1) {
            if(coeffs[iX + 1] >= 0.) {
                std::cout << " + ";
            } else {
                std::cout << " - ";
            }
        }
    }
    std::cout << std::endl;
}
} //modeling

int main() {
    std::vector<float> coeffs;
    int polyPower = 6;
    modeling::findOptimalCoeffs(polyPower, coeffs);
    modeling::printCoeffs(coeffs);
    std::cout << "Error is: " << modeling::getResidualError(coeffs) << std::endl;
    return 0;
}
