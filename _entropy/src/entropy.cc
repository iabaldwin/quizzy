
//
// \file    entropy.cc
// \author  Sergio Valero, esevalero@gmail.com
// \date    21.06.2018
//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// \brief:  THis program Compute an estimate of the discrete entropy of a     //
//          given source of random information (e.g.  "/dev/random"). Ensure  //
//          that the random source provides integers ranging from [0,128],    //
//          and assume that the input probability space is divided into 16    //
//          events [0,...,8) -> 0, [8,...16) -> 1, etc. Report answer in bits.//
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
#include <sys/stat.h>
#include <assert.h>


template <typename NumericType, int Discretization>
NumericType entropy(const std::string& fileDescriptor) {
  throw std::runtime_error("Implement me!");
}

//------------------------------------------------------------------------------
// In line Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// \brief   File sanity check inline function
//
inline bool isFile (
    const std::string&  name        //< String with the name of the file to check
){
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}


//------------------------------------------------------------------------------
// \brief       Insert path to the random source
//
// \return      String container to the random source path
//
const std::string inserPathToRandomSource(
    void
){
    std::string path;
    std::cout << "_____________________________________________________" << std::endl << std::endl;
    std::cout << "Hey!" << std::endl;
    std::cout << "I can estimate the entropy of a random source! " << std::endl << std::endl ;
    std::cout << "Could you please insert the path to your random source? " << std::endl;
    std::cout << "      1. I can deal with .csv coma separated files, " << std::endl;
    std::cout << "      2. So, please ensure you feed me with csv files! " << std::endl << std::endl;
    std::cout << "*****************************************************" << std::endl;
    std::cout << "Insert Random Source File: " ;
    std::cin >> path;

    bool fileOK = isFile(path);

    if (!fileOK){
        std::cout << "Error: The File doesn´t exist." << std::endl;
        assert( fileOK && "Error: The File doesn´t exist.") ;
    }

    bool isCSV = false;
    if(path.substr(path.find_last_of(".") + 1) == "csv")
        isCSV = true;

    if (!isCSV){
        std::cout << "Error: no .CSV file." << std::endl;
        assert( isCSV && "Error: no .CSV file") ;
    }

    return path;
}



//------------------------------------------------------------------------------
// Main
//
int main() {

    // Print out the entropy of a probability distribution over 8 discrete events,
    // with event occurrences given by a random source of information.

    // std::cout << entropy<float, 8>("/dev/random") << std::endl;

    std::string path = inserPathToRandomSource();

    std::ifstream file ( path ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
    std::string value;
    while ( file.good() )
    {
         getline ( file, value, ',' ); // read a string until next comma: http://www.cplusplus.com/reference/string/getline/
         std::cout << value; // display value removing the first and the last character from it
    }

}
