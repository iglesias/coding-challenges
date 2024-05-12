#include <nanobind/nanobind.h>

#include "include/nobcmp.h"

namespace nb = nanobind;

NB_MODULE(nob, m) {
    nb::class_<Cmd>(m, "Cmd")
        .def(nb::init<>())
        .def_rw("line", &Cmd::line);
}

/*
NB_MODULE(nob, m) {
    nb::class_<Nob_Cmd>(m, "Nob_Cmd")
        .def(nb::init<>())
        .def(nb::init<const char **, size_t, size_t>())
        .def_rw("items", &Nob_Cmd::items)
        .def_rw("count", &Nob_Cmd::count)
        .def_rw("capacity", &Nob_Cmd::capacity);
}
*/
