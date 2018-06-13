#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <type_traits>

template<typename NumericType, unsigned int SampleSize, unsigned long Events, unsigned long Upper>
NumericType entropy(const std::string &fileDescriptor) {
    // SampleSize - number of samples to draw from random number generator to build a distribution
    // Events - number of intervals to split the input integer probability space
    // Upper - defines the input integer probability space [0, Upper) to draw random numbers from
    static_assert(SampleSize >= 5, "Minimum 5 samples required to build a distribution");
    static_assert(Events >= 1, "Min number of events is 1");
    static_assert(Upper >= 1, "Min Upper is 1"); // draw integers at random from [0, Upper)
    static_assert(Upper % Events == 0); // ensure equal cardinality for each Event

    static_assert(std::is_floating_point<NumericType>::value, "Floating point type required");


    std::ifstream urandom(fileDescriptor, std::ios::in | std::ios::binary);

    unsigned long EventCardinality = Upper / Events;

    std::vector<unsigned int> event_counter = std::vector<unsigned int>(Events, 0);
    unsigned long long rv = 0;
    size_t size = sizeof(rv);
    if (urandom) {
        for (unsigned int i = 1; i <= SampleSize; i++) {
            urandom.read(reinterpret_cast<char *>(&rv), size);
            if (urandom) {
                rv %= Upper;
                event_counter[rv / EventCardinality]++;
            } else throw ("Cant read from " + fileDescriptor);
        }
        urandom.close();
    } else throw ("Cant open " + fileDescriptor);
    double H = 0.0;
    auto ss = (double) SampleSize;
    double p;
    for (unsigned int c: event_counter) {
        p = (double) c / ss;
        H -= (p == 0.0) ? 0.0 : p * std::log2(p);
    }
    return (NumericType) H;
}

int main() {
    /*
     *Print out the entropy of a probability distribution over 8 discrete events,
     with event occurrences given by a random source of information.
     */

    std::cout << entropy<float, 8, 16, 128>("/dev/random") << std::endl;
    std::cout << entropy<float, 80, 16, 128>("/dev/random") << std::endl;
    std::cout << entropy<float, 800, 16, 128>("/dev/random") << std::endl;
    std::cout << entropy<float, 8000, 16, 128>("/dev/random") << std::endl;
}
