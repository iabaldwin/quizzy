#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Returns true if a word is a palindrome. A word is considered a palindrome if
// it reads the same forwards and backwards. Punctuation is not considered part
// of the word, so "put-up", "ma'am", and "shah's" would be palindromes. The
// comparison is case-sensitive: "Abba" would not be a palindrome, but "abba"
// would.
bool isPalindromic(const std::string& word) {
  // Remove any characters which are not letters.
  // Note: this pre-filtering step adds a copy and an O(n) operation. It may not
  // be strictly necessary, depending on the use case.
  std::string alpha_word(word);
  alpha_word.erase(std::remove_if(alpha_word.begin(),
                                  alpha_word.end(),
                                  [](unsigned char c) {
                                    return !std::isalpha(c);
                                  }),
                   alpha_word.end());
  if (alpha_word.size() < 2) {
    return false;
  }

  // Checking that the first part of a word exactly mirrors the last part of the
  // word is sufficient. This requires ~2x fewer comparisons than using
  // word == reversed(word).
  return std::equal(alpha_word.begin(),
                    alpha_word.begin() + alpha_word.size() / 2, 
                    alpha_word.rbegin());
}

// Returns a vector of words read from a file. Each word is expected to be on a
// seperate line in the file; the system dictionaries are a good example.
//
// Note: the size of the file is expected to be "small" so that its contents can
// be stored easily into a vector. (This is true for the system dictionaries,
// which are ~1 MB.)
std::vector<std::string> readWords(const std::string& filename) {
  std::fstream filestream;
  filestream.open(filename, std::ios::in);
  if (!filestream.is_open()) {
    throw std::runtime_error(filename + " could not be opened for reading");
  }

  std::vector<std::string> words;
  std::string next_line;
  while (std::getline(filestream, next_line)) {
    words.push_back(next_line);
  }

  filestream.close();
  return words;
}

int main() {
  const std::vector<std::string> words = readWords("/usr/share/dict/words");
  std::cout << "Dictionary has " << words.size() << " words\n";
  int num_palindromes = std::count_if(words.begin(),
                                      words.end(),
                                      [](const std::string& word) {
                                        return isPalindromic(word); 
                                      });
  std::cout << "Palindrome count: " << num_palindromes << "\n";

  // For the sake of interest, print out the few single-word palindromes in the
  // dictionary.
  std::vector<std::string> palindromes;
  std::copy_if(words.begin(), words.end(), std::back_inserter(palindromes),
    [](const std::string& word) { return isPalindromic(word);});
  std::cout << "Palindromes are: ";
  for (const std::string& word : palindromes) {
    std::cout << word << ",  ";
  }
  std::cout << "\n";
}
