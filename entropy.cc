#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

const int max_count = 10000;
const int interval = 8;

template <typename NumericType, int Discretization>
NumericType entropy(const std::string& fileDescriptor) {
    FILE *fp = fopen(fileDescriptor.c_str(), "rb");

    char data;
    long n[Discretization] = {0};
    long count = 0;
    char mask = 0x7F; // for the range of [0, 128]
    int event_id = 0;
    // Count occurance for different event
    while (!feof(fp)) {
        fread(&data, 1, 1, fp);
        event_id = (data&mask)/interval;
        n[event_id]++;
        count++;
        if (count >= max_count)
            break;
    }
    fclose(fp);
    
    // Compute Entropy
    float entropy = 0.f;
    for (int i = 0; i < Discretization; i++) {
        float p = 1.f*n[i]/count;
        entropy += -p*log2(p);
    }
    
    return entropy;
}

int main() {
  /*
   *Print out the entropy of a probability distribution over 8 discrete events,
   with event occurrences given by a random source of information. 
   */
    
    /* Comments from Makefile:
        Entropy
        Compute an estimate of the discrete entropy of a given source of random
        information (e.g.  "/dev/random"). Ensure that the random source provides
        integers ranging from [0,128], and assume that the input probability space is
        divided into 16 events [0,...,8) -> 0, [8,...16) -> 1, etc. Report answer in
        bits.
     */
    std::cout << entropy<float, 16>("/dev/random") << std::endl;
  
   /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
}
