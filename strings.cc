#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

// Alternate implementation
// #include <string>
// bool isPalindrome2(const std::string &w) {
//   return std::equal(w.begin(), w.end(), w.rbegin());
// }

bool isPalindrome(const char *word) {
  if(word == nullptr) {
    fprintf(stderr, "Error: null input parameter\n");
    return false;
  }

  unsigned int len = strlen(word);
  //Considering an empty string as a valid palindrome
  if(len < 2) {
    return true;
  }
  unsigned int beg = 0;
  unsigned int end = len-1;

  while(beg < end) {
    if(word[beg++] != word[end--]) {
      return false;
    }
  }
  return true;
}

//The words don't need to be stored. So I didn't use a vector to read and store the whole file.
//Instead the code reads a word and checks if it's a palindrome
int main(int argc, char *argv[]) {
  if(argc != 2) {
    fprintf(stderr, "Usage: strings <path to input file with one word per line>\n");
    return -1;
  }

  const char *file_path = argv[1];

  FILE *fp = fopen(file_path, "r");
  if(fp == nullptr) {
    fprintf(stderr, "Error: Could not open %s for reading. %s\n", file_path, strerror(errno));
    return -1;
  }

  unsigned int num_palindromes = 0;

  char *line = nullptr;
  size_t len = 0;

  while(getline(&line, &len, fp) != -1) {
    //replacing \n with \0
    line[strlen(line)-1] = '\0';

    if(isPalindrome(line)) {
      //Uncomment for verbose logs
      //printf("YES - %s\n", line);
      num_palindromes++;
      continue;
    }

    //Uncomment for verbose logs
    //printf("NO  - %s\n", line);
  }
  printf("Number of palindrome words: %u\n", num_palindromes);

  fclose(fp);
  free(line);
  return 0;
}
