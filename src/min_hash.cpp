#include "min_hash.h"
#include <algorithm>
#include <cstdint>
#include <random>
#include <stdexcept>

#define XXH_INLINE_ALL
#include <xxhash.h>

namespace fast_dedup
{

MinHash::MinHash(const size_t& num_permutations, const size_t& seed)
    : num_perm{num_permutations}, seed{seed}
{
  min_hash.assign(num_perm, UINT64_MAX);

  std::mt19937_64 gen(seed);
  std::uniform_int_distribution<uint64_t> distrib(0, UINT64_MAX);

  A.reserve(num_perm);
  B.reserve(num_perm);

  for (size_t i{0}; i < num_perm; ++i) {
    // NOTE: Ensures A's random num is always odd, so that permuted hashes properly
    // distribute over the entire uint64_t range (multiplication with odds versus evens)
    uint64_t rand_a{distrib(gen) | 1ULL};

    A.push_back(rand_a);
    B.push_back(distrib(gen));
  }
}

void MinHash::update(std::string_view shingle)
{
  uint64_t raw_hash{XXH3_64bits_withSeed(shingle.data(), shingle.size(), seed)};

  for (size_t i{0}; i < num_perm; ++i) {
    // NOTE: Due to integer overflow handling modulos is implied, making this expression
    // equivalent to: (X * A + B) % 2^64
    uint64_t permuted_hash{raw_hash * A[i] + B[i]};

    min_hash[i] = std::min(permuted_hash, min_hash[i]);
  }
}

double MinHash::jaccard(const MinHash& other_set) const
{
  if (num_perm != other_set.num_perm || seed != other_set.seed) {
    throw std::invalid_argument("MinHash mismatch: permutation count and seed must match.");
  }

  size_t matches{0};
  for (size_t i{0}; i < num_perm; ++i) {
    if (min_hash[i] == other_set.min_hash[i]) {
      ++matches;
    }
  }

  return static_cast<double>(matches) / static_cast<double>(num_perm);
}

MinHash MinHash::merge(const MinHash& other_set) const
{
  if (num_perm != other_set.num_perm || seed != other_set.seed) {
    throw std::invalid_argument("MinHash mismatch: permutation count and seed must match.");
  }

  MinHash mh_copy{*this};
  for (size_t i{0}; i < num_perm; ++i) {
    mh_copy.min_hash[i] = std::min(mh_copy.min_hash[i], other_set.min_hash[i]);
  }

  return mh_copy;
}

bool MinHash::is_empty() const noexcept
{
  return std::all_of(min_hash.cbegin(), min_hash.cend(),
                     [](uint64_t i) { return i == UINT64_MAX; });
}

void MinHash::clear()
{ min_hash.assign(num_perm, UINT64_MAX); }

} // namespace fast_dedup
