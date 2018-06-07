#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <string>

using std::string;
using std::vector;
using std::ifstream;

// Low case or upper case??

bool isPalindromic(string s) {
    int front = 0;
    int end = s.length()-1;
    while (front < end) {
        if (s[front] != s[end])
            return false;
        
        front++;
        end--;
    }
    
    return true;
}

vector<string> readWords(const string &fp) {
    ifstream ifs(fp);
    if (!ifs.is_open())
        return {};
    vector<string> res;
    string line;
    while (std::getline(ifs, line)) {
        res.push_back(line);
    }
    
    ifs.close();
    return res;
}

int main() {
  /*
   *Read in the system dictionary, and print out the number of palindromic words
   */
    const auto words = readWords("/usr/share/dict/words");
  /*
   *Print out the number of palindromic words in this container
   */
    //vector<string> words = {"ab", "aa"};
    std::cout << std::count_if(words.begin(),
                             words.end(),
                             [](const std::string & word) {
                              return isPalindromic(word);
                              }) << std::endl;;
  /*
   *Note: this is a suggested function signature, feel free to implement
   whatever you see fit!
   */
}
