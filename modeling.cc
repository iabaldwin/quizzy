#include <iostream>
#include <cmath>
#include <array>
#include <stdio.h>

#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

const int kDimensions = 2;
const int kDataPoints = 20;
const float points[kDataPoints][kDimensions] = {{3.8686992545234347, 1.3559682753431659},
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
                                                {1.9357873494944466, 2.9778798335036534}};

//Use GD or the normal equation to estimate model coeficients
cv::Mat model( const float pts[][kDimensions], bool USE_GD ) {

  //Copy data into design matrix
  cv::Mat X(kDataPoints, kDimensions, CV_32F);
  for ( int i = 0; i < kDataPoints; i++ ) {
    for (int j = 0; j < kDimensions; j++) {       
      X.at<float>(i,j) = pts[i][j];
    }
  }

  //Copy Y into column vector
  cv::Mat Y(kDataPoints, 1, CV_32F);
  for ( int i = 0; i < kDataPoints; i++ ) {
    Y.at<float>(i) = X.at<float>(i,1);
  }

  //If using linear regression with gradient-discent
  if (USE_GD) {

    std::cout << "Using Linear regression w/ gradient-descent: " << std::endl;
    cv::Mat coefs(1, kDimensions*2, CV_32F);
    cv::fitLine( X, coefs, CV_DIST_L2, 0, 0.01, 0.01);

    std::cout << "Model Coeficients = ";
    return coefs;
  }

  //Otherwise try the normal equation
  else {

    std::cout << "Using normal-equation to estimate model: " << std::endl;

    //Append column of ones to input data
    cv::Mat Xcol(kDataPoints, kDimensions, CV_32F);
    for ( int i = 0; i < kDataPoints; i++ ) {
      Xcol.at<float>(i,1) = X.at<float>(i,0);
      Xcol.at<float>(i,0) = 1;
    }

    //Normal Equation - (X'X)^-1 * X' * Y
    cv::Mat xT = Xcol.t();
    cv::Mat temp = xT * Xcol;
    cv::invert( temp, temp, cv::DECOMP_SVD );
    cv::Mat coefs = temp * xT * Y;

    //compute residual error of resulting model
    float cost = 0;
    cv::Mat prod = Xcol * coefs;
    cv::pow((Y - prod), 2, prod);
    cv::Mat reduce; 
    cv::reduce( prod, reduce, 1, CV_REDUCE_SUM, CV_32F );
    reduce = (reduce / kDataPoints);
    cost = reduce.at<float>(0);
    std::cout << "Residual error from normal-equation model estimate: " << cost << std::endl;
    return coefs;
    
  }
    
};


int main() {
  /*
   *Use a model of your choice to approximate the relationship between the
   independent/dependent variable pairs above. Model choice is up to you - just
   print out coefficients!
   */
  
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
  std::cout << "Model Coeficients = " << model(points, 0) << std::endl;

  return 0;
}
