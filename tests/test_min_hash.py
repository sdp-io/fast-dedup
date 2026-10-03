import fast_dedup as fd


def test_mh_clear():
    min_hash = fd.min_hash()

    assert min_hash.is_empty()

    hello = "hello"
    min_hash.update(hello)
    assert not min_hash.is_empty()

    min_hash.clear()
    assert min_hash.is_empty()
