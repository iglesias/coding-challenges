#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>

#include "nob.cpp"

namespace nb = nanobind;

NB_MODULE(nob, m) {
    nb::class_<Cmd>(m, "Cmd")
        .def(nb::init<>())
        .def_rw("line", &Cmd::line);

    [&m]<std::size_t... Is>(std::index_sequence<Is...>) {
        (m.def("make_and_run_cmd", &make_and_run_cmd<Is + 1>), ...);
    }(std::make_index_sequence<19>{});
}
