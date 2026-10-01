#include "min_hash.h"
#include <algorithm>
#include <cstdint>
#include <random>

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
    // NOTE: Ensure A's random num is always odd, so that permuted hashes properly
    // distribute over the entire uint64_t range (multiplication with odds versus evens)
    uint64_t rand_a{distrib(gen) | 1ULL};

    A.push_back(rand_a);
    B.push_back(distrib(gen));
  }
}

void MinHash::clear()
{ min_hash.assign(num_perm, UINT64_MAX); }

bool MinHash::is_empty() const noexcept
{
  return std::all_of(min_hash.cbegin(), min_hash.cend(),
                     [](uint64_t i) { return i == UINT64_MAX; });
}

void MinHash::update(std::string_view shingle)
{
  uint64_t raw_hash{XXH3_64bits_withSeed(shingle.data(), shingle.size(), seed)};

  for (size_t i{0}; i < num_perm; ++i) {
    // NOTE: Due to integer overflow handling the modulos is implied, making this expression
    // equivalent to: (X * A + B) % 2^64
    uint64_t permuted_hash{raw_hash * A[i] + B[i]};

    min_hash[i] = std::min(permuted_hash, min_hash[i]);
  }
}

double jaccard(MinHash& set) const {}

MinHash merge(MinHash& set) const {}

} // namespace fast_dedup
