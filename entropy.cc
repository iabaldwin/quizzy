#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

std::vector<int> getRandomNumbers(const std::string& file_descriptor, const int num_samples) {
    std::ifstream input_stream(file_descriptor);
    std::vector<int> random_numbers;
    unsigned char cur_char;

    for (int i = 0; i < num_samples; i++) {
        input_stream >> cur_char; 
        random_numbers.push_back(static_cast<int>(cur_char % 128));
    }

    return random_numbers;
}

template <typename NumericType, int Discretization>
NumericType entropy(const std::string& file_descriptor) {
    NumericType entropy = 0.0;
    const int num_samples = 10000;
    const float num_samples_f = static_cast<float>(num_samples);
    int bin;

    // handle case where discretization doesn't perfectly divide input space
    int num_events = 128 / Discretization;
    if (num_events * Discretization < 128) {
        num_events++;
    }

    std::vector<int> event_counts(num_events);
    auto random_numbers = getRandomNumbers(file_descriptor, num_samples);

    // count number of each event
    for (auto number : random_numbers) {
        bin = number / Discretization;
        event_counts[bin]++;
    }

    // compute entropy
    NumericType event_prob;
    for (int event_idx = 0; event_idx < num_events; event_idx++) {
        event_prob = static_cast<NumericType>(event_counts[event_idx] / num_samples_f);
        entropy -= (event_prob * std::log(event_prob));
    }

    return entropy;
}

int main() {
  /*
   *Print out the entropy of a probability distribution over 8 discrete events,
   with event occurrences given by a random source of information. 
   */
  std::cout << "d = 8, entropy = " << entropy<float, 8>("/dev/random") << std::endl;
  std::cout << "d = 7, entropy = " << entropy<double, 7>("/dev/random") << std::endl;
  std::cout << "d = 64, entropy = " << entropy<float, 64>("/dev/random") << std::endl;

}
