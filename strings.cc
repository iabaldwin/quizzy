#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>

#include <string>

bool isPalindromic(const std::string &word)
{
  size_t full_sz = word.size();

  if(full_sz < 2) return false;

  size_t half_sz = full_sz / 2;

  for (size_t i = 0, j = word.size() - 1; i < half_sz; i++, j--)
    if(word[i] != word[j]) return false;
  return true;
}

int main() {
  /*
   *Read in the system dictionary, and print out the number of palindromic words
   */
  //const auto words = readWords("/usr/share/dict/words");
  /*
   *Print out the number of palindromic words in this container
   */
  //std::cout << std::count_if(words.begin(),
                             //words.end(),
                             //[](const std::string & word) {
                              //return isPalindromic(word);
                              //}) << std::endl;;
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */

  const char *file_name = "words.txt";
  std::ifstream ifs(file_name);
  std::string word;

  int n_palindroms = 0;

  while(ifs >> word)
    if(isPalindromic(word)) n_palindroms++;
    //std::cout << word << " " << isPalindromic(word) << std::endl;
  
  std::cout << file_name << " contains " << n_palindroms << " palindroms" << std::endl;

  return n_palindroms;
}
