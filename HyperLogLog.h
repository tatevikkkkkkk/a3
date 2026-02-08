#ifndef HYPERLOGLOG_H
#define HYPERLOGLOG_H

#include <algorithm>
#include <cmath>
#include <vector>
#include "HashFuncGen.h"

class HyperLogLog {
private:
    size_t B;
    size_t m;
    std::vector<uint8_t> registers;
    HashFuncGen hasher;

    uint8_t countLeadingZeros(uint32_t hash_value, size_t start_bit) const {
        uint32_t mask = 1U << (31 - start_bit);
        uint8_t count = 1;
        while ((hash_value & mask) == 0 && count <= (32 - start_bit)) {
            count++;
            mask >>= 1;
        }
        return count;
    }

    double getAlphaM() const {
        switch (m) {
            case 16:
                return 0.673;
            case 32:
                return 0.697;
            case 64:
                return 0.709;
            default:
                return 0.7213 / (1.0 + 1.079 / m);
        }
    }

public:
    HyperLogLog(size_t b = 10) : B(b), m(1ULL << b), hasher() { registers.resize(m, 0); }

    void add(const std::string &item) {
        uint32_t hash_value = hasher.hash(item);
        size_t index = hash_value >> (32 - B);
        uint8_t leading_zeros = countLeadingZeros(hash_value << B, B);
        registers[index] = std::max(registers[index], leading_zeros);
    }

    size_t estimate() const {
        double sum = 0.0;
        for (auto reg: registers) {
            sum += 1.0 / (1ULL << reg);
        }
        double raw_estimate = getAlphaM() * m * m / sum;

        if (raw_estimate <= 2.5 * m) {
            size_t zeros = std::count(registers.begin(), registers.end(), 0);
            if (zeros != 0) {
                return static_cast<size_t>(m * std::log(static_cast<double>(m) / zeros));
            }
        }

        if (raw_estimate > (1.0 / 30.0) * (1ULL << 32)) {
            return static_cast<size_t>(-1.0 * (1ULL << 32) * std::log(1.0 - raw_estimate / (1ULL << 32)));
        }

        return static_cast<size_t>(raw_estimate);
    }

    void reset() { std::fill(registers.begin(), registers.end(), 0); }
};

#endif
