#include <cmath>
#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>

template <typename NumericType, int Discretization>
NumericType entropy(const std::string& fileDescriptor) { 
 
  //Number of events input by user 
  const int NUM_EVENTS = Discretization;
  
  std::fstream inFile(fileDescriptor.c_str(), std::ios_base::in);
  
  NumericType events[NUM_EVENTS];
  NumericType* bins = NULL;
  
  NumericType entropy = 0.0;
  NumericType min = 10000000;
  NumericType max = NUM_EVENTS;
  
  //Read in random events, and calc min/max
  for ( int i = 0; i < NUM_EVENTS; i++ ) {
      inFile >> events[i];
      min = std::min( min, events[i] );
      max = std::max( max, events[i] );
  }

  //calc number of bins based on min/max
  int num_bins = (int) (max - min + 1);
  int bin_width = (int) ceil((max - min) / num_bins);
  bins = new NumericType[num_bins];
  for ( int i = 0; i < num_bins; i++ ) {
      bins[i] = 0;
  }
  
  //increment at apropriate bin
  for ( int i = 0; i < NUM_EVENTS; i++ ) {
      int bin_idx = (int) ( (events[i] - min) / bin_width );
      bins[ bin_idx ] += 1;
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
          entropy += -prob * log2(prob);
      }    
  }

  //Clean up so we don't segfault
  delete[] bins;
  bins = NULL;
  
  return entropy;
}

int main() {
  /*
   *Print out the entropy of a probability distribution over 8 discrete events
   with event occurrences given by a random source of information. 

   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
  const std::string inFile = "random_dat.txt";
  std::cout << entropy<float, 16>(inFile) << std::endl;
  
  return 0;
}
