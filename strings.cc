#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <vector>

using namespace std;

bool isPalindrone(char* str, int len)
{
    if (len == 0) return false;
    for (int i = 0; i<len / 2; ++i)
    {
        if (str[i] != str[len - i - 1])
        {
            return false;
        }
    }
    return true;
}


int numPalindromes(string filename)
{
    ifstream inFile(filename.c_str(), ifstream::in);

    if (!inFile.good())
    {
        cout << "Unable to access file: " << filename.c_str() << endl;
        return 0;
    }

    char buffer[64];
    int len = 0;
    char ch;

    int numPalindromes = 0;

    while (inFile.good())
    {
        ch = inFile.get();
        if (isspace(ch) || ch == '\n' || ch == '\0' || ch == -1)
        {
            if (len>0)
            {
                numPalindromes += isPalindrone(buffer, len) ? 1 : 0;
                len = 0;
            }
        }
        else
        {
            buffer[len++] = ch;
        }
    }
    //handle last buffer
    numPalindromes += isPalindrone(buffer, len) ? 1 : 0;

    inFile.close();
    return numPalindromes;
}



int main(int argc, char** argv)
{
    string filename = "/usr/share/dict/words";
    if (argc >1)
    {
        filename = string(argv[1]);
    }
    cout << numPalindromes(filename);
}
