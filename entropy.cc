#include <cmath>
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <algorithm>

constexpr int MAX_VALUE = 128;
constexpr int SAMPLES = 1000000;

template<typename NumericType, int Discretization>
NumericType entropy(const std::string &fileDescriptor) {
    // Open stream of random
    std::ifstream urandom(fileDescriptor, std::ios::in | std::ios::binary);
    if (!urandom) {
        throw std::runtime_error("Couldn't open stream");
    }

    // Fill vector with random data
    std::vector<unsigned char> randomData;
    while ((urandom.rdstate() != std::ios_base::eofbit) && (randomData.size() < SAMPLES))
        randomData.push_back(urandom.get() % MAX_VALUE);


    // Create histogram
    std::array<int, Discretization> hist;
    hist.fill(0);

    float binWidth = MAX_VALUE / static_cast<float>(Discretization);
    auto aRandomToBin = [&binWidth](const unsigned char &iChar) {
        return static_cast<int>(std::floor(iChar / binWidth));
    };

    std::for_each(randomData.begin(), randomData.end(),
                  [&hist, &aRandomToBin](const unsigned char &iChar) {
                      hist[aRandomToBin(iChar)]++;
                  });

    // Entropy calc
    auto aEntropyComputation = [&randomData](const int &iBinValue) {
        NumericType Px = iBinValue / static_cast<NumericType>(randomData.size());
        return -(Px * std::log2(Px));
    };
    NumericType H = 0;
    std::for_each(hist.begin(), hist.end(),
                  [&H, &aEntropyComputation](const int &iBinValue) {
                      H += iBinValue ? aEntropyComputation(iBinValue) : H;
                  });

    return H;
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
