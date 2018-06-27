#include <iostream>
#include <cmath>
#include <array>
#include <Eigen/Geometry>

constexpr int kDimensions = 2;
constexpr int kDataPoints = 20;
constexpr std::array<std::array<float, kDimensions>, kDataPoints> POINTS = {{
    {3.8686992545234347, 1.3559682753431659},
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
    {1.9357873494944466, 2.9778798335036534}
}};

/**
 * @brief Fit a polynomial to a set of data points
 * @details Fit a polynomial to an array of data points returning the 
 * coefficients in an Eigen::VectorXf in the order a + bx + cx^2 + ... kx^n.
 * The vector will be of degree+1 length.
 * 
 * The polynomial fitting is solved by utilizing Eigen as a linear algebra 
 * library to compute the Cholesky decomposition (A^t Ax = A^t b) to solve 
 * Ax=b where the rows of A are the values of x from [1,x,x^2,...x^n] for each 
 * point and each element of b is the corresponding y for each point.
 * 
 * @param points Array of points to fit a model to
 * @param degree Polynomial degree to model
 * @return Vector of coefficients
 */
Eigen::VectorXf fit_polynomial(
    const std::array<std::array<float, kDimensions>, kDataPoints>& points,
    unsigned int degree)
{
    Eigen::MatrixXf A(points.size(), degree+1);
    Eigen::VectorXf b(points.size());
    
    for (unsigned int row = 0; row < points.size(); ++row){
        const std::array<float,kDimensions>& point = points[row];
        float x = point[0];
        float y = point[1];
        A(row,0) = 1;
        for (unsigned int col = 1; col < degree+1; ++col){
            A(row,col) = A(row,col-1) * x;
        }
        b(row) = y;
    }
    
    Eigen::VectorXf solution = (A.transpose()*A).ldlt().solve(A.transpose()*b);
    return solution;
}

/**
 * @brief Return the error as the residual sum of squares
 */
float error_residuals(
    const std::array<std::array<float, kDimensions>, kDataPoints>& points,
    const Eigen::VectorXf& coefficients)
{
    float sum = 0.0f;
    for (const std::array<float,kDimensions>& point : points){
        float value = coefficients(0);
        float x = point[0];
        for(int ii = 1; ii < coefficients.size(); ++ii){
            value += coefficients(ii)*x;
            x *= point[0];
        }
        float diff = point[1] - value;
        sum += diff*diff;
    }
    return sum;
}


int main() {
  Eigen::VectorXf coefficients = fit_polynomial(POINTS, 2);
  float error = error_residuals(POINTS,coefficients);
  std::cout<<"Coefficients: "<<coefficients.transpose()<<std::endl;
  std::cout<<"Error:         "<<error<<std::endl;
}
