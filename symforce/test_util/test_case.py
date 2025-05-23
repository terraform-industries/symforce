# ----------------------------------------------------------------------------
# SymForce - Copyright 2022, Skydio, Inc.
# This source code is under the Apache 2.0 license found in the LICENSE file.
# ----------------------------------------------------------------------------

import os
import random
import sys
import unittest

import numpy as np

import symforce
from symforce import typing as T
from symforce.test_util.test_case_mixin import SymforceTestCaseMixin


class TestCase(SymforceTestCaseMixin):
    """
    Base class for symforce tests. Adds some useful helpers.
    """

    # Set by the --run_slow_tests flag to indicate that we should run all tests even
    # if we're on SymPy.
    _RUN_SLOW_TESTS = False

    @staticmethod
    def should_run_slow_tests() -> bool:
        # NOTE(aaron):  This needs to be accessible before main() is called, so we do it here
        # instead.  This should also be called from main to make sure it runs at least once
        if "--run_slow_tests" in sys.argv:
            TestCase._RUN_SLOW_TESTS = True
            sys.argv.remove("--run_slow_tests")
        return TestCase._RUN_SLOW_TESTS

    @staticmethod
    def main(*args: T.Any, **kwargs: T.Any) -> None:
        """
        Call this to run all tests in scope.
        """
        TestCase.should_run_slow_tests()

        # unittest on py3.12 exits with _NO_TESTS_EXITCODE = 5 if no tests are found.  As of
        # py3.12.1, this includes if all tests are skipped (which applies to some of our tests,
        # depending on symbolic API and installed packages).  So, we monkey patch unittest to exit
        # with 0 in this case (including if no tests were found, which isn't great).  This hack
        # can be removed once the fix is in a release:
        # https://github.com/python/cpython/commit/159e3db1f7697b9aecdf674bb833fbb87f3dcad3
        if sys.version_info >= (3, 12, 0) and sys.version_info < (3, 12, 2):
            sys.modules[unittest.main.__module__]._NO_TESTS_EXITCODE = 0  # type: ignore[attr-defined]  # noqa: SLF001

        SymforceTestCaseMixin.main(*args, **kwargs)

    def setUp(self) -> None:
        super().setUp()

        # Set random seeds
        np.random.seed(42)
        random.seed(42)

        # Store verbosity flag so tests can use
        self.verbose = ("-v" in sys.argv) or ("--verbose" in sys.argv)


def requires_source_build(func: T.Callable) -> T.Callable:
    """
    Decorator to mark a test as skipped for wheel installs (as opposed to from-source builds)
    """
    if "SYMFORCE_WHEEL_TESTS" in os.environ:
        return unittest.skip("Skipping for wheel install")(func)
    else:
        return func


def sympy_only(func: T.Callable) -> T.Callable:
    """
    Decorator to mark a test to only run on SymPy, and skip otherwise.
    """
    if symforce.get_symbolic_api() != "sympy":
        return unittest.skip("This test only runs on SymPy symbolic API.")(func)
    else:
        return func


def symengine_only(func: T.Callable) -> T.Callable:
    """
    Decorator to mark a test to only run on the SymEngine, and skip otherwise.
    """
    if symforce.get_symbolic_api() != "symengine":
        return unittest.skip("This test only runs on the SymEngine symbolic API")(func)
    else:
        return func


def expected_failure_on_sympy(func: T.Callable) -> T.Callable:
    """
    Decorator to mark a test to be expected to fail only on SymPy
    """
    if symforce.get_symbolic_api() == "sympy":
        return unittest.expectedFailure(func)
    else:
        return func


def slow_on_sympy(func: T.Callable) -> T.Callable:
    """
    Decorator to mark a test as slow on sympy.  Will be skipped unless passed the
    ``--run_slow_tests`` flag
    """
    if symforce.get_symbolic_api() == "sympy" and not TestCase.should_run_slow_tests():
        return unittest.skip("This test is too slow on SymPy.")(func)
    else:
        return func
