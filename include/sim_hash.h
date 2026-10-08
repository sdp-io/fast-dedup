#pragma once

#include <nanobind/nanobind.h> // Silence pip redefinition warnings

#define XXH_INLINE_ALL
#include <array>
#include <cstdint>
#include <string_view>
#include <xxhash.h>

namespace fast_dedup
{

class SimHash
{

public:
  explicit SimHash(uint64_t seed);

  void update(std::string_view shingle, const int64_t weight);

  uint64_t fingerprint() const noexcept;

  static size_t hamming_distance(uint64_t fingerprint1, uint64_t fingerprint2) noexcept;

  size_t hamming_distance(const SimHash& other) const noexcept;

  bool is_empty() const noexcept;

  void clear() noexcept;

private:
  static constexpr size_t HASH_BITS{64};
  std::array<int64_t, HASH_BITS> weights{};
  XXH64_hash_t seed{};
};

} // namespace fast_dedup
