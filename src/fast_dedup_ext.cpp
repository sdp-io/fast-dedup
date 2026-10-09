#include "bloom_filter.h"
#include "min_hash.h"
#include "sim_hash.h"
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/string_view.h>

namespace nb = nanobind;

using namespace nb::literals;

using namespace fast_dedup;

// clang-format off
NB_MODULE(fast_dedup_ext, m)
{
  // TEST: Temp testing for development purposes
  m.doc() = "This is a \"hello world\" example with nanobind";
  m.def("add", [](int a, int b) { return a + b; }, "a"_a, "b"_a);

  nb::class_<BloomFilter>(m, "BloomFilter")
      .def(nb::init<size_t, double>())
      .def("add", &BloomFilter::add, nb::call_guard<nb::gil_scoped_release>())
      .def("contains", &BloomFilter::contains, nb::call_guard<nb::gil_scoped_release>())
      .def("clear", &BloomFilter::clear)
      .def("size_in_bytes", &BloomFilter::size_in_bytes);

  nb::class_<MinHash>(m, "MinHash")
      .def(nb::init<size_t, uint64_t>(), "num_permutations"_a = 128, "seed"_a = 1)
      .def("update", &MinHash::update, nb::call_guard<nb::gil_scoped_release>())
      .def("jaccard", &MinHash::jaccard, nb::call_guard<nb::gil_scoped_release>())
      .def("merge", &MinHash::merge, nb::call_guard<nb::gil_scoped_release>())
      .def("is_empty", &MinHash::is_empty)
      .def("clear", &MinHash::clear);

  nb::class_<SimHash>(m, "SimHash")
      .def(nb::init<uint64_t>(), "seed"_a = 1)
      .def("update", &SimHash::update, nb::call_guard<nb::gil_scoped_release>())
      .def("fingerprint", &SimHash::fingerprint)
      .def("hamming_distance", nb::overload_cast<uint64_t, uint64_t>(&SimHash::hamming_distance))
      .def("hamming_distance", nb::overload_cast<const SimHash&>(&SimHash::hamming_distance, nb::const_))
      .def("is_empty", &SimHash::is_empty)
      .def("clear", &SimHash::clear);
}
// clang-format off
