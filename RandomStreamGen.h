#ifndef RANDOMSTREAMGEN_H
#define RANDOMSTREAMGEN_H

#include <random>
#include <string>
#include <vector>

class RandomStreamGen {
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> len_dist;
    std::uniform_int_distribution<int> char_dist;
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-";

public:
    RandomStreamGen(unsigned seed = 42) : rng(seed), len_dist(1, 30), char_dist(0, 62) {}

    std::string generateString() {
        int length = len_dist(rng);
        std::string result;
        for (int i = 0; i < length; ++i) {
            result += charset[char_dist(rng)];
        }
        return result;
    }

    std::vector<std::string> generateStream(size_t size) {
        std::vector<std::string> stream;
        for (size_t i = 0; i < size; ++i) {
            stream.push_back(generateString());
        }
        return stream;
    }
};

#endif
