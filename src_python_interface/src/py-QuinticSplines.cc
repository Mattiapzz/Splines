/**
 * PYTHON Wrapper for Splines
 *
 * License MIT - See LICENSE file
 * 2019 Matteo Ragni, Claudio Kerov Ghiglianovich, Enrico Bertolazzi
 */

#include <string>

#include "py-QuinticSplines.hh"

namespace pySpline {
  namespace py = pybind11;

  using Splines::real_type;
  using Splines::integer;

  using Splines::Spline;
  using Splines::QuinticSplineBase;
  using Splines::QuinticSpline;

  void python_register_quintic_splines_base_class(py::module & m) {
    py::class_<QuinticSplineBase, PythonicQuinticSplineBase, Spline>(m, "QuinticSplineBase")
      .def(py::init<std::string const &>(), py::arg("name") = "QuinticSplineBase")
      .def("copy_spline", &QuinticSplineBase::copy_spline)
      .def("yp_node", &QuinticSplineBase::yp_node)
      .def("ypp_node", &QuinticSplineBase::ypp_node);
  }

  void python_register_quintic_splines_class(module & m) {
    py::class_<QuinticSpline, QuinticSplineBase>(m, "QuinticSpline")
      .def(py::init<std::string const &>(), py::arg("name") = "QuinticSpline")
      .def("setQuinticType", &QuinticSpline::setQuinticType);
  }

}