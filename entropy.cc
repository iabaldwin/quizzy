#include <cmath>
#include <iostream>
#include <fstream>

#define MAX_DATA_SAMPLES  100000
#define DATA_MASK         0x7F

template <typename NumericType, int Discretization>
NumericType entropy(const std::string& fileDescriptor) {

  NumericType entropy = 0;
  char dataPoint;
  u_int sampleCount = 0;
  float bins[Discretization] = {0};
  const u_int binSize = (DATA_MASK + 1)/Discretization;

  FILE* file = std::fopen(fileDescriptor.c_str(), "rb");

  if(file == NULL) {
    std::cout << "Failed to Open File. Exiting." << std::endl;
    return entropy;
  }

  while((!std::feof(file)) && (sampleCount < MAX_DATA_SAMPLES)) {

    std::fread(&dataPoint, 1, 1, file);
    bins[(dataPoint & DATA_MASK)/binSize]++;
    sampleCount++;
  }

  // Shannon Entropy
  float prob;
  for(float frequency : bins) {

    if(frequency != 0) {
      prob = frequency/(float)sampleCount;
      entropy += -(prob * std::log2(prob));
    }
  }

  std::fclose(file);
  return entropy;

}

int main() {
  /*
   *Print out the entropy of a probability distribution over 16 discrete events,
   with event occurrences given by a random source of information. 
   */
  std::cout << entropy<float, 16>("/dev/random") << std::endl;
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
}
