#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <assert.h>

bool isPalindromic(const std::string& word) {
  for (int i = 0; i < word.size()/2; ++i) {
    if (word[i] != word[word.size() - 1 - i]) {
      return false;
    }
  }

  return true;
}

int numberOfPalindromes(const std::string path) {
  std::ifstream file;
  std::string word;
  int numPals = 0;

  file.open(path);

  while (file >> word) {
    if (isPalindromic(word)) {
      ++numPals;
    }
  }

  file.close();

  return numPals;
}

int main() {
  std::string path;
  std::cout << "Enter the path containing the dictionary:" << std::endl;
  std::cin >> path;
  std::cout << "There are " << numberOfPalindromes(path) << " palindromes in the dictionary." << std::endl;
  return 0;
}
