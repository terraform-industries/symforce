# ----------------------------------------------------------------------------
# SymForce - Copyright 2022, Skydio, Inc.
# This source code is under the Apache 2.0 license found in the LICENSE file.
# ----------------------------------------------------------------------------

import symforce.symbolic as sf
from symforce import typing as T
from symforce.ops import LieGroupOps
from symforce.ops import StorageOps


def tangent_jacobians(
    expr: T.Element, args: T.Sequence[T.Element], epsilon: sf.Scalar = sf.epsilon()
) -> T.List[sf.Matrix]:
    """
    Compute jacobians of expr, a Lie Group element which is a function of the Lie Group elements in
    args.  Jacobians are derivatives in the tangent space of expr with respect to changes in the
    tangent space of the arg, as opposed to jacobians of the storage of either which could be
    trivially computed with :meth:`sf.Matrix.jacobian <symforce.geo.matrix.Matrix.jacobian>` or
    ``sf.Expr.diff``.

    This uses :func:`tangent_jacobians_first_order` internally.

    Args:
        expr: The final expression that should be differentiated
        args: Sequence of variables (can be Lie Group elements) to differentiate with respect to

    Returns:
        The jacobian ``expr_D_arg`` for each arg in ``args``, where each ``expr_D_arg`` is of shape
            ``MxN``, with ``M`` the tangent space dimension of ``expr`` and ``N`` the tangent space
            dimension of ``arg``
    """
    return tangent_jacobians_first_order(expr, args, epsilon)


def tangent_jacobians_first_order(
    expr: T.Element, args: T.Sequence[T.Element], epsilon: sf.Scalar = sf.epsilon()
) -> T.List[sf.Matrix]:
    """
    An implementation of :func:`tangent_jacobians` using first-order simplifications of retract
    and local_coordinates.

    The interface is the same as for :func:`tangent_jacobians`.

    This is the method described in Section V.B.2 of
    `the SymForce paper <https://arxiv.org/abs/2204.07889>`_.

    If ``expr = f(arg)``, then the jacobian we want to return is the derivative of
    ``local_coordinates(f(arg), f(retract(arg), t)`` with respect to tangent vector t at ``t = 0``.

    ``local_coordinates`` and ``retract``, however, are often complicated functions which are hard
    to symbolically differentiate when composed with f. To avoid this issue, we replace them with
    first order approximations. The result is something which we can easily symbolically
    differentiate.

    This works because the approximations become exact in the limit as ``t -> 0``.

    While the output returned is different than that returned by
    :func:`tangent_jacobians_chain_rule`, they are the same when evaluated numerically.
    ``tangent_jacobians_first_order`` (almost?) always returns expressions which require fewer ops
    after cse.
    """
    jacobians = []

    def infinitesimal_retract(a: T.Element, v: sf.Matrix) -> T.Element:
        """
        Returns a first order approximation to LieGroupOps.retract(v)
        """
        return StorageOps.from_storage(
            a,
            (
                sf.M(StorageOps.to_storage(a)) + LieGroupOps.storage_D_tangent(a, epsilon) * v
            ).to_storage(),
        )

    def infinitesimal_local_coordinates(a: T.Element, b: T.Element) -> sf.Matrix:
        """
        Returns a first order (in b - a) approximation to LieGroupOps.local_coordinates(a, b)
        """
        return LieGroupOps.tangent_D_storage(a, epsilon) * (
            sf.M(StorageOps.to_storage(b)) - sf.M(StorageOps.to_storage(a))
        )

    def safe_subs(expr: T.Element, old: T.Element, new: T.Element) -> T.Element:
        """
        Substitutes occurrences of old in expr with new. This is safe to use even when the
        components of new contain symbols in old that we are replacing.
        """
        intermediate = StorageOps.from_storage(
            old,
            sf.M(StorageOps.storage_dim(old), 1)
            .symbolic("__symforce_internal_intermediate")
            .to_flat_list(),
        )
        return StorageOps.subs(StorageOps.subs(expr, old, intermediate), intermediate, new)

    for arg in args:
        xi = sf.M(LieGroupOps.tangent_dim(arg), 1).symbolic("__symforce_internal_xi")
        arg_perturbed = infinitesimal_retract(arg, xi)

        expr_perturbed = safe_subs(expr, arg, arg_perturbed)

        result = infinitesimal_local_coordinates(expr, expr_perturbed)
        # Use tangent_space=False here to avoid recursion
        arg_jacobian = result.jacobian(xi, tangent_space=False, epsilon=epsilon).subs(xi, xi.zero())

        jacobians.append(arg_jacobian)

    return jacobians


def tangent_jacobians_chain_rule(
    expr: T.Element, args: T.Sequence[T.Element], epsilon: sf.Scalar = sf.epsilon()
) -> T.List[sf.Matrix]:
    """
    An implementation of :func:`tangent_jacobians` using the symbolic chain rule with
    ``tangent_D_storage`` and ``storage_D_tangent``.

    The interface is the same as for :func:`tangent_jacobians`.

    This is the method described in Section V.B.1 of
    `the SymForce paper <https://arxiv.org/abs/2204.07889>`_.

    If ``expr = f(arg)``, then the jacobian we want to return is the derivative of
    ``local_coordinates(f(arg), f(retract(arg), t)`` with respect to tangent vector t at ``t = 0``.

    ``local_coordinates`` and ``retract``, however, are often complicated functions which are hard
    to symbolically differentiate. To avoid this issue, we compute their derivatives ahead of time,
    then use the chain rule.

    While the output returned is different than that returned by
    :func:`tangent_jacobians_first_order`, they are the same when evaluated numerically.
    :func:`tangent_jacobians_first_order` (almost?) always returns expressions which require fewer
    ops after cse.
    """
    jacobians = []

    # Compute jacobians in the space of the storage, then chain rule on the left and right sides
    # to get jacobian wrt the tangent space of both the arg and the result
    expr_storage = sf.M(StorageOps.to_storage(expr))
    expr_tangent_D_storage = LieGroupOps.tangent_D_storage(expr, epsilon)

    for arg in args:
        # Use tangent_space=False here to avoid recursion
        expr_storage_D_arg_storage = expr_storage.jacobian(
            StorageOps.to_storage(arg), tangent_space=False, epsilon=epsilon
        )
        arg_jacobian = expr_tangent_D_storage * (
            expr_storage_D_arg_storage * LieGroupOps.storage_D_tangent(arg, epsilon)
        )

        jacobians.append(arg_jacobian)

    return jacobians
