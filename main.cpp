#include <cmath>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include "HyperLogLog.h"
#include "RandomStreamGen.h"

size_t countUnique(const std::vector<std::string> &stream) {
    std::unordered_set<std::string> unique(stream.begin(), stream.end());
    return unique.size();
}

int main() {
    const size_t B = 10;
    const size_t stream_size = 50000;
    const size_t num_streams = 10;
    const double step_percent = 10.0;

    std::ofstream results("results.csv");
    results << "stream_id,stream_size,true_count,estimated_count,error\n";

    for (size_t i = 0; i < num_streams; ++i) {
        RandomStreamGen gen(i);
        auto stream = gen.generateStream(stream_size);
        HyperLogLog hll(B);

        for (size_t j = 1; j <= 10; ++j) {
            size_t part_size = (stream_size * j) / 10;
            std::vector<std::string> part(stream.begin(), stream.begin() + part_size);

            hll.reset();
            for (const auto &s: part)
                hll.add(s);

            size_t true_count = countUnique(part);
            size_t estimate = hll.estimate();
            double error = std::abs((double) estimate - true_count) / true_count;

            results << i << "," << part_size << "," << true_count << "," << estimate << "," << error << "\n";
        }
    }

    results.close();
    std::cout << "Готово! Результаты в results.csv\n";
    return 0;
}
