#include "bloom_filter.h"
#include <algorithm>
#include <cmath>
#include <cstdint>

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

// TODO: Write documentation for Kirsch + Mitzenmacher optimization
void BloomFilter::add(const std::string& element)
{
  XXH128_hash_t hash_128 = XXH3_128bits_withSeed(element.data(), element.size(), seed);

  uint64_t hash1{hash_128.low64};
  uint64_t hash2{hash_128.high64};

  for (size_t i{0}; i < ideal_k; ++i) {
    uint64_t combined_hash{hash1 + (hash2 * i)};

    uint64_t bit_index{combined_hash % ideal_bits};
    uint64_t block{bit_index / 64};
    uint64_t bit_pos{bit_index % 64};

    bloom_filter[block] |= (1ULL << bit_pos);
  }
}

bool BloomFilter::contains(const std::string& element) const
{
  XXH128_hash_t hash_128 = XXH3_128bits_withSeed(element.data(), element.size(), seed);

  uint64_t hash1{hash_128.low64};
  uint64_t hash2{hash_128.high64};

  for (size_t i{0}; i < ideal_k; ++i) {
    uint64_t combined_hash{hash1 + (hash2 * i)};

    uint64_t bit_index{combined_hash % ideal_bits};
    uint64_t block{bit_index / 64};
    uint64_t bit_pos{bit_index % 64};

    // Bit to check is not flipped, Bloom filter does not contain element
    if (!(bloom_filter[block] & 1ULL << bit_pos)) {
      return false;
    }
  }

  return true;
}

size_t BloomFilter::size_in_bytes() const noexcept
{ return bloom_filter.size() * sizeof(uint64_t) + sizeof(BloomFilter); }

} // namespace BloomFilter
