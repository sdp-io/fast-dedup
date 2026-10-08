#pragma once

#include <nanobind/nanobind.h> // Silence pip redefinition warnings

#define XXH_INLINE_ALL
#include <cstdint>
#include <vector>
#include <xxhash.h>

namespace fast_dedup
{

class MinHash
{

public:
  explicit MinHash(const size_t& num_permutations = 128, const uint64_t& seed = 1);

  void update(std::string_view data);

  double jaccard(const MinHash& set) const;

  MinHash merge(const MinHash& set) const;

  bool is_empty() const noexcept;

  void clear();

private:
  std::vector<uint64_t> min_hash{};
  std::vector<uint64_t> A{};
  std::vector<uint64_t> B{};
  size_t num_perm{};
  size_t seed{};
  XXH64_hash_t hash_seed{seed};
};

} // namespace fast_dedup
