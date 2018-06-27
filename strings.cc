#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>

static const std::string DICTIONARY_PATH = "/usr/share/dict/words";

/**
 * @brief Return true if the string is a palindrome, false otherwise.
 * @details Return true if the string is a palindrome. No verification is done
 * to determine if the input string is a word (e.g. "N" is a palindrome, but is
 * "N" a word?). All characters in the string are treated as part of the word; 
 * e.g. "a man a plan a canal panama" is a palindromic sentence, but only
 * after adjusting the spacing.
 * 
 * The solution is O(N) in the number of characters. It uses std::equal to 
 * compare the first half of the word to the second half in reverse order.
 * @see std::equal
 */
inline bool isPalindromic(const std::string& word){
    return std::equal(word.begin(), word.begin()+word.size()/2, word.rbegin());
}

int main() {
    // Read the dictionary file line by line because it could be too large to 
    // fit in memory. On Ubuntu 18.04 there are 102305 lines each containing 
    // one word.
    std::ifstream fs(DICTIONARY_PATH);
    std::string line;
    uint64_t count = 0;
    while (std::getline(fs, line)){
        if (isPalindromic(line)){
            count++;
        }
    }
    std::cout<<count<<std::endl;
    return 0;
}
