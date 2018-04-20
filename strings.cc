#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <string>
#include <iterator>
#include <memory>
#include <algorithm>

std::vector<std::string> readWords(const std::string &path) {
    // Read file
    std::ifstream input(path);

    // Instantiate string vector using a smart pointer
    auto words = std::vector<std::string>();

    // Copy file lines into vector
    std::copy(std::istream_iterator<std::string>(input),
              std::istream_iterator<std::string>(),
              std::back_inserter(words));

    return words;
}

bool isPalindromic(const std::string &word) {
    return std::equal(word.begin(), word.begin() + word.size() / 2, word.rbegin());
}

int main() {

    using namespace std;

    /*
     *Read in the system dictionary, and print out the number of palindromic words
     */
    const auto words = readWords("/usr/share/dict/words");

    /*
     *Print out the number of palindromic words in this container
     */
    cout << count_if(words.begin(), words.end(),
                     [](const string &word) {
                         return isPalindromic(word);
                     })
         << endl;
    /*
     *Note: this is a suggested function signature, feel free to implement
     whatever you see fit!
     */
}
