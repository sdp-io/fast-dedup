import math

import pytest

import fast_dedup as fd


def test_mh_clear():
    min_hash = fd.min_hash()

    assert min_hash.is_empty()

    hello = "hello"
    min_hash.update(hello)
    assert not min_hash.is_empty()

    min_hash.clear()
    assert min_hash.is_empty()


def test_mh_jaccard_full_match():
    min_hash1 = fd.min_hash()
    min_hash2 = fd.min_hash()

    hello = "hello"
    min_hash1.update(hello)
    min_hash2.update(hello)

    # NOTE: As the statistical variance for a MinHash Jaccard estimate is always equal to 0
    # when the Jaccard index is equal to 1.0, we can make a direct comparison.
    jaccard = min_hash1.jaccard(min_hash2)
    assert jaccard == 1.0


def test_mh_jaccard_half_match():
    min_hash1 = fd.min_hash()
    min_hash2 = fd.min_hash()

    hello = "hello"
    min_hash1.update(hello)
    min_hash2.update(hello)

    goodbye = "goodbye"
    min_hash1.update(goodbye)

    jaccard = min_hash1.jaccard(min_hash2)

    # Calculate sqrt((J * 1 - J) / k) where J is the Jaccard index, and k is the number of permutations
    standard_error = math.sqrt(0.50 * (1 - 0.50) / 128)
    three_sds = standard_error * 3

    assert jaccard == pytest.approx(0.50, abs=three_sds)


def test_mh_jaccard_no_match():
    min_hash1 = fd.min_hash()
    min_hash2 = fd.min_hash()

    hello = "hello"
    min_hash1.update(hello)

    goodbye = "goodbye"
    min_hash2.update(goodbye)

    jaccard = min_hash1.jaccard(min_hash2)

    # NOTE: As the statistical variance for a MinHash Jaccard estimate is always equal to 0
    # when the Jaccard index is equal to 0.0, we can make a direct comparison.
    assert jaccard == 0.0


def test_mh_merge():
    min_hash1 = fd.min_hash()
    min_hash2 = fd.min_hash()

    hello = "hello"
    min_hash1.update(hello)

    goodbye = "goodbye"
    min_hash2.update(goodbye)

    merged = min_hash1.merge(min_hash2)

    min_hash3 = fd.min_hash()
    min_hash3.update(hello)
    min_hash3.update(goodbye)

    merged_jaccard = merged.jaccard(min_hash3)

    # NOTE: As the statistical variance for a MinHash Jaccard estimate is always equal to 0
    # when the Jaccard index is equal to 1.0, we can make a direct comparison.
    assert merged_jaccard == 1.0
