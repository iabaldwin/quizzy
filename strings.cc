#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>

bool openStream(std::string path, std::ifstream &words) {

  bool success = false;

  words.open(path);
  if (words.is_open()) 
    success = true;

  return success;
}


bool isPalindromic(std::string &word) {

  int length = word.length();
  int end = length-1;

  if(length < 2) return false;

  for(int offset = 0; offset < length/2; offset++) {

    if(word[offset] != word[end - offset])  
      return false;  
  }

  return true;
}


int main() {
  /*
   *Read in the system dictionary, and print out the number of palindromic words
   */

  std::ifstream words;
  int palindrome_count = 0;

  // if we fail to acquire a file stream, exit the program
  if(!openStream("/usr/share/dict/words", words)) {

    std::cout << "Failure opening file. Exiting. " << std::endl;
    return -1;

  }

  // lower memory footprint implementation
  std::string word;
  while(words >> word) {
    if(isPalindromic(word) == true)
      palindrome_count ++;
  }

  std::cout << "Palindrome Count: " << palindrome_count << std::endl;

  words.close();
  return 0;
}
