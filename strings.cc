#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <assert.h>

//returns whether a string is palindromic
bool isPalindromic(const std::string& word) {
  for (int i = 0; i < word.size()/2; ++i) {
    if (word[i] != word[word.size() - 1 - i]) {
      return false;
    }
  }

  return true;
}

//returns number of palindromes and caches the "palindromeness" of a word so it doesn't need to check twice
int numberOfPalindromes(const std::string path, std::unordered_map<std::string, bool> &wordCache) {
  std::ifstream file;
  std::string word;
  int numPals = 0;

  file.open(path);

  while (file >> word) {
    if (wordCache.find(word) == wordCache.end()) {
      wordCache[word] = isPalindromic(word);
    }

    if (wordCache[word]) {
      ++numPals;
    }
  }

  file.close();

  return numPals;
}

int main() {
  std::string path;
  std::unordered_map<std::string, bool> wordCache;
  std::cout << "Enter the path containing the dictionary:" << std::endl;
  std::cin >> path;
  std::cout << "There are " << numberOfPalindromes(path, wordCache) << " palindromes in the dictionary." << std::endl;
  return 0;
}
