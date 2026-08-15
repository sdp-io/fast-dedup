#include "bloom_filter.h"
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

using namespace nb::literals;

using namespace fast_dedup;

NB_MODULE(fast_dedup_ext, m)
{
  // TEST: Temp testing for development purposes
  m.doc() = "This is a \"hello world\" example with nanobind";
  m.def("add", [](int a, int b) { return a + b; }, "a"_a, "b"_a);

  nb::class_<BloomFilter>(m, "bloom_filter")
      .def(nb::init<size_t, double>())
      .def("add", &BloomFilter::add, nb::call_guard<nb::gil_scoped_release>())
      .def("contains", &BloomFilter::contains, nb::call_guard<nb::gil_scoped_release>())
      .def("clear", &BloomFilter::clear)
      .def("size_in_bytes", &BloomFilter::size_in_bytes);
}
