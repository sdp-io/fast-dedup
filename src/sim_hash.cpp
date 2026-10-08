#include "sim_hash.h"
#include <algorithm>

namespace fast_dedup
{

SimHash::SimHash(uint64_t seed) : seed{seed} {}

void SimHash::update(std::string_view shingle, const int64_t weight)
{
  uint64_t hashed_val{XXH3_64bits_withSeed(shingle.data(), shingle.size(), seed)};

  for (size_t i{0}; i < HASH_BITS; ++i) {
    uint64_t observed_bit{(hashed_val >> i) & 1ULL};

    if (observed_bit) {
      weights[i] += weight;
    }
    else {
      weights[i] -= weight;
    }
  }
}

uint64_t SimHash::fingerprint() const noexcept
{
  uint64_t fingerprint{0};

  for (size_t i{0}; i < HASH_BITS; ++i) {
    if (weights[i] > 0) {
      fingerprint |= 1ULL << i;
    }
  }

  return fingerprint;
}

size_t SimHash::hamming_distance(uint64_t fingerprint1, uint64_t fingerprint2) noexcept
{
  // Static cast as Hamming distance can never be negative
  return static_cast<size_t>(__builtin_popcountll(fingerprint1 ^ fingerprint2));
}

size_t SimHash::hamming_distance(const SimHash& other) const noexcept
{ return hamming_distance(this->fingerprint(), other.fingerprint()); }

bool SimHash::is_empty() const noexcept
{
  return std::all_of(weights.cbegin(), weights.cend(), [](int64_t weight) { return weight == 0; });
}

void SimHash::clear() noexcept
{ weights.fill(0); }

} // namespace fast_dedup
