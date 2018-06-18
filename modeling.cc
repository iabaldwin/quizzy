#include <iostream>
#include <cmath>
#include <array>
#include "eigen-git-mirror\Eigen\Dense"
using namespace std;

using namespace Eigen;

constexpr int kDimensions = 2;
constexpr int kDataPoints = 20;
constexpr std::array<std::array<float, kDimensions>, kDataPoints> points =
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


int main() {
    // https://math.stackexchange.com/questions/426048/find-the-least-square-solution-for-the-best-parabola

    // I first plotted the points in a scatterplot.
    // It was clear that the points followed a quadratic fit.

    MatrixXd A(kDataPoints,3);
    MatrixXd B(kDataPoints,1);

    for (int i = 0; i < kDataPoints; i++)
    {
        A.row(i) << points[i][0] * points[i][0], points[i][0],1;
        B.row(i) << points[i][1];
    }

    auto At = A.transpose();
    auto AtA = At * A;
    auto invAtA = AtA.inverse();

    auto Atb = At * B;

    auto equation = invAtA * Atb;

    cout <<"Best fit equation:"<< equation.row(0) << "(x^2) + " << equation.row(1) << "(x) + " << equation.row(2) << endl;

    auto residuals = B - A * equation;
    double meanResidual = residuals.cwiseAbs().sum() / kDataPoints;
    cout << "Mean Residual:" << meanResidual << endl;
    double meanSqResidual = (residuals * residuals.transpose()).sum() / kDataPoints;
    cout << "Mean Square Residual:" << meanSqResidual << endl;
}
