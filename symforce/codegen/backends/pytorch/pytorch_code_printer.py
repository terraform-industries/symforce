# ----------------------------------------------------------------------------
# SymForce - Copyright 2022, Skydio, Inc.
# This source code is under the Apache 2.0 license found in the LICENSE file.
# ----------------------------------------------------------------------------
from __future__ import annotations

import sympy
from sympy.printing.codeprinter import CodePrinter
from sympy.printing.pycode import PythonCodePrinter

import symforce.internal.symbolic as sf
from symforce import typing as T

_known_functions_torch = {
    "Abs": "abs",
    "acos": "acos",
    "acosh": "acosh",
    "asin": "asin",
    "asinh": "asinh",
    "atan": "atan",
    "atan2": "atan2",
    "atanh": "atanh",
    "ceiling": "ceil",
    "cos": "cos",
    "cosh": "cosh",
    "erf": "erf",
    "erfc": "erfc",
    "exp": "exp",
    "expm1": "expm1",
    "floor": "floor",
    "hypot": "hypot",
    "loggamma": "lgamma",
    "log": "log",
    "ln": "log",
    "log10": "log10",
    "log1p": "log1p",
    "log2": "log2",
    "sin": "sin",
    "sinh": "sinh",
    "Sqrt": "sqrt",
    "tan": "tan",
    "tanh": "tanh",
    "CopysignNoZero": "copysign",
}

_known_constants_math = {
    "Exp1": "e",
    "Pi": "pi",
    "E": "e",
    "Infinity": "inf",
    "NaN": "nan",
    "ComplexInfinity": "nan",
}


def _print_known_const(self: PyTorchCodePrinter, expr: sympy.Expr) -> str:
    return f"torch.tensor(math.{_known_constants_math[expr.__class__.__name__]}, **tensor_kwargs)"


def _print_known_func(self: PyTorchCodePrinter, expr: sympy.Expr) -> str:
    name = _known_functions_torch[expr.__class__.__name__]
    return f"torch.{name}({', '.join(map(self._print, expr.args))})"


class PyTorchCodePrinter(CodePrinter):
    """
    Symforce customized code printer for PyTorch. Modifies the Sympy printing
    behavior for codegen compatibility and efficiency.

    This is more different from PythonCodePrinter than it is similar, so we go mostly from scratch
    and call some methods from that printer where desired.
    """

    # NOTE(aaron): We need to override this from the value set by StrPrinter, so that we don't use
    # expressions' implementations of `_sympystr`, even though we don't make use of `_pytorch`.
    printmethod = "_pytorch"

    known_functions = _known_functions_torch
    language = "Python"
    _default_settings = dict(CodePrinter._default_settings, human=False)  # noqa: SLF001

    def __init__(self, settings: T.Optional[T.Mapping[str, T.Any]] = None):
        if settings and settings.get("human", False):
            raise ValueError("Setting `human=True` not supported for PyTorchCodePrinter")
        super().__init__(settings)

    def doprint(self, expr: sympy.Expr, assign_to: T.Optional[T.Any] = None) -> str:
        _, not_supported, result = super().doprint(expr, assign_to)
        if not_supported:
            raise NotImplementedError(
                f"Tried to print the following unsupported expressions: {not_supported}"
            )
        return result

    @staticmethod
    def _format_code(lines: T.List[str]) -> T.List[str]:
        return lines

    def _print_Mod(self, expr: sympy.Mod) -> str:
        return f"torch.remainder({self._print(expr.args[0])}, {self._print(expr.args[1])})"

    def _print_sign(self, expr: sympy.sign) -> str:
        return f"torch.sign({self._print(expr.args[0])})"

    def _print_Pow(self, expr: sympy.Pow, rational: bool = False) -> str:
        # TODO(aaron): Optimize this?
        return f"torch.pow({self._print(expr.base)}, {self._print(expr.exp)})"

    @staticmethod
    def _print_Rational(expr: sympy.Rational) -> str:
        # This is py3-only, need decimal points if we want py2
        return f"torch.tensor({expr.p}/{expr.q}, **tensor_kwargs)"

    def _print_Float(self, flt: sympy.Float) -> str:
        return f"torch.tensor({super()._print_Float(flt)}, **tensor_kwargs)"

    def _print_frac(self, expr: sympy.frac) -> str:
        return self._print_Mod(sympy.Mod(expr.args[0], 1))

    @staticmethod
    def _print_Integer(expr: sympy.Integer) -> str:
        """
        Customizations:
            * Cast all integers to Tensor
        """
        return f"torch.tensor({expr.p}, **tensor_kwargs)"

    def _print_NumberSymbol(self, expr: sympy.Expr) -> str:
        """
        Customizations:
            * Cast all NumberSymbols to Tensor
        """
        return f"torch.tensor({super()._print_NumberSymbol(expr)}, **tensor_kwargs)"

    @staticmethod
    def _print_Zero(expr: sympy.Expr) -> str:
        """
        Customizations:
            * Cast Zero to Tensor
        """
        return "torch.tensor(0, **tensor_kwargs)"

    def _print_Symbol(self, expr: sympy.Symbol) -> str:
        name = super()._print_Symbol(expr)

        if name in PythonCodePrinter.reserved_words:
            raise ValueError(
                f'This expression includes the symbol "{name}" which is a reserved keyword in Python.'
            )

        return name

    def _print_Max(self, expr: sympy.Max) -> str:
        if len(expr.args) == 1:
            return self._print(expr.args[0])
        else:
            from sympy.functions.elementary.miscellaneous import Max

            return f"torch.maximum({self._print(expr.args[0])}, {self._print(Max(*expr.args[1:]))})"

    def _print_Min(self, expr: sympy.Min) -> str:
        if len(expr.args) == 1:
            return self._print(expr.args[0])
        else:
            from sympy.functions.elementary.miscellaneous import Min

            return f"torch.minimum({self._print(expr.args[0])}, {self._print(Min(*expr.args[1:]))})"

    # NOTE(brad): We type ignore the signature because mypy complains that it
    # does not match that of the sympy base class CodePrinter. This is because the base class
    # defines _print_Heaviside with: _print_Heaviside = None (see
    # https://github.com/sympy/sympy/blob/95f0228c033d27731f8707cdbb5bb672e500847d/sympy/printing/codeprinter.py#L446
    # ).
    # Despite this, our signature here matches the signatures of the sympy defined subclasses
    # of CodePrinter. I don't know of any other way to resolve this issue other than to
    # to type ignore.
    def _print_Heaviside(self, expr: "sympy.Heaviside") -> str:  # type: ignore[override]
        return f"torch.heaviside({self._print(expr.args[0])}, values=torch.tensor(1.0, **tensor_kwargs))"

    def _print_gamma(self, expr: sympy.functions.special.gamma_functions.gamma) -> str:
        # PyTorch does not have the gamma function, this is the best we can do
        return f"torch.lgamma({self._print(expr.args[0])}).exp()"

    def _print_SignNoZero(self, expr: sf.SymPySignNoZero) -> str:
        arg = self._print(expr.args[0])
        return f"torch.copysign(torch.tensor(1.0, **tensor_kwargs), {arg})"


for k in _known_functions_torch:
    setattr(PyTorchCodePrinter, f"_print_{k}", _print_known_func)

for k in _known_constants_math:
    setattr(PyTorchCodePrinter, f"_print_{k}", _print_known_const)
