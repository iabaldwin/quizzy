#include <assert.h>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::string> readWords(const std::string file_path) {
  std::ifstream input_stream(file_path);
  std::string current_word;
  std::vector<std::string> words;

  while (std::getline(input_stream, current_word)) {
    words.push_back(current_word);
  }

  return words;
}

bool isPalindromic(const std::string &word) {
  auto num_characters = word.size();
  if (num_characters == 0) {
    return false;
  }

  auto left_idx = 0;
  auto right_idx = num_characters - 1;
  while (left_idx < right_idx) {
    if (word[left_idx] != word[right_idx]) {
      return false;
    }

    left_idx++;
    right_idx--;
  }

  std::cout << "palindrome: " << word << std::endl;
  return true;
}

int main() {
  /*
   *Read in the system dictionary, and print out the number of palindromic words
   */
  const auto words = readWords("/usr/share/dict/words");

  /*
   *Print out the number of palindromic words in this container
   */
  std::cout << std::count_if(
                   words.begin(), words.end(),
                   [](const std::string &word) { return isPalindromic(word); })
            << std::endl;
  ;
}
