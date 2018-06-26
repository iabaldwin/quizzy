#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <assert.h>

constexpr int NUM_WORDS = 99171; 

//Read in the system dictionar
std::vector<std::string> readWords( const std::string filename ) {

  std::vector<std::string> words(NUM_WORDS);

  std::ifstream file;
  file.open("/usr/share/dict/words");
  if ( file.fail() ) {
    std::cout << "System dict opening failed.\n";
    exit(1);
  }
  int cnt = 0;
  while( !file.eof() ) {
    std::string word;
    file >> word;
    words.push_back(word);
    cnt++;
  }

  return words;
};

//LessNaive
bool isPalindromic( const std::string word ) {

  if (word.length() < 2) {
    return false;
  }
  
  else if (word.length() % 2 == 0) {
    for (int i =0; i < (word.length() / 2); i++) {
      if (word[i] != word[(word.length() -1 - i)]) {
	return false;
      }
    }
  }

  else {
    for (int i = 0; i < (word.length() / 2); i++) {
      if (word[i] != word[(word.length() - 1 - i)]) {
	return false;
      }
    }
  }

  return true;

};

int main() {

  /*
   *Read in the system dictionary, and print out the number of palindromic words
   */
  const auto words = readWords("/usr/share/dict/words");
  
  /*
   *Print out the number of palindromic words in this container
   */
  std::cout << std::count_if(words.begin(), words.end(),
                             [](const std::string & word) {
                             return isPalindromic(word);
                             }) << " palindromic strings in here!" << std::endl;

}
