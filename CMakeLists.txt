cmake_minimum_required(VERSION 3.15...4.4)

project(fast_dedup LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT SKBUILD)
  message(WARNING "\
  This CMake file is meant to be executed using 'scikit-build'. Running
  it directly will almost certainly not produce the desired result. If
  you are a user trying to install this package, please use the command
  below, which will install all necessary build dependencies, compile
  the package in an isolated environment, and then install it.
  =====================================================================
   $ pip install fast-dedup
  =====================================================================
  If you are a software developer, and this is your own package, then
  it is usually much more efficient to install the build dependencies
  in your environment once and use the following command that avoids
  a costly creation of a new virtual environment at every compilation:
  =====================================================================
   $ pip install nanobind scikit-build-core[pyproject]
   $ pip install --no-build-isolation -ve .
  =====================================================================
  You may optionally add -Ceditable.rebuild=true to auto-rebuild when
  the package is imported. Otherwise, you need to re-run the above
  after editing C++ files.")
endif()

find_package(Python 3.10
  REQUIRED COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)

find_package(nanobind CONFIG REQUIRED)

# Compile the extension module through nanobind
nanobind_add_module(
  # Extension name
  fast_dedup_ext

  # Target the stable ABI for Python 3.12+, reducing
  # the number of binary wheels that must be built.
  # This does nothing on older Python versions
  STABLE_ABI

  # Build libnanobind statically and merge it into
  # the extension (which itself remains a shared libary)
  NB_STATIC

  src/fast_dedup_ext.cpp
  src/bloom_filter.cpp
)

if (MSVC)
  target_compile_options(fast_dedup_ext PRIVATE /W4)
else()
  target_compile_options(fast_dedup_ext PRIVATE
  -Wall
  -Wextra
  -Wpedantic
  -Wconversion)
endif()

target_sources(fast_dedup_ext PRIVATE
    src/fast_dedup_ext.cpp
    src/bloom_filter.cpp
    include/bloom_filter.h
)

target_include_directories(fast_dedup_ext PRIVATE include/)

install(TARGETS fast_dedup_ext LIBRARY DESTINATION fast_dedup)
