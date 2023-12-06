import sys
sys.path.insert(0, '../build')

import sdsl4py
import pytest

@pytest.mark.parametrize("vector_type", [sdsl4py.enc_vector_elias_delta, sdsl4py.enc_vector_elias_gamma, sdsl4py.enc_vector_fibonacci, sdsl4py.enc_vector_comma_2,
                                         sdsl4py.vlc_vector_elias_delta, sdsl4py.vlc_vector_elias_gamma, sdsl4py.vlc_vector_fibonacci, sdsl4py.vlc_vector_comma_2,
                                         sdsl4py.dac_vector])
def test_encoded_vector(vector_type):
    v = sdsl4py.int_vector(1<<10)
    for i in range(1<<10):
        v[i] = i
    ev = vector_type(v)
    assert len(ev) == len(v)
    for i in range(1<<10):
        assert v[i] == ev[i]