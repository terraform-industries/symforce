// -----------------------------------------------------------------------------
// This file was autogenerated by symforce from template:
//     ops/CLASS/lie_group_ops.cc.jinja
// Do NOT modify by hand.
// -----------------------------------------------------------------------------

#include <cmath>

#include <sym/ops/pose3/lie_group_ops.h>
#include <sym/pose3.h>

namespace sym {

template <typename Scalar>
sym::Pose3<Scalar> LieGroupOps<Pose3<Scalar>>::FromTangent(const TangentVec& vec,
                                                           const Scalar epsilon) {
  // Total ops: 15

  // Input arrays

  // Intermediate terms (3)
  const Scalar _tmp0 =
      std::sqrt(Scalar(std::pow(epsilon, Scalar(2)) + std::pow(vec(0, 0), Scalar(2)) +
                       std::pow(vec(1, 0), Scalar(2)) + std::pow(vec(2, 0), Scalar(2))));
  const Scalar _tmp1 = (Scalar(1) / Scalar(2)) * _tmp0;
  const Scalar _tmp2 = std::sin(_tmp1) / _tmp0;

  // Output terms (1)
  Eigen::Matrix<Scalar, 7, 1> _res;

  _res[0] = _tmp2 * vec(0, 0);
  _res[1] = _tmp2 * vec(1, 0);
  _res[2] = _tmp2 * vec(2, 0);
  _res[3] = std::cos(_tmp1);
  _res[4] = vec(3, 0);
  _res[5] = vec(4, 0);
  _res[6] = vec(5, 0);

  return sym::Pose3<Scalar>(_res);
}

template <typename Scalar>
typename LieGroupOps<Pose3<Scalar>>::TangentVec LieGroupOps<Pose3<Scalar>>::ToTangent(
    const sym::Pose3<Scalar>& a, const Scalar epsilon) {
  // Total ops: 14

  // Input arrays
  const Eigen::Matrix<Scalar, 7, 1>& _a = a.Data();

  // Intermediate terms (2)
  const Scalar _tmp0 = std::min<Scalar>(std::fabs(_a[3]), 1 - epsilon);
  const Scalar _tmp1 = 2 * std::copysign(Scalar(1.0), _a[3]) * std::acos(_tmp0) /
                       std::sqrt(Scalar(1 - std::pow(_tmp0, Scalar(2))));

  // Output terms (1)
  Eigen::Matrix<Scalar, 6, 1> _res;

  _res(0, 0) = _a[0] * _tmp1;
  _res(1, 0) = _a[1] * _tmp1;
  _res(2, 0) = _a[2] * _tmp1;
  _res(3, 0) = _a[4];
  _res(4, 0) = _a[5];
  _res(5, 0) = _a[6];

  return _res;
}

template <typename Scalar>
sym::Pose3<Scalar> LieGroupOps<Pose3<Scalar>>::Retract(const sym::Pose3<Scalar>& a,
                                                       const TangentVec& vec,
                                                       const Scalar epsilon) {
  // Total ops: 48

  // Input arrays
  const Eigen::Matrix<Scalar, 7, 1>& _a = a.Data();

  // Intermediate terms (9)
  const Scalar _tmp0 =
      std::sqrt(Scalar(std::pow(epsilon, Scalar(2)) + std::pow(vec(0, 0), Scalar(2)) +
                       std::pow(vec(1, 0), Scalar(2)) + std::pow(vec(2, 0), Scalar(2))));
  const Scalar _tmp1 = (Scalar(1) / Scalar(2)) * _tmp0;
  const Scalar _tmp2 = std::cos(_tmp1);
  const Scalar _tmp3 = std::sin(_tmp1) / _tmp0;
  const Scalar _tmp4 = _a[3] * _tmp3;
  const Scalar _tmp5 = _a[2] * _tmp3;
  const Scalar _tmp6 = _tmp3 * vec(2, 0);
  const Scalar _tmp7 = _a[0] * _tmp3;
  const Scalar _tmp8 = _a[1] * _tmp3;

  // Output terms (1)
  Eigen::Matrix<Scalar, 7, 1> _res;

  _res[0] = _a[0] * _tmp2 + _a[1] * _tmp6 + _tmp4 * vec(0, 0) - _tmp5 * vec(1, 0);
  _res[1] = _a[1] * _tmp2 + _tmp4 * vec(1, 0) + _tmp5 * vec(0, 0) - _tmp7 * vec(2, 0);
  _res[2] = _a[2] * _tmp2 + _a[3] * _tmp6 + _tmp7 * vec(1, 0) - _tmp8 * vec(0, 0);
  _res[3] = -_a[2] * _tmp6 + _a[3] * _tmp2 - _tmp7 * vec(0, 0) - _tmp8 * vec(1, 0);
  _res[4] = _a[4] + vec(3, 0);
  _res[5] = _a[5] + vec(4, 0);
  _res[6] = _a[6] + vec(5, 0);

  return sym::Pose3<Scalar>(_res);
}

template <typename Scalar>
typename LieGroupOps<Pose3<Scalar>>::TangentVec LieGroupOps<Pose3<Scalar>>::LocalCoordinates(
    const sym::Pose3<Scalar>& a, const sym::Pose3<Scalar>& b, const Scalar epsilon) {
  // Total ops: 47

  // Input arrays
  const Eigen::Matrix<Scalar, 7, 1>& _a = a.Data();
  const Eigen::Matrix<Scalar, 7, 1>& _b = b.Data();

  // Intermediate terms (4)
  const Scalar _tmp0 = -_a[0] * _b[0] - _a[1] * _b[1] - _a[2] * _b[2];
  const Scalar _tmp1 = _a[3] * _b[3];
  const Scalar _tmp2 = std::min<Scalar>(1 - epsilon, std::fabs(_tmp0 - _tmp1));
  const Scalar _tmp3 = 2 * std::copysign(Scalar(1.0), -_tmp0 + _tmp1) * std::acos(_tmp2) /
                       std::sqrt(Scalar(1 - std::pow(_tmp2, Scalar(2))));

  // Output terms (1)
  Eigen::Matrix<Scalar, 6, 1> _res;

  _res(0, 0) = _tmp3 * (-_a[0] * _b[3] - _a[1] * _b[2] + _a[2] * _b[1] + _a[3] * _b[0]);
  _res(1, 0) = _tmp3 * (_a[0] * _b[2] - _a[1] * _b[3] - _a[2] * _b[0] + _a[3] * _b[1]);
  _res(2, 0) = _tmp3 * (-_a[0] * _b[1] + _a[1] * _b[0] - _a[2] * _b[3] + _a[3] * _b[2]);
  _res(3, 0) = -_a[4] + _b[4];
  _res(4, 0) = -_a[5] + _b[5];
  _res(5, 0) = -_a[6] + _b[6];

  return _res;
}

template <typename Scalar>
sym::Pose3<Scalar> LieGroupOps<Pose3<Scalar>>::Interpolate(const sym::Pose3<Scalar>& a,
                                                           const sym::Pose3<Scalar>& b,
                                                           const Scalar alpha,
                                                           const Scalar epsilon) {
  // Total ops: 106

  // Input arrays
  const Eigen::Matrix<Scalar, 7, 1>& _a = a.Data();
  const Eigen::Matrix<Scalar, 7, 1>& _b = b.Data();

  // Intermediate terms (18)
  const Scalar _tmp0 = _a[0] * _b[2] - _a[1] * _b[3] - _a[2] * _b[0] + _a[3] * _b[1];
  const Scalar _tmp1 = -_a[0] * _b[0] - _a[1] * _b[1] - _a[2] * _b[2];
  const Scalar _tmp2 = _a[3] * _b[3];
  const Scalar _tmp3 = std::copysign(Scalar(1.0), -_tmp1 + _tmp2);
  const Scalar _tmp4 = std::min<Scalar>(1 - epsilon, std::fabs(_tmp1 - _tmp2));
  const Scalar _tmp5 = std::acos(_tmp4);
  const Scalar _tmp6 = -_a[0] * _b[3] - _a[1] * _b[2] + _a[2] * _b[1] + _a[3] * _b[0];
  const Scalar _tmp7 = 1 - std::pow(_tmp4, Scalar(2));
  const Scalar _tmp8 = 4 * std::pow(_tmp3, Scalar(2)) * std::pow(_tmp5, Scalar(2)) *
                       std::pow(alpha, Scalar(2)) / _tmp7;
  const Scalar _tmp9 = -_a[0] * _b[1] + _a[1] * _b[0] - _a[2] * _b[3] + _a[3] * _b[2];
  const Scalar _tmp10 =
      std::sqrt(Scalar(std::pow(_tmp0, Scalar(2)) * _tmp8 + std::pow(_tmp6, Scalar(2)) * _tmp8 +
                       _tmp8 * std::pow(_tmp9, Scalar(2)) + std::pow(epsilon, Scalar(2))));
  const Scalar _tmp11 = (Scalar(1) / Scalar(2)) * _tmp10;
  const Scalar _tmp12 = 2 * _tmp3 * _tmp5 * alpha * std::sin(_tmp11) / (_tmp10 * std::sqrt(_tmp7));
  const Scalar _tmp13 = _a[2] * _tmp12;
  const Scalar _tmp14 = std::cos(_tmp11);
  const Scalar _tmp15 = _a[1] * _tmp12;
  const Scalar _tmp16 = _a[3] * _tmp12;
  const Scalar _tmp17 = _a[0] * _tmp12;

  // Output terms (1)
  Eigen::Matrix<Scalar, 7, 1> _res;

  _res[0] = _a[0] * _tmp14 - _tmp0 * _tmp13 + _tmp15 * _tmp9 + _tmp16 * _tmp6;
  _res[1] = _a[1] * _tmp14 + _tmp0 * _tmp16 + _tmp13 * _tmp6 - _tmp17 * _tmp9;
  _res[2] = _a[2] * _tmp14 + _tmp0 * _tmp17 - _tmp15 * _tmp6 + _tmp16 * _tmp9;
  _res[3] = _a[3] * _tmp14 - _tmp0 * _tmp15 - _tmp13 * _tmp9 - _tmp17 * _tmp6;
  _res[4] = _a[4] + alpha * (-_a[4] + _b[4]);
  _res[5] = _a[5] + alpha * (-_a[5] + _b[5]);
  _res[6] = _a[6] + alpha * (-_a[6] + _b[6]);

  return sym::Pose3<Scalar>(_res);
}

}  // namespace sym

// Explicit instantiation
template struct sym::LieGroupOps<sym::Pose3<double>>;
template struct sym::LieGroupOps<sym::Pose3<float>>;
