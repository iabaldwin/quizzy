#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cmath>
#include <iostream>
#include <fstream>

//Notes:
//- Caller can specify the number of samples to collect to calculate the entropy
//- The function calculates entropy per byte. So the Range parameter should be within (0, 256]
//- Discretization allows you to split the range into events
//  For example: Discretization of 8 and Range of 128 will give you 16 events
//
template<typename NumericType, unsigned int Discretization, unsigned int Range>
NumericType entropy(const std::string &file_path, unsigned int samples) {
  int fd = open(file_path.c_str(), O_RDONLY);
  if(fd < 0) {
    fprintf(stderr, "Error: Could not open %s. %s\n", file_path.c_str(), strerror(errno));
    return -1;
  }

  static_assert((Range > 0), "Error: Range cannot be 0");

  //Stores number of bytes(samples) read
  unsigned int count = 0;

  constexpr unsigned int num_events = Range/Discretization;

  //Holds the count of each event
  int events[num_events] = {};

  while(count < samples) {
    //c stores the current random byte
    unsigned char c;

    if(read(fd, &c, 1) < 0) {
      fprintf(stderr, "Error: Could not read from file %s. %s\n", file_path.c_str(), strerror(errno));
      close(fd);
      return -1;
    }

    //Note: If Range can be guaranteed as a power of 2, this calulation can just be
    //      done using a bit mask
    unsigned int modval = c % Range;
    unsigned int event = modval / Discretization;
    events[event]++;
    count++;

    //When reading a large number of samples, the source might block.
    //Print current progress otherwise it may seem that the program is stuck
    printf("Progress: %d%c\r", (int)((float)count/samples * 100.0), '%');
    fflush(stdout);
  }
  printf("\n");

  close(fd);

  NumericType probability[num_events] = {};
  NumericType h = 0;
  for(unsigned int i = 0; i < num_events; i++) {
    probability[i] = (NumericType)events[i]/samples;
    NumericType l2 = std::log2(probability[i]);
    h += -1 * probability[i] * l2;

    //Uncomment for verbose info
    //printf("events[%d]: %d, prob[%d]: %f l2: %f h: %f\n", i, events[i], i, probability[i], l2, h);
  }

  return h;
}

int main(int argc, char **argv) {
  if(argc != 3) {
    fprintf(stderr, "Usage:   entropy <file path to source> <number of samples>\n");
    fprintf(stderr, "Example: entropy /dev/random 1000");
    return -1;
  }

  int samples = std::stoi(std::string(argv[2]));
  if(samples < 0) {
    fprintf(stderr, "Error: invalid sample size: %d\n", samples);
    return -1;
  }

  constexpr unsigned int discretization = 8;
  constexpr unsigned int range = 128;

  float h = entropy<float, discretization, range>(argv[1], (unsigned int)samples);

  printf("Entropy from %s for %d samples with dicretization: %u range: %u is %f\n",
         argv[1], discretization, range, samples, h);
  return 0;
}
