import sys
sys.path.insert(0, '../build')

import sdsl4py
import pytest
import re

#@pytest.mark.parametrize("vector_type, vector_size", [sdsl4py.int_vector, sdsl4py.int_vector_8,
#                                        sdsl4py.int_vector_16, sdsl4py.int_vector_32, 
#                                        sdsl4py.int_vector_64])

@pytest.mark.parametrize("vector_width", [i for i in range(1, 65)])
def test_int_vector(vector_width):
    VECTOR_SIZE = 5
    v = sdsl4py.int_vector(0, 0, vector_width)
    assert v.width() == vector_width
    assert not v
    v.resize(VECTOR_SIZE)
    assert v.size() == VECTOR_SIZE
    assert len(v) == VECTOR_SIZE
    assert v[0] == 0
    l = [0, 1, 2**vector_width - 1, 2**(vector_width - 1), 0]
    for i in range(VECTOR_SIZE):
        v[i] = l[i]
        assert v[i] == l[i]
    assert max(v) == max(l)
    assert min(v) == min(l)
    assert sum(v) == sum(l)

@pytest.mark.parametrize("vector_type", [sdsl4py.int_vector_8, sdsl4py.int_vector_16, sdsl4py.int_vector_32, sdsl4py.int_vector_64])
def test_int_vector_w(vector_type):
    VECTOR_SIZE = 5
    vector_width = int(re.search(r'\d+', vector_type.__name__).group())
    v = vector_type(0, 0)
    assert v.width() == vector_width
    assert not v
    v.resize(VECTOR_SIZE)
    assert v.size() == VECTOR_SIZE
    assert len(v) == VECTOR_SIZE
    assert v[0] == 0
    l = [0, 1, 2**vector_width - 1, 2**(vector_width - 1), 0]
    for i in range(VECTOR_SIZE):
        v[i] = l[i]
        assert v[i] == l[i]
    assert max(v) == max(l)
    assert min(v) == min(l)
    assert sum(v) == sum(l)

def test_bit_vector():
    VECTOR_SIZE = 5
    v = sdsl4py.bit_vector(0, 0)
    assert v.width() == 1
    assert not v
    v.resize(VECTOR_SIZE)
    assert v.size() == VECTOR_SIZE
    assert len(v) == VECTOR_SIZE
    assert v[0] == 0
    l = [0, 1, 0, 1, 0]
    for i in range(VECTOR_SIZE):
        v[i] = l[i]
        assert v[i] == l[i]
    v.flip()
    for i in range(VECTOR_SIZE):
        assert v[i] == 1 - l[i]
    assert max(v) == max(l)
    assert min(v) == min(l)
    assert sum(v) == len(l) - sum(l)