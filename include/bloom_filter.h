#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace BloomFilter
{

class BloomFilter
{

public:
  BloomFilter(size_t expected_elements, double error_rate) {}

  void add(std::string& element);

  bool contains(std::string& element);

  void clear();

  size_t size_in_bytes();

private:
  std::vector<uint64_t> bloom_filter{};
};

}; // namespace BloomFilter
