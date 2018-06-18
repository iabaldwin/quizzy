#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

int num_events = 8;
int datapoints = 8;

template <typename NumericType, int Discretization>
NumericType entropy(const std::string& fileDescriptor) {
    
    ifstream inFile(fileDescriptor.c_str(), ios::binary);

    if (!inFile.good())
    {
        cout << "Unable to access file: " << fileDescriptor.c_str() << endl;
        // return 0;
    }
    vector<int> counts(num_events,0);

    for(int i=0;i<datapoints;++i)
    {
      char b = inFile.get();
      // read integer 0-127
      int randint = b  & 0x7f;
      //bucket into events
      int event = randint / num_events;
      counts[event]++;
    }

    NumericType sum = 0;

    for(int i=0;i<num_events;++i)
    {
      NumericType Pe = counts[i];
      Pe /= num_events;
      int logPe = 0;
      // log(0) = 0 in this application
      if (Pe != 0) logPe = log(Pe);
      // Pe * log Pe base num_events
      sum += Pe * logPe / log(num_events);
    }
    return sum;
}

int main() {
  /*
   *Print out the entropy of a probability distribution over 8 discrete events,
   with event occurrences given by a random source of information.
   */
  std::cout << entropy<float, 8>("/dev/random") << std::endl;
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
}
