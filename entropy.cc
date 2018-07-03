#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>

template <typename NumericType, int Discretization>
NumericType entropy(const std::string& fileDescriptor) {
    
    std::ifstream ReadRandomData(fileDescriptor.c_str());
    int NumberEvents = Discretization;
    NumericType Events[NumberEvents];
    for(int i = 0; i < NumberEvents-1; i++)
    {
        ReadRandomData >> Events[i];
    }
    NumericType* min_ptr = std::min_element(Events,Events+NumberEvents);
    NumericType* max_ptr = std::max_element(Events,Events+NumberEvents);
    
    NumericType min_value = *min_ptr;
    NumericType max_value = *max_ptr;
    
    int NumberBins = max_value - min_value + 1;
    
    NumericType* Histogram = new NumericType [NumberBins];
    for ( int i = 0; i < NumberEvents; i++ ) 
    {
      int idx = (int) ((Events[i] - min_value) / (ceil((max_value - min_value) / NumberBins) ));
      Histogram[ idx ] ++;
    }
    NumericType entropy_val = 0;
    for (int i = 0; i < NumberEvents; i++) 
    {
      NumericType prob = Histogram[i] * 1 / (float)NumberEvents;
      entropy_val += -prob * log2(prob);
    }
    delete[] Histogram;
    return entropy_val;
    
}

int main() {
  /*
   *Print out the entropy of a probability distribution over 8 discrete events,
   with event occurrences given by a random source of information. 
   */
    std::cout << entropy<float, 8>("random.txt") << std::endl;
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
}
