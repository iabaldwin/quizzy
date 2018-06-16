#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <functional>

bool isPalindromic(const std::string &word) {
    // check if the word is a palindrome
    int l = 0;
    int r = word.size() - 1;
    while (l < r) {
        if (word[l++] != word[r--]) return false;
    }
    return true;
}

std::vector<std::string> readWords(const std::string &path, const std::function<bool (std::string)> & filter) {
    // read words line by line from a file
    // apply filter function to check if a word should be in the final output
    // to reduce the memory needed
    std::vector<std::string> wList;
    std::ifstream input(path);
    for (std::string line; getline(input, line);){
        if (filter(line)) wList.push_back(line);
    }
    return wList;
}

int main() {
    /*
     *Read in the system dictionary, and print out the number of palindromic words
     */
    const std::string dictionary_path = "/usr/share/dict/words";
    const auto words = readWords(dictionary_path, isPalindromic);
    /*
     *Print out the number of palindromic words in this container
     */

    std::cout << dictionary_path << " has " << words.size() << " palindromic words " << std::endl;

}
