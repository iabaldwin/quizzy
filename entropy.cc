#include <cmath>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <random>

static const unsigned short  buckets = 16;
static const unsigned short rangeMax = 128;


template <typename NumericType, int Discretization>
NumericType entropy(const std::string& fileDescriptor)
{
  std::random_device generator( fileDescriptor );
  std::uniform_real_distribution<NumericType> distribution( 0, rangeMax);
  
  std::vector< int > histogram( buckets );

  // fill histogram
  NumericType value;
  for ( int i = 0; i < Discretization; ++i )
  {
    value = distribution( generator );

    int bucket = ( ( double( buckets ) * value ) / rangeMax );

    histogram[ bucket ] += 1;
  }

  NumericType ret = 0;

  // estimate entropy of the results
  for ( double frequency : histogram )
  {
    double log = log2( frequency / Discretization );

    if ( frequency != 0 )
    {
      ret -= ( frequency / Discretization ) * log;
    }
  } 

  return ret;
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
