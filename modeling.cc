#include <iostream>
#include <cmath>
#include <array>
#include <vector>

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


void model(const std::array<std::array<float, kDimensions>, kDataPoints> &points) {


  // Quadratic Polynomial regression
  // borrowed from https://gist.github.com/chrisengelsma/108f7ab0a746323beaaf7d6634cf4add
  const u_int x_index = 0;
  const u_int y_index = 1;
  int n = 2;  // 2nd order, quadratic 
  int np1 = n + 1;
  int np2 = n + 2;
  int tnp1 = 2 * n + 1;
  float tmp;

  // sigma(xi^2n)
  std::vector<float> X(tnp1);
  for (int i = 0; i < tnp1; ++i) {
    X[i] = 0;
    for (int j = 0; j < kDataPoints; ++j)
      X[i] += (float)pow(points[j][x_index], i);
  }

  // a = vector to store final coefficients.
  std::vector<float> a(np1);

  // B = normal augmented matrix that stores the equations.
  std::vector<std::vector<float> > B(np1, std::vector<float> (np2, 0));

  for (int i = 0; i <= n; ++i) 
    for (int j = 0; j <= n; ++j) 
      B[i][j] = X[i + j];

  // Y = vector to store values of sigma(xi^n * yi)
  std::vector<float> Y(np1);
  for (int i = 0; i < np1; ++i) {
    Y[i] = (float)0;
    for (int j = 0; j < kDataPoints; ++j) {
      Y[i] += (float)pow(points[j][x_index], i)*points[j][y_index];
    }
  }

  // Load values of Y as last column of B
  for (int i = 0; i <= n; ++i) 
    B[i][np1] = Y[i];

  n += 1;
  int nm1 = n-1;

  // Pivotisation of the B matrix.
  for (int i = 0; i < n; ++i) 
    for (int k = i+1; k < n; ++k) 
      if (B[i][i] < B[k][i]) 
        for (int j = 0; j <= n; ++j) {
          tmp = B[i][j];
          B[i][j] = B[k][j];
          B[k][j] = tmp;
        }

  // Performs the Gaussian elimination.
  // (1) Make all elements below the pivot equals to zero
  //     or eliminate the variable.
  for (int i=0; i<nm1; ++i)
    for (int k =i+1; k<n; ++k) {
      float t = B[k][i] / B[i][i];
      for (int j=0; j<=n; ++j)
        B[k][j] -= t*B[i][j];         // (1)
    }

  // Back substitution.
  // (1) Set the variable as the rhs of last equation
  // (2) Subtract all lhs values except the target coefficient.
  // (3) Divide rhs by coefficient of variable being calculated.
  for (int i=nm1; i >= 0; --i) {
    a[i] = B[i][n];                   // (1)
    for (int j = 0; j<n; ++j)
      if (j != i)
        a[i] -= B[i][j] * a[j];       // (2)
    a[i] /= B[i][i];                  // (3)
  }

  std::vector<float> coeffs(a.size());
  for (size_t i = 0; i < a.size(); ++i) 
    coeffs[i] = a[i];


  std::cout << "y = " << coeffs[2] << "x^2 + "<< coeffs[1] << "x + " << coeffs[0] << std::endl;

  // compute error
  float error = 0;
  for(int i = 0; i < kDataPoints; ++i) {
    float y = (coeffs[2] * pow(points[i][x_index], 2)) + (coeffs[1] * pow(points[i][x_index], 1)) + coeffs[0];
    error += abs(y - points[i][y_index])/points[i][y_index];
  }

  error = error/kDataPoints;
  std::cout << "error: " << error << std::endl;
}


int main() {
  /*
   *Use a model of your choice to approximate the relationship between the
   independent/dependent variable pairs above. Model choice is up to you - just
   print out coefficients!
   */
  //std::cout << model(points) << std::endl;
  model(points);

  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
}
