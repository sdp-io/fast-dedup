#pragma once

// clang-format off
#include <nanobind/nanobind.h> // Silence pip redefinition warnings
// clang-format on

#define XXH_INLINE_ALL
#include <xxhash.h>

namespace fast_dedup
{

class MinHash
{

public:
  MinHash(int num_perm) : num_permutations{num_perm} {}

  void update(std::string_view data);

  double jaccard(MinHash& set);

  bool is_empty();

  void clear();

  MinHash merge(MinHash& set);

private:
  int num_permutations{};
  XXH64_hash_t seed{1};
};

} // namespace fast_dedup
