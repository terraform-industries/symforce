// -----------------------------------------------------------------------------
// This file was autogenerated by symforce from template:
//     function/FUNCTION.h.jinja
// Do NOT modify by hand.
// -----------------------------------------------------------------------------

#pragma once

#include <Eigen/Core>

#include <sym/pose3.h>

namespace sym {

/**
 * Composition of two elements in the group.
 *
 * Returns:
 *     Element: a @ b
 *     res_D_b: (6x6) jacobian of res (6) wrt arg b (6)
 */
template <typename Scalar>
sym::Pose3<Scalar> ComposePose3WithJacobian1(const sym::Pose3<Scalar>& a,
                                             const sym::Pose3<Scalar>& b,
                                             Eigen::Matrix<Scalar, 6, 6>* const res_D_b = nullptr) {
  // Total ops: 173

  // Input arrays
  const Eigen::Matrix<Scalar, 7, 1>& _a = a.Data();
  const Eigen::Matrix<Scalar, 7, 1>& _b = b.Data();

  // Intermediate terms (74)
  const Scalar _tmp0 = _a[2] * _b[1];
  const Scalar _tmp1 = _a[0] * _b[3];
  const Scalar _tmp2 = _a[1] * _b[2];
  const Scalar _tmp3 = _a[3] * _b[0];
  const Scalar _tmp4 = -_tmp0 + _tmp1 + _tmp2 + _tmp3;
  const Scalar _tmp5 = _a[3] * _b[1];
  const Scalar _tmp6 = _a[1] * _b[3];
  const Scalar _tmp7 = _a[0] * _b[2];
  const Scalar _tmp8 = _a[2] * _b[0];
  const Scalar _tmp9 = _tmp5 + _tmp6 - _tmp7 + _tmp8;
  const Scalar _tmp10 = _a[0] * _b[1];
  const Scalar _tmp11 = _a[2] * _b[3];
  const Scalar _tmp12 = _a[3] * _b[2];
  const Scalar _tmp13 = _a[1] * _b[0];
  const Scalar _tmp14 = _tmp10 + _tmp11 + _tmp12 - _tmp13;
  const Scalar _tmp15 = _a[1] * _b[1];
  const Scalar _tmp16 = _a[2] * _b[2];
  const Scalar _tmp17 = _a[0] * _b[0];
  const Scalar _tmp18 = _a[3] * _b[3];
  const Scalar _tmp19 = -_tmp15 - _tmp16 - _tmp17 + _tmp18;
  const Scalar _tmp20 = 2 * _a[0];
  const Scalar _tmp21 = _a[2] * _tmp20;
  const Scalar _tmp22 = 2 * _a[3];
  const Scalar _tmp23 = _a[1] * _tmp22;
  const Scalar _tmp24 = _tmp21 + _tmp23;
  const Scalar _tmp25 = _a[1] * _tmp20;
  const Scalar _tmp26 = _a[2] * _tmp22;
  const Scalar _tmp27 = _tmp25 - _tmp26;
  const Scalar _tmp28 = -2 * std::pow(_a[2], Scalar(2));
  const Scalar _tmp29 = -2 * std::pow(_a[1], Scalar(2));
  const Scalar _tmp30 = _tmp28 + _tmp29 + 1;
  const Scalar _tmp31 = _tmp25 + _tmp26;
  const Scalar _tmp32 = 2 * _a[1] * _a[2];
  const Scalar _tmp33 = _a[0] * _tmp22;
  const Scalar _tmp34 = _tmp32 - _tmp33;
  const Scalar _tmp35 = 1 - 2 * std::pow(_a[0], Scalar(2));
  const Scalar _tmp36 = _tmp28 + _tmp35;
  const Scalar _tmp37 = _tmp21 - _tmp23;
  const Scalar _tmp38 = _tmp32 + _tmp33;
  const Scalar _tmp39 = _tmp29 + _tmp35;
  const Scalar _tmp40 = (Scalar(1) / Scalar(2)) * _tmp10;
  const Scalar _tmp41 = (Scalar(1) / Scalar(2)) * _tmp11;
  const Scalar _tmp42 = (Scalar(1) / Scalar(2)) * _tmp12;
  const Scalar _tmp43 = (Scalar(1) / Scalar(2)) * _tmp13;
  const Scalar _tmp44 = _tmp40 + _tmp41 + _tmp42 - _tmp43;
  const Scalar _tmp45 = 2 * _tmp14;
  const Scalar _tmp46 = (Scalar(1) / Scalar(2)) * _tmp5;
  const Scalar _tmp47 = (Scalar(1) / Scalar(2)) * _tmp6;
  const Scalar _tmp48 = (Scalar(1) / Scalar(2)) * _tmp7;
  const Scalar _tmp49 = (Scalar(1) / Scalar(2)) * _tmp8;
  const Scalar _tmp50 = -_tmp46 - _tmp47 + _tmp48 - _tmp49;
  const Scalar _tmp51 = 2 * _tmp9;
  const Scalar _tmp52 = -_tmp50 * _tmp51;
  const Scalar _tmp53 = (Scalar(1) / Scalar(2)) * _tmp0;
  const Scalar _tmp54 = (Scalar(1) / Scalar(2)) * _tmp1;
  const Scalar _tmp55 = (Scalar(1) / Scalar(2)) * _tmp2;
  const Scalar _tmp56 = (Scalar(1) / Scalar(2)) * _tmp3;
  const Scalar _tmp57 = _tmp53 - _tmp54 - _tmp55 - _tmp56;
  const Scalar _tmp58 = 2 * _tmp4;
  const Scalar _tmp59 = -Scalar(1) / Scalar(2) * _tmp15 - Scalar(1) / Scalar(2) * _tmp16 -
                        Scalar(1) / Scalar(2) * _tmp17 + (Scalar(1) / Scalar(2)) * _tmp18;
  const Scalar _tmp60 = 2 * _tmp19;
  const Scalar _tmp61 = _tmp59 * _tmp60;
  const Scalar _tmp62 = -_tmp57 * _tmp58 + _tmp61;
  const Scalar _tmp63 = _tmp45 * _tmp59;
  const Scalar _tmp64 = _tmp50 * _tmp58;
  const Scalar _tmp65 = _tmp45 * _tmp57;
  const Scalar _tmp66 = _tmp51 * _tmp59;
  const Scalar _tmp67 = -_tmp53 + _tmp54 + _tmp55 + _tmp56;
  const Scalar _tmp68 = 2 * _tmp67;
  const Scalar _tmp69 = -_tmp40 - _tmp41 - _tmp42 + _tmp43;
  const Scalar _tmp70 = -_tmp45 * _tmp69;
  const Scalar _tmp71 = _tmp51 * _tmp69;
  const Scalar _tmp72 = _tmp58 * _tmp59;
  const Scalar _tmp73 = 2 * _tmp46 + 2 * _tmp47 - 2 * _tmp48 + 2 * _tmp49;

  // Output terms (2)
  Eigen::Matrix<Scalar, 7, 1> _res;

  _res[0] = _tmp4;
  _res[1] = _tmp9;
  _res[2] = _tmp14;
  _res[3] = _tmp19;
  _res[4] = _a[4] + _b[4] * _tmp30 + _b[5] * _tmp27 + _b[6] * _tmp24;
  _res[5] = _a[5] + _b[4] * _tmp31 + _b[5] * _tmp36 + _b[6] * _tmp34;
  _res[6] = _a[6] + _b[4] * _tmp37 + _b[5] * _tmp38 + _b[6] * _tmp39;

  if (res_D_b != nullptr) {
    Eigen::Matrix<Scalar, 6, 6>& _res_D_b = (*res_D_b);

    _res_D_b(0, 0) = _tmp44 * _tmp45 + _tmp52 + _tmp62;
    _res_D_b(1, 0) = _tmp44 * _tmp60 - _tmp51 * _tmp57 - _tmp63 + _tmp64;
    _res_D_b(2, 0) = -_tmp44 * _tmp58 + _tmp50 * _tmp60 - _tmp65 + _tmp66;
    _res_D_b(3, 0) = 0;
    _res_D_b(4, 0) = 0;
    _res_D_b(5, 0) = 0;
    _res_D_b(0, 1) = _tmp60 * _tmp69 + _tmp63 - _tmp64 - _tmp68 * _tmp9;
    _res_D_b(1, 1) = _tmp4 * _tmp68 + _tmp52 + _tmp61 + _tmp70;
    _res_D_b(2, 1) = -_tmp45 * _tmp50 + _tmp60 * _tmp67 + _tmp71 - _tmp72;
    _res_D_b(3, 1) = 0;
    _res_D_b(4, 1) = 0;
    _res_D_b(5, 1) = 0;
    _res_D_b(0, 2) = _tmp19 * _tmp73 - _tmp58 * _tmp69 + _tmp65 - _tmp66;
    _res_D_b(1, 2) = -_tmp14 * _tmp73 + _tmp57 * _tmp60 - _tmp71 + _tmp72;
    _res_D_b(2, 2) = _tmp62 + _tmp70 + _tmp73 * _tmp9;
    _res_D_b(3, 2) = 0;
    _res_D_b(4, 2) = 0;
    _res_D_b(5, 2) = 0;
    _res_D_b(0, 3) = 0;
    _res_D_b(1, 3) = 0;
    _res_D_b(2, 3) = 0;
    _res_D_b(3, 3) = _tmp30;
    _res_D_b(4, 3) = _tmp31;
    _res_D_b(5, 3) = _tmp37;
    _res_D_b(0, 4) = 0;
    _res_D_b(1, 4) = 0;
    _res_D_b(2, 4) = 0;
    _res_D_b(3, 4) = _tmp27;
    _res_D_b(4, 4) = _tmp36;
    _res_D_b(5, 4) = _tmp38;
    _res_D_b(0, 5) = 0;
    _res_D_b(1, 5) = 0;
    _res_D_b(2, 5) = 0;
    _res_D_b(3, 5) = _tmp24;
    _res_D_b(4, 5) = _tmp34;
    _res_D_b(5, 5) = _tmp39;
  }

  return sym::Pose3<Scalar>(_res);
}  // NOLINT(readability/fn_size)

// NOLINTNEXTLINE(readability/fn_size)
}  // namespace sym
