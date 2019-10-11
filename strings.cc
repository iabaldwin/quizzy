#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <string>
#include <streambuf>
#include <sstream>
#include <algorithm>

//read words from a given filename
std::vector< std::string > readWords(const std::string& filename){
	std::ifstream t(filename);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    std::string tmp;
    std::stringstream str_strm(str);
    std::vector< std::string > words;
    
    while (str_strm >> tmp) {
        if (!tmp.empty()) words.push_back(tmp);
    }
   	return words;
}

//check if a given word is a palindrome
bool isPalindromic(const std::string& s){
	return equal(s.begin(), s.begin() + s.size()/2, s.rbegin());
}

int main() {
  /*
   *Read in the system dictionary, and print out the number of palindromic words
   */
	const auto words = readWords("/usr/share/dict/words");
  /*
   *Print out the number of palindromic words in this container
   */
	std::cout << std::count_if(words.begin(), words.end(), [](const std::string & word) {
    															return isPalindromic(word);
    														}) << std::endl;;
   return 0;
}
