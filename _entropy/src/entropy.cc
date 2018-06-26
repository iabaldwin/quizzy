
//
// \file    entropy.cc
// \author  Sergio Valero, esevalero@gmail.com
// \date    21.06.2018
//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// \brief:  This program Compute an estimate the discrete entropy of a        //
//          given source of random information (e.g.  "/dev/random"). Ensure  //
//          that the random source provides integers ranging from [0,128],    //
//          and assume that the input probability space is divided into 16    //
//          events [0,...,8) -> 0, [8,...16) -> 1, etc. Report answer in bits.//
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <math.h>
#include <iostream>
#include <fstream>
#include <random>
#include <sys/stat.h>
#include <assert.h>
#include <set>


//------------------------------------------------------------------------------
// In line Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// \brief   File sanity check
//
inline bool isFile (
    const std::string&  name        //< String with the name of the file
){
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

//------------------------------------------------------------------------------
// \brief       Insert path to the random source
//
// \return      String container to the random source path
//
const std::string inserPathToRandomSource(){

    std::string path;
    std::cout << "*****************************************************" << std::endl << std::endl;
    std::cout << "Hey!" << std::endl;
    std::cout << "I can estimate the entropy of a random source! " << std::endl << std::endl ;
    std::cout << "Could you please insert the path to your random source? " << std::endl;
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << ">> Insert Random Source File : " ;
    std::cin >> path;

    bool fileOK = isFile(path);

    if (!fileOK){
        std::cout << "Error: The File doesn't exist." << std::endl;
        assert( fileOK && "Error: The File doesn't exist.") ;
    }

    return path;
}

//------------------------------------------------------------------------------
// \brief       Calculate the Shannon entropy
//
// \return      entropy in bits
//
template <typename NumericType, int Discretization>
NumericType entropy(
    const std::string& fileDescriptor   //< File to a random source
) {
    // Some initializetions
    const int MAX_RANGE = 128;
    char word;
    int numWord;
    int idx;
    int bins = MAX_RANGE/Discretization;
    std::vector<int> hist(bins, 0);

    // Reading file
    std::ifstream source(fileDescriptor);
    int lenFreq = 0;
    while (source.good()){
        source.get(word);
        numWord = int(word);
        
        // Histogram with the frecuency of repetitions
        // of every symbol
        if (numWord >= 0 && numWord < MAX_RANGE){
            
            idx = (int)(numWord / Discretization);
            hist[idx] += 1;
            lenFreq ++;
        }
    }
    
    source.close();

    NumericType ent = 0.0f;
    for (auto freq=hist.begin(); freq != hist.end(); ++freq ){
        if (*freq != 0){
            /// Shannon entropy
            float rep = (float)*freq/(float)lenFreq;
            ent -=  rep * ( log2(rep) / log2(2.0) ) ;
        }
    }

    return ent;
}


//------------------------------------------------------------------------------
// Main
//------------------------------------------------------------------------------
int main() {

    // Print out the entropy of a probability distribution over 8 discrete events,
    // with event occurrences given by a random source of information.
     std::string path = inserPathToRandomSource();
     std::cout << "The entropy of the file in bits is : " << entropy<float, 8>(path) << std::endl;

     return 0;
}
