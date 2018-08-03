#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <iterator>
#include <algorithm>

bool isPalindromic( const std::string& word )
{
  auto leftIt = word.begin();
  auto rightIt = word.end();
  rightIt--;

  while ( *leftIt == *rightIt )
  { 
    if ( leftIt == rightIt || leftIt + 1 == rightIt )
    { 
      return true;
    }

    --rightIt;
    ++leftIt;
  }

  return false;
}


int main( int argc, char **argv )
{
  if ( argc != 2 )
  {
    std::cout << "Strings requires a word list path as an argument" << std::endl;
    exit( 1 );
  }

  std::ifstream dict( argv[1] );
  if ( !dict.is_open() )
  {
    std::cout << "Unable to open file " << argv[1] << std::endl;
    exit( 1 );    
  }

  std::cout << std::count_if( std::istream_iterator< std::string >( dict ), std::istream_iterator< std::string >(), [](const std::string& word) { return isPalindromic(word); } ) 
            << std::endl;;


  exit( 0 );
}
