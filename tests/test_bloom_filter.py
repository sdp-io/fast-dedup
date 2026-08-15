import fast_dedup as fd


def test_bf_add_valid():
    expected_elements = 1_000_000
    error_rate = 0.01  # 1% error rate

    bloom_filter = fd.bloom_filter(expected_elements, error_rate)

    hello = "Hello"
    world = "World!"

    bloom_filter.add(hello)
    bloom_filter.add(world)

    assert bloom_filter.contains(hello) and bloom_filter.contains(world)


def test_bf_add_invalid():
    expected_elements = 1_000_000
    error_rate = 0.01

    bloom_filter = fd.bloom_filter(expected_elements, error_rate)

    hello = "Hello"
    world = "World!"
    goodbye = "Goodbye"

    bloom_filter.add(hello)
    bloom_filter.add(world)

    assert not bloom_filter.contains(goodbye)
