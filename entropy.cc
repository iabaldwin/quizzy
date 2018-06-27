#include <cmath>
#include <iostream>
#include <fstream>
#include <cstring>

/**
 * @brief Path to read bytes from
 * @note Sampling from /dev/random is very slow compared to /dev/urandom or
 * a premade binary file.
 */
static const std::string DEVICE = "/dev/urandom";

/**
 * @brief Max number of samples to take from the device.
 */
static const unsigned int NUM_SAMPLES = 10000;

/**
 * @brief Compute the entropy of a device
 * @details Compute the entropy of the random device located at path. Bytes are
 * read and only the lower order bytes (0111 1111) are considered in order to
 * keep the input value in the range [0,128). The Discretization breaks is the
 * size of the intervals for each bin. For example, a Discretization of 8 will
 * create 16 bins. The device is sampled up to NUM_SAMPLES times, fewer if 
 * there are fewer bytes than NUM_SAMPLES. The value returned is the Entropy 
 * as defined by Claude Shannon:
 * 
 *  H(X) = -sum( P(xi) * log2(P(xi)), i=0 to n-1)
 * @see NUM_SAMPLES
 * @see DEVICE
 */
template <typename NumericType, int Discretization>
NumericType entropy(const std::string& path) {
    // We only care about discretizing events in the range [0,128)
    static_assert(Discretization < 128);
    
    std::ifstream device(path, std::ios::in | std::ios::binary);
    
    // Configure histogram
    // An example configuration:
    //      Discretization: 8
    //      Input Range: [0,128)
    //      Bins: { 0 = [0,8), 1 = [8,16), ... 15 = [120,128) }
    int num_bins = 128/Discretization;
    int interval = Discretization;
    unsigned int histogram[num_bins];
    std::memset(histogram,0,sizeof(histogram));
    
    int sample_count = 0;
    
    for (unsigned int ii = 0; ii < NUM_SAMPLES; ++ii){
        if (device.eof())
            break;
        
        // Read a value from the device
        char value = device.get();
        
        // Record the number of samples in case NUM_SAMPLES exceeds the number
        // of samples we can take from the device.
        ++sample_count;
        
        // Capture the relevant bits for values 0-127; ignore the sign bit
        value = value & 0x7F;
        
        // Populate the histogram
        int bin = value / interval;
        
        ++histogram[bin];
    }
    
    device.close();
    
    if (sample_count == 0)
        return 0;
    
    // Shannon Entropy is defined as:
    // H(X) = -sum( P(xi) * log(P(xi)), i=0 to n-1)
    NumericType sum = 0;
    for(unsigned int ii = 0; ii < num_bins ; ++ii)
    {
        unsigned int count = histogram[ii];
        // Check for counts of 0 because std::log2 because from documentation
        // for std::log2: Pole error may occur if arg is zero.
        if (count == 0)
            continue;
        float prob = count / static_cast<float>(sample_count);
        sum += (prob  * std::log2(prob));
    }
    // Return the negative sum
    return -sum;
}

int main() {
    /*
    * Print out the entropy of a probability distribution over 8 discrete events,
    * with event occurrences given by a random source of information. 
    */
    std::cout << entropy<float, 8>(DEVICE) << std::endl;
}
