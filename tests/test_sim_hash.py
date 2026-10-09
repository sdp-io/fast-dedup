import xxhash

import fast_dedup as fd


def test_sh_clear_and_is_empty():
    sim_hash = fd.sim_hash()
    assert sim_hash.fingerprint() == 0 and sim_hash.is_empty()

    hello = "hello"

    sim_hash.update(hello, 1)
    assert sim_hash.fingerprint() != 0 and not sim_hash.is_empty()

    sim_hash.clear()

    assert sim_hash.fingerprint() == 0 and sim_hash.is_empty()


def test_sh_update():
    sim_hash = fd.sim_hash()

    hello = "hello"
    sim_hash.update(hello, 1)

    expected_hash = xxhash.xxh3_64_intdigest(hello.encode(), seed=1)
    assert sim_hash.fingerprint() == expected_hash


def test_sh_hamming_distance():
    sim_hash1 = fd.sim_hash()
    sim_hash2 = fd.sim_hash()

    hello = "hello"
    sim_hash1.update(hello, 1)
    sim_hash2.update(hello, 1)

    assert sim_hash1.hamming_distance(sim_hash2) == 0

    goodbye = "goodbye"
    sim_hash2.update(goodbye, 1)

    assert sim_hash1.hamming_distance(sim_hash2) > 0

    # Testing static Hamming distance function
    assert fd.sim_hash.hamming_distance(0b1111, 0b1100) == 2
    assert fd.sim_hash.hamming_distance(0, 0xFFFFFFFFFFFFFFFF) == 64
