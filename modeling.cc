#include <iostream>
#include <cmath>
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

void ModelFit(float coeff[])
{
    float x = 0, y = 0, xy = 0, x2 = 0;
    float mean_x = 0, mean_y = 0;
    for(int i = 0; i < kDataPoints; i++)
    {
        x += points[i][0];
        y += points[i][1];
        xy += points[i][0] * points[i][1];
        x2 += points[i][0] * points[i][0];
    }
    mean_x = x/kDataPoints;
    mean_y = y/kDataPoints;
    float ss_xy = xy - (kDataPoints*mean_x*mean_y);
    float ss_x2 = x2 - (kDataPoints*mean_x*mean_x);
    coeff[0] = ss_xy/ss_x2;
    coeff[1] = mean_y - coeff[0]*mean_x;
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
  float coefficients[2];
  ModelFit(coefficients);
  std::cout <<"coefficients for the model: y = C0 + C1*x"<<std::endl;
  std::cout <<"coefficient C0:"<<coefficients[1]<<"\n"<<"coefficient C1:"<<coefficients[0]<<std::endl; 
}
