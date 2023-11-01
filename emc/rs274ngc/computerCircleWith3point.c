/*
 * File: computerCircleWith3point.c
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 28-May-2018 18:13:06
 */

/* Include Files */
//#include "rt_nonfinite.h"
#include "computerCircleWith3point.h"
#include "norm.h"

/* Function Definitions */

/*
 * Arguments    : const double P1[3]
 *                const double P2[3]
 *                const double P3[3]
 *                double *flag
 *                double center[3]
 *                double normal[3]
 * Return Type  : void
 */
void computerCircleWith3point(const double P1[3], const double P2[3], const
  double P3[3], double *flag, double center[3], double normal[3])
{
  double P12[3];
  double P23[3];
  int p1;
  double normP12;
  double normP23;
  double b_P12[3];
  static const signed char iv0[3] = { 0, 0, 1 };

  double absx11;
  double a[9];
  double x[9];
  double b_normal;
  int p2;
  int p3;
  double absx21;
  double absx31;
  int itmp;
  double c[9];
  double dv0[3];
  for (p1 = 0; p1 < 3; p1++) {
    P12[p1] = P2[p1] - P1[p1];
    P23[p1] = P3[p1] - P2[p1];
    center[p1] = 0.0;
    normal[p1] = iv0[p1];
  }

  normP12 = norm(P12);
  normP23 = norm(P23);
  if ((normP12 < 1.0E-6) || (normP23 < 1.0E-6)) {
    *flag = 0.0;
  } else {
    for (p1 = 0; p1 < 3; p1++) {
      b_P12[p1] = P12[p1] / normP12 - P23[p1] / normP23;
    }

    if (fabs(norm(b_P12)) < 1.0E-6) {
      *flag = 0.0;
    } else {
      *flag = 1.0;
      normal[0] = P12[1] * P23[2] - P12[2] * P23[1];
      normal[1] = P12[2] * P23[0] - P12[0] * P23[2];
      normal[2] = P12[0] * P23[1] - P12[1] * P23[0];
      absx11 = norm(normal);
      for (p1 = 0; p1 < 3; p1++) {
        b_normal = normal[p1] / absx11;
        a[3 * p1] = P12[p1];
        a[1 + 3 * p1] = P23[p1];
        a[2 + 3 * p1] = b_normal;
        normal[p1] = b_normal;
      }

      memcpy(&x[0], &a[0], 9U * sizeof(double));
      p1 = 0;
      p2 = 3;
      p3 = 6;
      absx11 = fabs(a[0]);
      absx21 = fabs(a[1]);
      absx31 = fabs(a[2]);
      if ((absx21 > absx11) && (absx21 > absx31)) {
        p1 = 3;
        p2 = 0;
        x[0] = a[1];
        x[1] = a[0];
        x[3] = a[4];
        x[4] = a[3];
        x[6] = a[7];
        x[7] = a[6];
      } else {
        if (absx31 > absx11) {
          p1 = 6;
          p3 = 0;
          x[0] = a[2];
          x[2] = a[0];
          x[3] = a[5];
          x[5] = a[3];
          x[6] = a[8];
          x[8] = a[6];
        }
      }

      absx11 = x[1] / x[0];
      x[1] /= x[0];
      absx21 = x[2] / x[0];
      x[2] /= x[0];
      x[4] -= absx11 * x[3];
      x[5] -= absx21 * x[3];
      x[7] -= absx11 * x[6];
      x[8] -= absx21 * x[6];
      if (fabs(x[5]) > fabs(x[4])) {
        itmp = p2;
        p2 = p3;
        p3 = itmp;
        x[1] = absx21;
        x[2] = absx11;
        absx11 = x[4];
        x[4] = x[5];
        x[5] = absx11;
        absx11 = x[7];
        x[7] = x[8];
        x[8] = absx11;
      }

      absx11 = x[5] / x[4];
      x[5] /= x[4];
      x[8] -= absx11 * x[7];
      absx11 = (x[5] * x[1] - x[2]) / x[8];
      absx21 = -(x[1] + x[7] * absx11) / x[4];
      c[p1] = ((1.0 - x[3] * absx21) - x[6] * absx11) / x[0];
      c[p1 + 1] = absx21;
      c[p1 + 2] = absx11;
      absx11 = -x[5] / x[8];
      absx21 = (1.0 - x[7] * absx11) / x[4];
      c[p2] = -(x[3] * absx21 + x[6] * absx11) / x[0];
      c[p2 + 1] = absx21;
      c[p2 + 2] = absx11;
      absx11 = 1.0 / x[8];
      absx21 = -x[7] * absx11 / x[4];
      c[p3] = -(x[3] * absx21 + x[6] * absx11) / x[0];
      c[p3 + 1] = absx21;
      c[p3 + 2] = absx11;
      absx11 = 0.0;
      absx21 = 0.0;
      absx31 = 0.0;
      for (p1 = 0; p1 < 3; p1++) {
        absx11 += P12[p1] * (P1[p1] + P2[p1]);
        absx21 += P23[p1] * (P2[p1] + P3[p1]);
        absx31 += normal[p1] * P2[p1];
      }

      dv0[0] = absx11 / 2.0;
      dv0[1] = absx21 / 2.0;
      dv0[2] = absx31;
      for (p1 = 0; p1 < 3; p1++) {
        center[p1] = 0.0;
        for (p2 = 0; p2 < 3; p2++) {
          center[p1] += c[p1 + 3 * p2] * dv0[p2];
        }
      }
    }
  }
}

/*
 * File trailer for computerCircleWith3point.c
 *
 * [EOF]
 */
