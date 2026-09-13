# fast-dedup
A performance-oriented text deduplication library written in C++20 with Python bindings via nanobind

Designed for processing and deduplicating large datasets, `fast-dedup` utilizes nanobind to
create binaries and wheels of minimal size whilst significantly increasing performance of
comparable libraries written in pure Python.

## Features

* **Bloom Filter:** Hardware-aligned, lock-free atomic bit-packing w/ 128-bit hashing via `xxHash`
* **MinHash:** (In Development)
* **SimHash:** (Planned)
* **Suffix Array:** (Planned)

## Building

### Prerequisites
* **C++20 Compiler:** GCC 11+, Clang 13+, or MSVC 2019+
* **Python:** 3.10 or higher
* (Build tools like `nanobind` and `scikit-build-core` are handled automatically via Python)

### Development Setup

1. **Clone the repository:**
   ```bash
   git clone https://github.com/sdp-io/fast-dedup.git
   cd fast-dedup
   ```

2. **Create and activate a virtual environment**
  ```bash
  python3 -m venv .venv
  source .venv/bin/activate  # On Windows: .venv\Scripts\activate
  ```

3. **Install build prerequisites**
  ```bash
  pip install nanobind "scikit-build-core[pyproject]" pytest
  ```

4. **Compile and install in editable development mode:**
  ```bash
  pip install --no-build-isolation -ve .
  ```

5. **Run the Python test suite:**
  ```bash
  pytest -s tests/
  ```
