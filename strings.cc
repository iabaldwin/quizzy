#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>

void readWords(char const* file_path, std::vector<std::string> *words) {
  std::ifstream input(file_path);
  std::string line;

  while (getline(input, line)) {
    words->push_back(line);
  }
}

bool isPalindromic(std::string word) {
  for (size_t i = 0; i < word.size(); ++i) {
    size_t j = word.size() - 1 - i;
    if (word[i] != word[j]) {
      return false;
    }
  }
  return true;
}

int main() {
  /*
   *Read in the system dictionary, and print out the number of palindromic words
   */
  std::vector<std::string> words;
  readWords("/usr/share/dict/words", &words);
  /*
   *Print out the number of palindromic words in this container
   */
  std::cout << std::count_if(words.begin(),
                             words.end(),
                             [](const std::string & word) {
                              return isPalindromic(word);
                              }) << std::endl;;
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
}
