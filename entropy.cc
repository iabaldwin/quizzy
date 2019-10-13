#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

template <typename NumericType>
std::vector<NumericType> get_random_numbers_from_dev_random(const std::string& random_source_name, NumericType number, NumericType max_value){
    std::vector<NumericType> results;
    for (NumericType i = 0; i<number; i++){
		NumericType random_value = 0; //Declare value to store data into
		size_t size = sizeof(random_value); //Declare size of data
		std::ifstream urandom(random_source_name, std::ios::in|std::ios::binary); //Open stream
		if(urandom) //Check if stream is open
		{
		    urandom.read(reinterpret_cast<char*>(&random_value), size); //Read from urandom
		    if(urandom) //Check if stream is ok, read succeeded
		    {
		    	random_value = random_value%max_value; //Ensuring our random_number is under the specified max value
		        //std::cout << "Read random value: " << random_value << std::endl;
		        results.push_back(random_value);
		    }
		    else //Read failed
		    {
		        std::cerr << "Failed to read from "<< random_source_name << std::endl;
		    }
		    urandom.close(); //close stream
		}
		else //Open failed
		{
		    std::cerr << "Failed to open "<< random_source_name << std::endl;
		}
	}
    return results;
}

double log2( double number ) {
   return log( number ) / log( 2 ) ;
}

//naive plugin estimator for entropy using frequencies
template <typename NumericType, int Discretization>
double entropy(const std::string& fileDescriptor, NumericType InformationSize, NumericType max_value) {
	const auto random_numbers = get_random_numbers_from_dev_random<unsigned short>(fileDescriptor, InformationSize, max_value);
	auto random_events = random_numbers;
	for(short i=0; i<random_events.size(); i++){
		random_events[i] = random_events[i] % Discretization;
	}
	std::map< NumericType , int > frequencies ;
	for ( NumericType s : random_events ) frequencies[ s ] ++ ;
	int numlen = random_events.size() ;
	double infocontent = 0 ;
	for ( std::pair< NumericType , int > p : frequencies ) {
		double freq = static_cast<double>( p.second ) / numlen ;
		infocontent -= freq * log2( freq ) ;
	}
	//std::cout << "The entropy of /dev/urandom is " << infocontent << " bits." << std::endl;
	return infocontent;
}

int main() {
  /*
   *Print out the entropy of a probability distribution over 8 discrete events,
   with event occurrences given by a random source of information. 
   */
	std::cout << entropy<unsigned short, 8>("/dev/urandom", 128, 128) << std::endl;
	return 0;
}
