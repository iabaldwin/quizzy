#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <cmath>
#include <chrono>
#include <limits>
#include <iomanip>

namespace entropy_tools {

namespace ch = std::chrono;

template <typename NumericType, int Discretization>
NumericType entropy(const std::string& filePath, const int64_t& requiredNumberOfReads) {
    if(requiredNumberOfReads <= 0) {
        //could not estimate entropy without taking samples
        return -1.f;
    }
    uint8_t randVal(0);
    //I'm making assumption that in task it was [0,128) range, but not [0,128]
    //this will make range for all 16 events equal and number will use 7 bits
    const int SIZE_TO_READ = 1;
    const int EVENTS_COUNT = 16;
    //counts for each event to get probabilities
    std::vector<int64_t> statistics(EVENTS_COUNT, 0);
    std::ifstream randomSource(filePath.c_str(), std::ios::in | std::ios::binary);
    if(randomSource) {
        for(int64_t iR = 0; iR < requiredNumberOfReads; ++iR) {
            ch::high_resolution_clock::time_point beforeRead = ch::high_resolution_clock::now();
            randomSource.read(reinterpret_cast<char*>(&randVal), SIZE_TO_READ);
            ch::high_resolution_clock::time_point afterRead = ch::high_resolution_clock::now();
            ch::duration<double> readTime = ch::duration_cast<ch::duration<double> >(afterRead - beforeRead);
            //check if function was not running too long for case when system possibly run out of entropy
            if(readTime.count() > 1.) {
                std::cout << "Warning! Number of samples is less than required due to read times. Precision could be affected" << std::endl;
                break;
            }

            //will use only 7 bits from byte
            randVal = randVal >> 1;

            for(int iE = 0; iE < EVENTS_COUNT; ++iE) {
                //checking if random value is not greater than max value of current event
                if(int(randVal) < (Discretization * (iE + 1))) {
                    statistics[iE]++;
                    break;
                }
            }
        }
        //calculate number of samples taken
        int64_t totalElemsCount(0);
        for(int iE = 0; iE < EVENTS_COUNT; ++iE) {
            totalElemsCount += statistics[iE];
        }

        float entropy(0.f);
        for(int iE = 0; iE < EVENTS_COUNT; ++iE) {
            float currEventProb = float(statistics[iE]) / totalElemsCount;
            if(currEventProb < std::numeric_limits<float>::epsilon()) {
                continue;
            }
            entropy += currEventProb * log2(currEventProb);
        }
        //because in formula log2 took from probability, but not 1 / probability
        entropy *= -1.f;
        return entropy;
    } else {
        //file could not be opened for reading
        return -1.f;
    }
}
} //entropy_tools

int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cout << "Incorrect input. Please enter source of random information as a parameter" << std::endl;
        return 1;
    }
    std::string filePath(argv[1]);
    //number of samples picked to make sure program will work not too long(up to couple sec) on /dev/urandom file
    const int64_t REQUIRED_READS_COUNT = 10000000;
    float entropy = entropy_tools::entropy<float, 8>(filePath, REQUIRED_READS_COUNT);
    std::cout << std::setprecision(15);
    if(entropy >= 0.f) {
        std::cout << "For current source entropy is " << entropy << " bits" << std::endl;
    } else {
        std::cout << "Something went wrong. Please check that file exists and could provide data" << std::endl;
    }
    return 0;
}
