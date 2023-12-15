#ifndef ENC_VECTOR_CPP
#define ENC_VECTOR_CPP

#include <pybind11/pybind11.h>
#include <sdsl/vectors.hpp>
#include <sdsl/coder.hpp>

namespace py = pybind11;

using sdsl::enc_vector;
using sdsl::vlc_vector;
using sdsl::dac_vector;


template <class T>
inline auto add_enc_vector(py::module &m, const char* name){

    m.def("size_in_bytes", &sdsl::size_in_bytes<T>, py::arg("sdsl_object"));
    m.def("size_in_mega_bytes", &sdsl::size_in_mega_bytes<T>, py::arg("sdsl_object"));
    m.def("store_to_file", &sdsl::store_to_file<T>, py::arg("sdsl_object"), py::arg("file_name"));
    m.def("load_from_file", &sdsl::load_from_file<T>, py::arg("sdsl_object"), py::arg("file_name"));


    return py::class_<T>(m, name)
    .def(py::init([](const sdsl::int_vector<0> container){
        return new T(container);
    }), py::arg("container"))
    .def(py::init<>())
    .def("__getitem__", [](const T v, int index){
        return v[index];
    })
    .def("size", &T::size)
    .def("__len__", &T::size)
    .def("max_size", &T::max_size)
    .def("empty", &T::empty)
    .def("__bool__", [](const T &self){
        return !self.empty();
    })
    //TODO: swap method
    //TODO: load, serialize
    //TODO: sample
    //TODO: get_inter_sampled_values
    .def("__iter__", [](const py::sequence &self){
            return py::make_iterator(self.begin(), self.end());
        },
    py::keep_alive<0, 1>(), py::is_operator());
}

template <class T>
inline auto add_enc_vlc_vector(py::module &m, const char* name){
    add_enc_vector<T>(m, name).def("get_sample_dens", &T::get_sample_dens);
}

void enc_vector_wrapper(py::module &m){
    //TODO: Support variable density
    add_enc_vlc_vector<enc_vector<sdsl::coder::elias_delta, 128, 0>>(m, "enc_vector_elias_delta");
    add_enc_vlc_vector<enc_vector<sdsl::coder::elias_gamma, 128, 0>>(m, "enc_vector_elias_gamma");
    add_enc_vlc_vector<enc_vector<sdsl::coder::fibonacci, 128, 0>>(m, "enc_vector_fibonacci");
    add_enc_vlc_vector<enc_vector<sdsl::coder::comma<2>, 128, 0>>(m, "enc_vector_comma_2");

    add_enc_vlc_vector<vlc_vector<sdsl::coder::elias_delta, 128, 0>>(m, "vlc_vector_elias_delta");
    add_enc_vlc_vector<vlc_vector<sdsl::coder::elias_gamma, 128, 0>>(m, "vlc_vector_elias_gamma");
    add_enc_vlc_vector<vlc_vector<sdsl::coder::fibonacci, 128, 0>>(m, "vlc_vector_fibonacci");
    add_enc_vlc_vector<vlc_vector<sdsl::coder::comma<2>, 128, 0>>(m, "vlc_vector_comma_2");

    add_enc_vector<dac_vector<>>(m, "dac_vector");
}


#endif //ENC_VECTOR_CPP