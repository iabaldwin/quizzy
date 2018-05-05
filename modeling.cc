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

// determinant of 3x3 matrix (row major)
float determinant(const std::array<float, 9> &a)
{
  //0 1 2
  //3 4 5
  //6 7 8

  return a[0] * a[4] * a[8] -
         a[0] * a[5] * a[7] -
         a[1] * a[3] * a[8] +
         a[1] * a[5] * a[6] +
         a[2] * a[3] * a[7] -
         a[2] * a[4] * a[6];
}


// minimize ||a * quad - rhs||_2
// a is stored in row major
void model(const std::array<std::array<float, kDimensions>, kDataPoints> &points, std::array<float, 3> &quad)
{
  std::vector<float> a(kDataPoints * 3);
  std::vector<float> rhs(kDataPoints);
  for (size_t i = 0; i < kDataPoints; i++){

    float x = points[i][0];
    float y = points[i][1];

    a[i * 3 + 0] = x * x; 
    a[i * 3 + 1] = x; 
    a[i * 3 + 2] = 1.0; 

    rhs[i] = y;
  }

  // compute symmetric aTa = aT * a
  std::array<float, 9> aTa = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  for (size_t i = 0; i < 3; i++)
    for (size_t j = 0; j < i + 1; j++)
      for (size_t k = 0; k < kDataPoints; k++)
        aTa[i * 3 + j] += a[k * 3 + i] * a[k * 3 + j]; 

  aTa[1] = aTa[3];
  aTa[2] = aTa[6];
  aTa[5] = aTa[7];

  // compute b = aT * rhs
  std::array<float, 3> b = {0.0, 0.0, 0.0};
  for (size_t i = 0; i < 3; i++)
      for (size_t k = 0; k < kDataPoints; k++)
        b[i] += a[k * 3 + i] * rhs[k];

  // solve aTa * quad = b 
  float det = determinant(aTa);

  for (size_t i = 0; i < 3; i++){
    std::array<float, 9> aTa_xyz = aTa;
    aTa_xyz[0 + i] = b[0];
    aTa_xyz[3 + i] = b[1];
    aTa_xyz[6 + i] = b[2];
    quad[i] = determinant(aTa_xyz) / det;
  }
}

int main() {
  /*
   *Use a model of your choice to approximate the relationship between the
   independent/dependent variable pairs above. Model choice is up to you - just
   print out coefficients!
   */
  //std::cout << model(points) << std::endl;
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */

  std::array<float, 3> quad;
  model(points, quad);
  std::cout << "f(x) = " << quad[0] << " * x * x + " << quad[1] << " * x + " << quad[2] << std::endl;

  // check point differences
  //for (size_t i = 0; i < kDataPoints; i++){
  //  float x = points[i][0];
  //  float y = points[i][1];
  //  std::cout << y - (quad[0] * x * x + quad[1] * x + quad[2]) << std::endl; 
  //}
}
