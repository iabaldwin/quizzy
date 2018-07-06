#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <cstdint>

namespace file_tools {
///Signature is different from suggested to remove copy of vector
int readWords(const std::string& filePath, std::vector<std::string>& words) {
    std::ifstream fileStream(filePath.c_str());
    if(!fileStream) {
        //file could not be opened for reading
        return 1;
    }
    //getline will be used because need to read empty lines too, they are palindroms
    std::string currWord("");
    while(std::getline(fileStream, currWord)) {
        words.push_back(currWord);
    }
    return 0;
}

bool isPalindromic(const std::string& word) {
    if(word.find_first_of(' ') != std::string::npos) {
        return false; //string with space in it is not a word
    }

    std::string::const_iterator wordIt = word.begin(); //forward iterator from the first symbol
    std::string::const_reverse_iterator rWordIt = word.rbegin(); //backward iterator from the last symbol
    std::string::const_iterator middleSybmIt = word.begin() + word.size() / 2; //iterator to middle symbol
    while(wordIt != middleSybmIt) {
        if(*wordIt != *rWordIt) {
            return false;
        }
        ++wordIt;
        ++rWordIt;
    }
    return true;
}
} //file_tools

int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cout << "Incorrect input. Please enter a file with list of words" << std::endl;
        return 1;
    }
    std::string filePath(argv[1]);
    std::vector<std::string> words;

    if(file_tools::readWords(filePath, words) != 0) {
        std::cout << "File could not be read. Please make sure path is correct and file is readable." << std::endl;
        return 1;
    }

    int64_t palindromicCount(0); //type is 64bit because of vector<string> max_size on 64bit platform is 2^61
    std::vector<std::string>::const_iterator wordsIt = words.begin();
    for( ; wordsIt != words.end(); ++wordsIt) {
        if(file_tools::isPalindromic(*wordsIt)) {
            ++palindromicCount;
        }
    }
    
    std::cout << palindromicCount << std::endl;
    return 0;
}
