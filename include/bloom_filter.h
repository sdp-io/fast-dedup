#pragma once

#include "xxhash.h"
#include <atomic>
#include <cstddef>
#include <memory>
#include <string>

namespace fast_dedup
{

class BloomFilter
{

public:
  BloomFilter(size_t expected_elements, double error_rate);

  BloomFilter(const BloomFilter&) = delete;

  BloomFilter& operator=(const BloomFilter&) = delete;

  BloomFilter(BloomFilter&&) noexcept = default;

  BloomFilter& operator=(BloomFilter&&) noexcept = default;

  ~BloomFilter() = default;

  void add(const std::string& element);

  bool contains(const std::string& element) const;

  void clear();

  size_t size_in_bytes() const noexcept;

private:
  size_t ideal_k{};
  size_t ideal_bits{};
  size_t num_blocks{};
  std::unique_ptr<std::atomic_uint64_t[]> bloom_filter{};
  XXH64_hash_t seed{1};
};

}; // namespace fast_dedup
