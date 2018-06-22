#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <iterator>

#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

constexpr int kPatchSize = 5;

constexpr float referencePatch[kPatchSize][kPatchSize] = {{0.13562204,0.27552594,0.35980544,0.94924713,0.34222606},
                                                          {0.94931411,0.27460949,0.75677189,0.24251008,0.50926451},
                                                          {0.97589259,0.46464161,0.52612093,0.95127515,0.37320958},
                                                          {0.10335494,0.22910981,0.44470971,0.28351396,0.65846204},
                                                          {0.94594718,0.52795235,0.98536625,0.09142491,0.99676918}};

constexpr float patchCandidate1[kPatchSize][kPatchSize] = {{0.65051287,0.0858077 ,0.62365821,0.93836648,0.50158388},
                                                           {0.82328337,0.82281573,0.05730619,0.44645271,0.96427283},
                                                           {0.82971349,0.80185606,0.96409221,0.98732598,0.28638456},
                                                           {0.07682913,0.26305113,0.75847402,0.80500383,0.28722148},
                                                           {0.20879353,0.24539516,0.71978387,0.66240491,0.08076461}};

constexpr float patchCandidate2[kPatchSize][kPatchSize] = {{ 0.13290402,0.96803355,0.8971992 ,0.76052644,0.30922267},
                                                           {0.30115749,0.78518142,0.30251533,0.69725331,0.58646293},
                                                           {0.03114041,0.15132543,0.60837695,0.18235618,0.74499181},
                                                           {0.19450344,0.93216069,0.5751807 ,0.38489764,0.5703268},
							   {0.92990664,0.22307124,0.63934838,0.38695049,0.21440734}};

template < typename NumericType, int Discretization >
NumericType entropy( const NumericType patch[][kPatchSize] ) { 
 
  //Number of events input by user 
  const int NUM_EVENTS = Discretization;
  
  NumericType* bins = NULL;
  NumericType entrpy = 0.0;
  NumericType min = 1000000;
  NumericType max = 0;
  
  //Read in random events, and calc min/max
  for ( int i = 0; i < kPatchSize; i++ ) {
    for ( int j = 0; j < kPatchSize; j++ ) {
      min = std::min( min, patch[i][j] );
      max = std::max( max, patch[i][j] );
    }
  }

  //calc number of bins based on min/max
  //we scale by a factor of ten to avoid all events in one bin
  int num_bins = (int) (floor(10*max) - floor(10*min) + 1);
  int bin_width = (int) ceil( (floor(10*max) - floor(10*min)) / num_bins );
  bins = new NumericType[num_bins];
  for ( int i = 0; i < num_bins; i++ ) {
    bins[i] = 0;
  }

  //increment at appropriate bin
  for ( int i = 0; i < kPatchSize; i++ ) {
    for ( int j = 0; j < kPatchSize; j++ ) {
      int bin_idx = (int) ((floor(10*patch[i][j]) - min) / bin_width);
      bins[ bin_idx ]++;
    }
  }
  
  //If only one symbol occured 
  if ( bins[0] == NUM_EVENTS ) {
      return 0;
  }

  //Sum over all bin log probabilities
  float invTotal = 1.0 / (float) NUM_EVENTS;
  for (int i = 0; i < NUM_EVENTS; i++) {
    NumericType prob = bins[i] * invTotal;
    if ( prob > 0.0000001 ) {
        entrpy += -prob * log2(prob);
    }    
  }

  delete[] bins;
  bins = NULL;
  
  return entrpy;
  
};


/*
 * User can specify similarity metric with USE_OPENCV flag: 
 * 0-mutual-information, 
 * 1-L2 distance, 
 * 2-Normalized cross-correlation
 */
template < typename NumericType, int num_pxls >
NumericType similarity( const NumericType reference[kPatchSize][kPatchSize], 
                        const NumericType candidate[kPatchSize][kPatchSize],
			bool USE_OPENCV) {

  //If user specifies L2 distance metric
  if (USE_OPENCV == 1) {

    //Copy data into design matrix
    cv::Mat ref(kPatchSize, kPatchSize, CV_32F);
    cv::Mat cand(kPatchSize, kPatchSize, CV_32F);
    for ( int i = 0; i < kPatchSize; i++ ) {
      for (int j = 0; j < kPatchSize; j++) {       
        ref.at<float>(i,j) = reference[i][j];
	cand.at<float>(i,j) = candidate[i][j];
      }
    }

    //L2 similarity 
    NumericType norm = cv::norm( ref, cand );

    return norm;
  }

  //If cross-corr template matching specified 
  else if (USE_OPENCV == 2) {

    //Copy data into design matrix
    cv::Mat ref(kPatchSize, kPatchSize, CV_32F);
    cv::Mat cand(kPatchSize, kPatchSize, CV_32F);
    for ( int i = 0; i < kPatchSize; i++ ) {
      for (int j = 0; j < kPatchSize; j++) {       
        ref.at<float>(i,j) = reference[i][j];
	cand.at<float>(i,j) = candidate[i][j];
      }
    }

    cv::Mat corr(kPatchSize, kPatchSize, CV_32F);
    cv::matchTemplate( ref, cand, corr, cv::TM_CCORR_NORMED );

    NumericType res = corr.at<float>(0);
    return res;

  }
  
  else { //Use entropy based mutual-information
    
    //Get entropy values for each individual image
    NumericType mutualInfo = 0;
    NumericType entropy1 = entropy<NumericType, num_pxls>( reference );
    NumericType entropy2 = entropy<NumericType, num_pxls>( candidate );
  
    //Calculate entropy from joint histogram
    NumericType jointData[kPatchSize*2][kPatchSize];
    std::copy( &reference[0][0], &reference[0][0]+kPatchSize*kPatchSize, &jointData[0][0] );
    std::copy( &candidate[0][0], &candidate[0][0]+kPatchSize*kPatchSize, &jointData[0][0]+kPatchSize*kPatchSize );
    NumericType jointEntropy = entropy<NumericType, num_pxls*2>( jointData );

    //Mutual-information
    mutualInfo = (NumericType) (entropy1 + entropy2 - jointEntropy) / (NumericType) sqrt(entropy1 * entropy2);

    return mutualInfo;
  }
    
};

int main() {

  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
  
  /*
   *Print out the similarity between the reference patch, and candidate 1
   */
  std::cout << similarity<float, kPatchSize*kPatchSize>(referencePatch, patchCandidate1, 1) << std::endl;
  /*
   *Print out the similarity between the reference patch and candidate 2 
   */
  std::cout << similarity<float, kPatchSize*kPatchSize>(referencePatch, patchCandidate2, 1) << std::endl;

  std::cout << "Patch Candidate 2 is a better match!" << std::endl;
  return 0;
  
}
