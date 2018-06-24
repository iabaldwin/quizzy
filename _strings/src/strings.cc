
//
// \file    strings.cc
// \author  Sergio Valero, esevalero@gmail.com
// \date    21.06.2018
//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// \brief:  This program that takes a path to a file containing a list        //
//          of words (e.g. "/usr/share/dict) and count the number of          //
//          palindromic words in the list. A palindromic word (for this       //
//          problem) is a word that reads the same in the forwards and reverse//
//          order, and is case-sensitive (i.e. Z != z). Report total count of //
//          palindromic words                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <sys/stat.h>
#include <algorithm>


//------------------------------------------------------------------------------
// In line Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// \brief   File sanity check inline function
//
inline bool isFile (
    const std::string&  name        //< String with the name of the file to check
){
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

//------------------------------------------------------------------------------
// \brief       Insert path of the text file to analize
//
// \return      String container to the file path
//
const std::string inserPathToFile(
    void
){
    std::string path;
    std::cout << "_____________________________________________________" << std::endl << std::endl;
    std::cout << "Hey!" << std::endl;
    std::cout << "I can detect the palindromic words in a text! " << std::endl << std::endl ;
    std::cout << "Could you please insert the path to your file? " << std::endl;
    std::cout << "*****************************************************" << std::endl;
    std::cout << "Insert the path to your file: " ;
    std::cin >> path;
    std::cout << std::endl;

    bool fileOK = isFile(path);

    if (!fileOK){
        std::cout << "Error: The File doesn´t exist." << std::endl;
        assert( fileOK && "Error: The File doesn´t exist.") ;
    }

    return path;
}


//------------------------------------------------------------------------------
// \brief   this function take a text file as a input and return a vector of
//          strings with al the words one by one.
//
const auto readWords(
    std::string filePath    //< String with the path to the file
){
    std::vector<std::string> words;
    std::string str;

    std::ifstream fin(filePath);

    for (int i = 0; (fin >> str); ++i)
        words.push_back(str);

    fin.close();

    return words;
}


//------------------------------------------------------------------------------
// \brief   Detect if a word is palindromic
//
bool isPalindromic(
    std::string word    //< word to analize as a palindromic
){
    int start = 0,
    int end   = word.length() - 1;
    while (start < end) {
        if ( std::toupper(word[start++]) != std::toupper(word[end--]) )
            return false;
    }

    return true;
}



//------------------------------------------------------------------------------
// Main
//
int main() {

    //--------------------------------------------------------------------------
    // Ask to insert file
    std::string filePath = inserPathToFile();

    //--------------------------------------------------------------------------
    // Reading the all the words in a text file. The words are separated
    // by spaces
    const auto words = readWords(filePath);

    // count the palindromic words in the file
    std::cout << "Total number of palindromic words in the file: " << std::count_if( words.begin(),
                                words.end(),
                                [](const std::string & word) { return isPalindromic(word); }
                              ) << std::endl;

    // print palindromic words
    std::cout << "------------------------------------------------" << std::endl;
    for (int i = 0; i < words.size(); ++i)
        if (isPalindromic(words[i]))
            std::cout << "This is a Palindromic Word:    " << words[i] << std::endl;

    return 0;

}
