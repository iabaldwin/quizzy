#include <iostream>
#include <cmath>
#include <array>
#include <gsl/gsl_multifit.h>
 
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


void model(const std::array<std::array<float, kDimensions>, kDataPoints>& points) {
  double x_array[kDataPoints];
  double y_array[kDataPoints];
  for (size_t i = 0; i < kDataPoints; ++i) {
    x_array[i] = points[i][0];
    y_array[i] = points[i][1];
  }

  int degree = 3;
  double coeff[degree];

  gsl_multifit_linear_workspace *ws;
  gsl_matrix *cov, *X;
  gsl_vector *y, *c;
  double chisq;

  int i, j;

  X = gsl_matrix_alloc(kDataPoints, degree);
  y = gsl_vector_alloc(kDataPoints);
  c = gsl_vector_alloc(degree);
  cov = gsl_matrix_alloc(degree, degree);

  for(i=0; i < kDataPoints; i++) {
    for(j=0; j < degree; j++) {
      gsl_matrix_set(X, i, j, pow(x_array[i], j));
    }
    gsl_vector_set(y, i, y_array[i]);
  }

  ws = gsl_multifit_linear_alloc(kDataPoints, degree);
  gsl_multifit_linear(X, y, c, cov, &chisq, ws);

  for(i=0; i < degree; i++) {
    coeff[i] = gsl_vector_get(c, i);
  }

  gsl_multifit_linear_free(ws);
  gsl_matrix_free(X);
  gsl_matrix_free(cov);
  gsl_vector_free(y);
  gsl_vector_free(c);

  for (size_t i = 0; i < degree; ++i) {
    std::cout << coeff[i] << "\n";
  }
}

int main() {
  /*
   *Use a model of your choice to approximate the relationship between the
   independent/dependent variable pairs above. Model choice is up to you - just
   print out coefficients!
   */
  std::cout << "Fitting a quadratic curve to the data using GNU Scientific Library" << std::endl;
  std::cout << "Printing coefficients to the equation, y = ax**2 + bx + c" << std::endl;
  model(points);
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
}
