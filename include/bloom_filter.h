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

  bool contains(const std::string& element);

  void clear();

  size_t size_in_bytes();

private:
  size_t ideal_k{};
  size_t ideal_bits{};
  size_t num_blocks{};
  std::vector<uint64_t> bloom_filter{};
  XXH64_hash_t seed{1};
};

}; // namespace BloomFilter
