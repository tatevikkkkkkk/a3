#ifndef HASHFUNCGEN_H
#define HASHFUNCGEN_H

#include <cstdint>
#include <string>

class HashFuncGen {
private:
    uint32_t seed;

public:
    HashFuncGen(uint32_t seed = 0x12345678) : seed(seed) {}

    uint32_t hash(const std::string &key) const {
        const uint32_t c1 = 0xcc9e2d51;
        const uint32_t c2 = 0x1b873593;

        uint32_t hash_value = seed;
        const uint8_t *data = reinterpret_cast<const uint8_t *>(key.c_str());
        size_t len = key.length();

        const size_t nblocks = len / 4;
        const uint32_t *blocks = reinterpret_cast<const uint32_t *>(data);

        for (size_t i = 0; i < nblocks; ++i) {
            uint32_t k = blocks[i];
            k *= c1;
            k = (k << 15) | (k >> 17);
            k *= c2;
            hash_value ^= k;
            hash_value = ((hash_value << 13) | (hash_value >> 19)) * 5 + 0xe6546b64;
        }

        const uint8_t *tail = data + nblocks * 4;
        uint32_t k1 = 0;

        switch (len & 3) {
            case 3:
                k1 ^= tail[2] << 16;
            case 2:
                k1 ^= tail[1] << 8;
            case 1:
                k1 ^= tail[0];
                k1 *= c1;
                k1 = (k1 << 15) | (k1 >> 17);
                k1 *= c2;
                hash_value ^= k1;
        }

        hash_value ^= len;
        hash_value ^= (hash_value >> 16);
        hash_value *= 0x85ebca6b;
        hash_value ^= (hash_value >> 13);
        hash_value *= 0xc2b2ae35;
        hash_value ^= (hash_value >> 16);

        return hash_value;
    }
};

#endif
