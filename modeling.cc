#include <numeric>
#include <iostream>
#include <cmath>
#include <array>

constexpr int num_dimensions = 2;
constexpr int num_data_points = 20;
constexpr std::array<std::array<float, num_dimensions>, num_data_points> points = {{{3.8686992545234347, 1.3559682753431659},
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

//Need 3 coefficients for a quadratic equation
constexpr int num_coefficients = 3;
constexpr int num_rows_sigma_mat = num_coefficients;
//Number of columns increases by 1 since the square matrix is augmented with the RHS as a column
constexpr int num_cols_sigma_mat = num_rows_sigma_mat + 1;

//Fills the coefficients in the array passed in as 2nd parameter
void gaussElimination(std::array<std::array<float, num_cols_sigma_mat>, num_rows_sigma_mat> &mat,
                      std::array<float, num_coefficients> &ret) {
  int rows = num_rows_sigma_mat;
  int cols = num_cols_sigma_mat;

  //Forward elimination
  for(int i = 0; i < rows; i++) {
    int max_idx = i;
    float max_val = std::fabs(mat[max_idx][i]);

    //Find max value in the col
    for(int j = i+1; j < rows; j++) {
      if(std::fabs(mat[j][i]) > max_val) {
        max_val = mat[j][i];
        max_idx = j;
      }
    }

    if(max_idx != i) {
      //swap rows
      for(int j = 0; j < cols; j++) {
        float tmp = mat[i][j];
        mat[i][j] = mat[max_idx][j];
        mat[max_idx][j] = tmp;
      }
    }

    for(int j = i+1; j < rows; j++) {
      float factor = mat[j][i]/mat[i][i];
      for(int k = i+1; k < cols; k++) {
        mat[j][k] -= mat[i][k] * factor;
      }

      //Explicitly set elements to 0 below the current index
      mat[j][i] = 0.0;
    }
  }

  //Back substitution to calculate a2, a1 and a0
  for(int i = rows-1; i >= 0; i--) {
    //Start with value in last col (RHS)
    ret[i] = mat[i][cols-1];

    //This will not be executed for last row (a2)
    //Use value of co-efficient calculated in previous pass and
    //subtract it's multiple from the value set above
    for(int j = i+1; j < rows; j++) {
      ret[i] -= mat[i][j] * ret[j];
    }

    //Finally divide by the LHS position whose co-efficient is
    //being calculated in this pass
    ret[i] /= mat[i][i];
  }
}

//Calculates coefficients to fit a quadratic equation to an array of x,y points using least-squares method
//It tries to minimize the sum of residuals and uses gaussian elimination to solve the resulting matrix
void model(const std::array<std::array<float, num_dimensions>, num_data_points> &pts) {
  //Summation of 1 "num_data_points" times
  float sum_n = num_data_points;
  //Summation of all x values
  float sum_x = std::accumulate(pts.begin(), pts.end(), 0.0,
                                [](float sum, std::array<float, num_dimensions> val) {
                                  return sum + val[0];
                                });
  //Summation of all x^2 values
  float sum_xp2 = std::accumulate(pts.begin(), pts.end(), 0.0,
                                  [](float sum, std::array<float, num_dimensions> val) {
                                    return sum + val[0]*val[0];
                                  });
  //Summation of all x^3 values
  float sum_xp3 = std::accumulate(pts.begin(), pts.end(), 0.0,
                                  [](float sum, std::array<float, num_dimensions> val) {
                                    return sum + val[0]*val[0]*val[0];
                                  });
  //Summation of all x^4 values
  float sum_xp4 = std::accumulate(pts.begin(), pts.end(), 0.0,
                                  [](float sum, std::array<float, num_dimensions> val) {
                                    return sum + val[0]*val[0]*val[0]*val[0];
                                  });
  //Summation of all y values
  float sum_y = std::accumulate(pts.begin(), pts.end(), 0.0,
                                [](float sum, std::array<float, num_dimensions> val) {
                                  return sum + val[1];
                                });
  //Summation of all x*y values
  float sum_xy = std::accumulate(pts.begin(), pts.end(), 0.0,
                                 [](float sum, std::array<float, num_dimensions> val) {
                                   return sum + val[0]*val[1];
                                 });
  //Summation of all (x^2)*y values
  float sum_xp2y = std::accumulate(pts.begin(), pts.end(), 0.0,
                                   [](float sum, std::array<float, num_dimensions> val) {
                                     return sum + val[0]*val[0]*val[1];
                                   });

  //This matrix is made up of all the summations(sigma) above
  //The matrix is to solve Ax=B where A is augmented with B to give a 3x4 matrix
  std::array<std::array<float, num_cols_sigma_mat>, num_rows_sigma_mat> sigma_mat =
    {{{{sum_n,   sum_x,   sum_xp2,  sum_y}},
      {{sum_x,   sum_xp2, sum_xp3,  sum_xy}},
      {{sum_xp2, sum_xp3, sum_xp4,  sum_xp2y}}}};

  //Array to hold the coefficients
  std::array<float, num_coefficients> ret = {};

  gaussElimination(sigma_mat, ret);

  printf("coefficients for y = a0 + a1(x) + a2(x^2)\n");
  for(int i  = 0; i < num_coefficients; i++) {
    printf("a[%d]: %f\n", i, ret[i]);
  }
}

//Curve fit plot at: https://www.desmos.com/calculator/ukken1azhi
int main() {
  model(points);
  return 0;
}
