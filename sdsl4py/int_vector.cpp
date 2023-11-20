#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <sdsl/int_vector.hpp>

namespace py = pybind11;

using sdsl::int_vector;

template <class T>
inline void add_intvector(py::module &m, const char* name){
    py::class_<T>(m, name, py::buffer_protocol())
        .def(py::init([](uint64_t size, uint64_t default_value, uint8_t int_width){
            return new T(size, default_value, int_width);
        }),R"doc(
            Constructor for int_vector
            
            Parameters:
                size (int): Number of elements. Default value is 0.
                default_value (int): Initialize all value to `default value`.
                int_width (int): The width of each integer.

            See Also:
                resize
                width
        )doc", py::arg("size") = 1, py::arg("default_value") = 0, py::arg("int_width") = 0)
        .def("__bool__", [](T &self){
            return !self.empty();
        })
        .def("empty", &T::empty, "True if vector is empty.")
        .def("swap", &T::swap, py::arg("v"), "Swap method for int_vector.")
        .def("resize", &T::resize, py::arg("size"), R"doc(
            Resize the int_vector in terms of elements.

            Parameters:
                size (int) The size to resize the int_vector in terms of elements.
        )doc")
        .def("bit_resize", &T::bit_resize, py::arg("size"), R"doc(
            Resize the int_vector in terms of bits.

            Parameters:
                size (int) The size to resize the int_vector in terms of bits.
        )doc")
        .def("size", &T::size, R"doc(
            The number of elements in the int_vector.

            See Also:
                max_size
                bit_size
                capacity
        )doc")
        .def("__len__", &T::size)
        .def("max_size", &T::max_size, R"doc(
            Maximum size of the int_vector.

            See Also:
                size
                bit_size
                capacity
        )doc")
        .def("bit_size", &T::bit_size, R"doc(
            The number of bits in the int_vector.

            See Also:
                size
                max_size
                bit_size
                capacity
        )doc")
        .def("capacity", &T::capacity, R"doc(
            Returns the size of the occupied bits of the int_vector.
            The capacity of a int_vector is greater or equal to the
            bit_size of the vector: capacity() >= bit_size().

            See Also:
                size
                bit_size
                max_size
                capacity
        )doc")
        //TODO, data pointer:
        /*
        .def("data", &T::data, py::return_value_policy::reference_internal, R"doc(
            Pointer to the raw data of the int_vector.

            Returns:
                pointer to the raw data of the int_vector
        )doc")
        */
        .def("get_int", &T::get_int, py::arg("idx"), py::arg("len") = 64, R"doc(
            Get the integer value of the binary string of length len starting at position idx in the int_vector.

            Parameters:
                idx (int): Starting index of the binary representation of the integer.
                len (int): Length of the binary representation of the integer. Default value is 64.

            Returns:
                The integer value of the binary string of length len starting at position idx.
            
            See Also:
                setInt
                getBit
                setBit
        )doc")
        .def("set_int", &T::set_int, py::arg("idx"), py::arg("x"), py::arg("len") = 64, R"doc(
            Set the bits from position idx to idx+len-1 to the binary representation of integer x.
            The bit at position idx represents the least significant bit(lsb), and the bit at position idx+len-1 the most significant bit (msb) of x.

            Parameters:
                idx (int): Starting index of the binary representation of x.
                x (int): The integer to store in the int_vector.
                len (int): Length of the binary representation of the integer. Default value is 64.

            Returns:
                The integer value of the binary string of length len starting at position idx.
            
            See Also:
                getInt
                getBit
                setBit
        )doc")
        .def("width", py::overload_cast<>(&T::width, py::const_), R"doc(
            Returns the width of the integers which are accessed via the [] operator.

            Returns:
                The width of the integers which are accessed via the [] operator.
            
            See Also:
                width
        )doc")
        .def("width", py::overload_cast<uint8_t>(&T::width), py::arg("new_width"), R"doc(
            Sets the width of the integers which are accessed via the [] operator.

            Parameters:
                intWidth (int): New width of the integers accessed via the [] operator.

            See Also:
                width
        )doc")
        .def("write_data", &T::write_data, py::arg("out"), "Write data to a stream")
        //TODO serialize funciton and serialize to buffer info
        //TODO load from stream function
        .def("__getitem__", [](const T &self, size_t index) {
            if (index >= self.size()) {
                throw py::index_error("Index out of bounds");
            }
            return self[index];
        })
        .def("__setitem__", [](T &self, size_t index, size_t value){
            if(index >= self.size()){
                throw py::index_error("Index out of bounds");
            }
            self[index] = value;
        })
        .def(py::self == py::self, R"doc(
            Equality operator for two int_vectors.

            Two int_vectors are equal if
            - capacities and sizes are equal and
            - width are equal and
            - the bits in the range [0..bit_size()-1] are equal.
        )doc")
        .def(py::self != py::self, R"doc(
            Inequality operator for two int_vectors.

            Two int_vectors are not equal if
            - capacities and sizes are not equal or
            - int widths are not equal or
            - the bits in the range [0..bit_size()-1] are not equal.
        )doc")
       .def(py::self < py::self, R"doc(
            Less operator for two int_vectors.

            int_vector w is less than v if
            - w[i]==v[i] for i<j and w[j]<v[j] with j in [0, min(w.size(), v.size()) )
            - or w[i]==v[i] for all i < min(w.size(), v.size()) and w.size()<v.size().
        )doc")
        .def(py::self > py::self, R"doc(
            Greater operator for two int_vectors.

            int_vector w is greater than v if
            - w[i]==v[i] for i<j and w[j]>v[j] with j in [0, min(w.size(), v.size()) )
            - or w[i]==v[i] for all i < min(w.size(), v.size()) and w.size()>v.size().
        )doc")
        .def(py::self <= py::self, "Less or equal operator.")
        .def(py::self >= py::self, "Greater or equal operator.")
        .def(py::self &= py::self, "Bitwise AND assignment operator.")
        .def(py::self |= py::self, "Bitwise OR assignment operator.")
        .def(py::self ^= py::self, "Bitwise XOR assignment operator.");
        
        //TODO iterators
        /*
        .def("__iter__", [](T &self){
            return py::make_iterator(self.begin(), self.end());
        },
        py::keep_alive<0, 1>(), "Iterator for int_vector");
        */
        //TODO flip method for bit vectors
 
        //TODO read header

        m.def("size_in_bytes", &sdsl::size_in_bytes<T>);
}

PYBIND11_MODULE(sdsl4py, m){
    m.doc() = "Python bindings for sdsl";

    //Run time fixed width vector
    add_intvector<int_vector<0>>(m, "int_vector");
    //Compile time fixed width vector
    add_intvector<int_vector<8>>(m, "int_vector_8");
    add_intvector<int_vector<16>>(m, "int_vector_16");
    add_intvector<int_vector<32>>(m, "int_vector_32");
    add_intvector<int_vector<64>>(m, "int_vector_64");
    
    //int vector bit width 1
    add_intvector<int_vector<1>>(m, "bit_vector");

   

    m.attr("__version__") = "0.0.1";
}
