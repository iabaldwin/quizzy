#include <iostream>
#include <cmath>
#include <array>

using std::array;

constexpr int kDimensions = 2;
constexpr int kDataPoints = 20;
constexpr int n_quad_param = 3;
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
// Simple version of 3x3 matrix inversion
int inverse_3x3(const array<array<float, 3>, 3> &src,
                 array<array<float, 3>, 3> &dst) {
    float det = +src[0][0]*(src[1][1]*src[2][2]-src[2][1]*src[1][2])
                -src[0][1]*(src[1][0]*src[2][2]-src[1][2]*src[2][0])
                +src[0][2]*(src[1][0]*src[2][1]-src[1][1]*src[2][0]);
    if (det == 0)
        return 1;
    
    double invdet = 1/det;
    dst[0][0] =  (src[1][1]*src[2][2]-src[2][1]*src[1][2])*invdet;
    dst[1][0] = -(src[0][1]*src[2][2]-src[0][2]*src[2][1])*invdet;
    dst[2][0] =  (src[0][1]*src[1][2]-src[0][2]*src[1][1])*invdet;
    dst[0][1] = -(src[1][0]*src[2][2]-src[1][2]*src[2][0])*invdet;
    dst[1][1] =  (src[0][0]*src[2][2]-src[0][2]*src[2][0])*invdet;
    dst[2][1] = -(src[0][0]*src[1][2]-src[1][0]*src[0][2])*invdet;
    dst[0][2] =  (src[1][0]*src[2][1]-src[2][0]*src[1][1])*invdet;
    dst[1][2] = -(src[0][0]*src[2][1]-src[2][0]*src[0][1])*invdet;
    dst[2][2] =  (src[0][0]*src[1][1]-src[1][0]*src[0][1])*invdet;
    
    return 0;
}

// I plot out the point distribution, and found that quadratic model is good enough to
// describe the relationship.
// So I will use second order of polynomial regression model:
// y = a0 + a1*x + a2*x^2
// The result will be 3x1 vector which are the coefficients: [a0, a1, a2];
// So the problem becomes to solve the linear equation of: Y = X*a, where
// Y = [y0, y1, ... yn], a = [a0, a1, a2],
// X = [1 x1 x1^2]
//     [1 x2 x2^2]
//     [...      ]
//     [1 x2 xn^2]
array<float, n_quad_param>
model(const std::array<std::array<float, kDimensions>, kDataPoints> points) {
    // Will not build X matrix directly in case too much memory
    // comsumption with big input of points; instead, I build A, B matrix
    // as below to solve this linear equation:
    // A = (Xt*X) = [n         sum(xi)   sum(xi^2)]
    //              [sum(xi)   sum(xi^2) sum(xi^3)]
    //              [sum(xi^2) sum(xi^3) sum(xi^4)]
    // B = (Xt*Y) = [sum(yi)     ]
    //              [sum(xi*yi)  ]
    //              [sum(xi^2*yi)]
    array<array<float, n_quad_param>, n_quad_param> A = {0.f};
    array<float, n_quad_param> B = {0};
    
    A[0][0] = kDataPoints;
    float x, x2, x3, x4, y;
    for (int i = 0; i < kDataPoints; i++) {
        x = points[i][0];
        x2 = x*x; x3 = x2*x; x4 = x2*x2;
        y = points[i][1];
        A[0][1] += x;
        A[0][2] += x2;
        A[1][2] += x3;
        A[2][2] += x4;
        
        B[0] += y;
        B[1] += x*y;
        B[2] += x2*y;
    }
    A[1][1] = A[0][2];
    for (int i = 0; i < n_quad_param; i++) {
        for (int j = i+1; j < n_quad_param; j++) {
            A[j][i] = A[i][j];
        }
    }
    
    // Get A inverse
    array<array<float, n_quad_param>, n_quad_param> A_inv;
    int ret = inverse_3x3(A, A_inv);
    if (ret) {
        std::cout << "observation matrix can not be inversed, program quits" << std::endl;
        exit(1);
    }
    
    // Get solution
    array<float, n_quad_param> X = {0.f};
    for (int i = 0; i < n_quad_param; i++) {
        for (int j = 0; j < n_quad_param; j++) {
            X[i] += A_inv[i][j]*B[j];
        }
    }
    
    // Get residual to see if it is small enough
    float sum_r2 = 0;
    for (int i = 0; i < kDataPoints; i++) {
        x = points[i][0];
        y = points[i][1];
        float dy = X[0] + X[1]*x + X[2]*x*x - y;
        sum_r2 += dy*dy;
    }

    std::cout << "Estimated parameters (a0, a1, a2): "
              << X[0] << ", " << X[1] << ", " << X[2] << std::endl;
    std::cout << "Sum of square of residual: " << sum_r2 << std::endl;

    
    return X;
}


int main() {
  /*
   *Use a model of your choice to approximate the relationship between the
   independent/dependent variable pairs above. Model choice is up to you - just
   print out coefficients!
   */
  //std::cout << model(points) << std::endl;
    array<float, n_quad_param> X = model(points);
    
    
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
}
