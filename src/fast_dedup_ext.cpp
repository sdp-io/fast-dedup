#define XXH_INLINE_ALL
#include <nanobind/nanobind.h>

namespace nb = nanobind;

using namespace nb::literals;

NB_MODULE(fast_dedup_ext, m) {
  m.doc() = "This is a \"hello world\" example with nanobind";
  m.def("add", [](int a, int b) { return a + b; }, "a"_a, "b"_a);
}
