#include <cmath>
#include <iostream>
#include <fstream>

#include <ctime>
#include <vector>

template <typename NumericType, int Discretization>
NumericType entropy(const std::string& fileDescriptor) {
  throw std::runtime_error("Implement me!");
}

void generateFile(std::string &fileName, size_t n)
{
  std::ofstream ofs(fileName);
  std::srand((unsigned)std::time(0)); 
  for (size_t i = 0; i < n; i++){
    int rnd = rand() % 128; 
    ofs << rnd << std::endl;  
  }
  return;
}

int main() {
  /*
   *Print out the entropy of a probability distribution over 8 discrete events,
   with event occurrences given by a random source of information. 
   */
  //std::cout << entropy<float, 8>("/dev/random") << std::endl;
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */

  // generate file with random numbers
  std::string file_name = "random.txt";
  size_t n = 1000;
  generateFile(file_name, n);


  // count occurences of numbers [0, 128) 
  std::ifstream ifs(file_name);
  size_t rnd;
  std::vector<double> counter(128, 0);
  double m = 0.0;
  while(ifs >> rnd){
    counter[rnd]++;
    m++;
  }

  // probabilities
  std::vector<double> prob(16, 0);
  for (size_t i = 0; i < 16; i++)
    for (size_t j = 0; j < 8; j++)    
      prob[i] += counter[i * 8 + j] / m;
    
  // entropy
  double entropy = 0.0;
  for (size_t i = 0; i < 16; i++)
    if(prob[i] > 0.0)
      entropy -= prob[i] * std::log2(prob[i]);
  
  std::cout << "entropy: " << entropy << std::endl;

  return 0;
}
