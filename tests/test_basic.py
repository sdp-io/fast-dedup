import fast_dedup as fd


# 'Hello World'-esque test to ensure nanobind is functioning properly
def test_add():
    assert fd.add(1, 2) == 3
