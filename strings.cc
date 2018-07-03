#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <string>

using namespace std;
int NumberOfPalindrome(std::string DictionaryPath)
{
    std::string test;
    int count = 0;
    ifstream ReadDict(DictionaryPath);
    while(ReadDict >> test)
    {
       count += std::equal(test.begin(), test.begin() + test.size() / 2, test.rbegin()); 
    }
    return count;
    ReadDict.close();
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
  std::string path = "words.txt";
  int PalindromeCount = NumberOfPalindrome(path);
  cout << "The number of palindromes in the dictionary are :"<<PalindromeCount<<endl;
}
