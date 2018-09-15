#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <algorithm>

std::vector<std::string> readWords(std::string path);
bool isPalindromic(std::string word);



int main() {
  /*
   *Read in the system dictionary, and print out the number of palindromic words
   */
  const auto words = readWords("/usr/share/dict/words");
  /*
   *Print out the number of palindromic words in this container
   */
  std::cout << words.size() << std::endl;
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
}

std::vector<std::string> readWords(std::string path) {
  std::ifstream file;
  file.open(path);
  std::vector<std::string> palindromes;
  if (!file.is_open()) return palindromes;

  std::string word;
  while (file >> word)
  {
      if (isPalindromic(word)) {
        palindromes.push_back(word);
      }
  }
  return palindromes;
}

bool isPalindromic(std::string word) {
  for (int i=0; i < word.size()/2; i++) {
    if(word[i] != word[word.size()-i-1])
      return false;
  }
  return true;
}
