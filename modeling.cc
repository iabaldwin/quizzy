#include <iostream>
#include <cmath>
#include <numeric>
#include <array>

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


std::pair<float, float> getLinearRegressionCoeffs(std::array<std::array<float, kDimensions>, kDataPoints> data_points) {
    float x_sum = std::accumulate(data_points.begin(), data_points.end(), 0.0f, [](auto lhs, const auto rhs) {return lhs + rhs[0];});
    float y_sum = std::accumulate(data_points.begin(), data_points.end(), 0.0f, [](auto lhs, const auto rhs) {return lhs + rhs[1];});
    float x_mean = x_sum / static_cast<float>(kDataPoints);
    float y_mean = y_sum / static_cast<float>(kDataPoints);

    float xy_covariance = 0.0f;
    float x_variance = 0.0f;
    float x, y;
    for (auto data_point : data_points) {
        x = data_point[0];
        y = data_point[1];
        x_variance += std::pow(x - x_mean, 2);
        xy_covariance += ((y - y_mean) * (x - x_mean));
    }

    float beta = xy_covariance / x_variance;
    float alpha = y_mean - (beta * x_mean);

    return std::pair<float, float>(alpha, beta);
}

float getLinearRegressionMse(std::array<std::array<float, kDimensions>, kDataPoints> data_points, std::pair<float, float> coeffs) {
    float mse = 0.0f;
    float total_data_points_f = static_cast<float>(data_points.size());
    float pred, label, x;

    for (auto data_point : data_points) {
        x = data_point[0];
        label = data_point[1];

        pred = coeffs.first + (x * coeffs.second);
        mse += std::pow(pred - label, 2) / total_data_points_f;
        std::cout << "pred = " << pred << ", label = " << label << std::endl;
    }

    return mse;
}

int main() {
  /*
   *Use a model of your choice to approximate the relationship between the
   independent/dependent variable pairs above. Model choice is up to you - just
   print out coefficients!
   */
  auto lr_coeffs = getLinearRegressionCoeffs(points);
  auto mse = getLinearRegressionMse(points, lr_coeffs);
  std::cout << "alpha = " << lr_coeffs.first << ", beta = " << lr_coeffs.second << ", mse = " << mse << std::endl;
}
