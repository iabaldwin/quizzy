#include <cmath>
#include <iostream>
#include <fstream>
#include <random>

int frequency(std::vector<int> array, int target) {
  // Returns the frequency of the target
  int count = 0;
  for (std::vector<int>::iterator it = array.begin(); it != array.end(); ++it) {
    if (*it == target) {
      count++;
    }
  }

  return count;
}

template <typename NumericType, int Discretization>
NumericType entropy() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  
  size_t max_value = 128 - 1;
  size_t num_events = max_value / Discretization;
  std::uniform_int_distribution<int> distribution(0, max_value);

  // Generate a series of dice rolls between 0 and the max_value
  size_t num_rolls = 100;
  std::vector<int> array;
  for (size_t i = 0; i < num_rolls; ++i) {
    int raw_rand = distribution(generator);
    int dice_roll = raw_rand / Discretization;
    array.push_back(dice_roll);
  }

  // Calculate the entropy
  NumericType entropy = 0.;
  for (size_t i = 0; i < num_events; ++i) {
    int count = frequency(array, i);
    if (count == 0) {
      continue;
    }

    NumericType p = NumericType(count) / num_rolls;
    entropy -= p * log2(p);
  }

  return entropy;
}

int main() {
  /*
   *Print out the entropy of a probability distribution over 8 discrete events,
   with event occurrences given by a random source of information. 
   */
  std::cout << "Entropy in bits: " << entropy<float, 8>() << std::endl;
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
}
