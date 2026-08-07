#pragma once

#include "xxhash.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace BloomFilter
{

class BloomFilter
{

public:
  BloomFilter(size_t expected_elements, double error_rate);

  void add(const std::string& element);

  bool contains(const std::string& element) const;

  void clear();

  // Explicit copy method for Python users for more "Pythonic" usage
  BloomFilter copy() const;

  size_t size_in_bytes() const noexcept;

private:
  size_t ideal_k{};
  size_t ideal_bits{};
  size_t num_blocks{};
  std::vector<uint64_t> bloom_filter{};
  XXH64_hash_t seed{1};
};

}; // namespace BloomFilter
