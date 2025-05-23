# ----------------------------------------------------------------------------
# SymForce - Copyright 2022, Skydio, Inc.
# This source code is under the Apache 2.0 license found in the LICENSE file.
# ----------------------------------------------------------------------------

"""
The core SymForce symbolic API

This combines functions available from various sources:

- Many functions we expose from the SymPy (and SymEngine) API
- Additional functions defined here to override those provided by SymPy or SymEngine, or provide a
  uniform interface between the two.  See https://symforce.org/api/symforce.symbolic.html for
  information on these
- Logic functions defined in :mod:`symforce.logic`, see the documentation for that module
- Additional geometry and camera types provided by SymForce from the :mod:`symforce.geo` and
  :mod:`symforce.cam` modules

It typically isn't necessary to actually access the symbolic API being used internally, but that is
available as well as ``symforce.symbolic.sympy``.
"""

# ruff: noqa: F401, F403

from symforce.internal.symbolic import *

# isort: split
from symforce.geo import *

# isort: split
from symforce.cam import *

# isort: split
from symforce import util

# This doesn't change the runtime behavior, but tells Sphinx to document everything in here
__all__ = list(globals().keys())
