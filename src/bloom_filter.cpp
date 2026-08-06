#include "bloom_filter.h"

namespace BloomFilter
{

BloomFilter::BloomFilter(size_t expected_elements, double error_rate)
{
  double ee_dbl = static_cast<double>(expected_elements);

  // Formula for size of bit array: m = -(n ln P) / (ln 2)^2  where n is num of expected elements
  double m = std::ceil(-((ee_dbl * log(error_rate) / pow(log(2), 2))));
  ideal_bits = static_cast<size_t>(m);

  // Calculate number of uint64_t blocks required to pack ideal num of bits into Bloom filter
  num_blocks = (ideal_bits + 64 - 1) / 64; // Ceiling division for integers

  // Account for any excess bits in num_blocks, Potentially decreases error rate whilst preventing
  // space wastage
  ideal_bits = num_blocks * 64;

  // Formula for optimum num of hashes: k = (m / n) * ln 2
  // Select either formula result or 1, as ideal num of hashes cannot be less than 1
  double k = std::max(1.0, floor((m / ee_dbl) * log(2)));
  ideal_k = static_cast<size_t>(k);

  bloom_filter.resize(num_blocks);
}

} // namespace BloomFilter
