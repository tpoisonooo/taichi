#include <pybind11/pybind11.h>
#include <taichi/python/export.h>
#include <taichi/common/interface.h>
#include "tlang.h"

TLANG_NAMESPACE_BEGIN

PYBIND11_MODULE(taichi_lang, m) {
  py::class_<Expr>(m, "Expr").def("serialize", &Expr::serialize);
  py::class_<ExprGroup>(m, "ExprGroup")
      .def(py::init<>())
      .def("push_back", &ExprGroup::push_back)
      .def("serialize", &ExprGroup::serialize);
  py::class_<Stmt>(m, "Stmt");

  m.def("expr_add", expr_add);
  m.def("expr_sub", expr_sub);
  m.def("expr_mul", expr_mul);
  m.def("expr_div", expr_div);

  m.def("expr_cmp_le", expr_cmp_le);
  m.def("expr_cmp_lt", expr_cmp_lt);
  m.def("expr_cmp_ge", expr_cmp_ge);
  m.def("expr_cmp_gt", expr_cmp_gt);
  m.def("expr_cmp_ne", expr_cmp_ne);
  m.def("expr_cmp_eq", expr_cmp_eq);

  m.def("make_global_load_stmt", Stmt::make<GlobalLoadStmt, Stmt *>);
  m.def("make_global_store_stmt", Stmt::make<GlobalStoreStmt, Stmt *, Stmt *>);
  m.def("make_frontend_assign_stmt",
        Stmt::make<FrontendAssignStmt, const Expr &, const Expr &>);

  m.def("make_id_expr", Expr::make<IdExpression, std::string>);
  m.def("make_constant_expr", Expr::make<ConstExpression, int>);
  m.def("make_constant_expr", Expr::make<ConstExpression, float32>);
  m.def("make_constant_expr", Expr::make<ConstExpression, float64>);

  m.def("make_global_ptr_expr",
        Expr::make<GlobalPtrExpression, const Expr &, const ExprGroup &>);

  auto &&bin = py::enum_<BinaryOpType>(m, "BinaryOpType", py::arithmetic());
  for (int t = 0; t <= (int)BinaryOpType::undefined; t++)
    bin.value(binary_op_type_name(BinaryOpType(t)).c_str(), BinaryOpType(t));
  bin.export_values();
  m.def("make_binary_op_expr",
        Expr::make<BinaryOpExpression, const BinaryOpType &, const Expr &,
                   const Expr &>);

  auto &&unary = py::enum_<UnaryOpType>(m, "UnaryOpType", py::arithmetic());
  for (int t = 0; t <= (int)UnaryOpType::undefined; t++)
    unary.value(unary_op_type_name(UnaryOpType(t)).c_str(), UnaryOpType(t));
  unary.export_values();
  m.def("make_unary_op_expr",
        Expr::make<UnaryOpExpression, const UnaryOpType &, const Expr &>);
}

TLANG_NAMESPACE_END
