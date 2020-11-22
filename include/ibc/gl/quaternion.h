// =============================================================================
//  quaternion.h
//
//  MIT License
//
//  Copyright (c) 2019 Dairoku Sekiguchi
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
// =============================================================================
/*!
  \file     ibc/gl/quaternion.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/10
  \brief    Header file for Quaternion
*/

#ifndef IBC_QUATERNION_H_
#define IBC_QUATERNION_H_

// Includes --------------------------------------------------------------------
#include <iostream>
#include <math.h>
#include "ibc/gl/vector.h"
#include "ibc/gl/matrix.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gl
 {
  // ---------------------------------------------------------------------------
  // QuaternionBase class
  // ---------------------------------------------------------------------------
  template <typename QuaternionType> class  QuaternionBase
  {
  public:
    // Constants ---------------------------------------------------------------
    // Quaternion Vector Index
    //  Please note that this is a just one way to assign components of quaternion
    //  (The following might be the most common assignment?)
    static const int Qx   = 0;
    static const int Qy   = 1;
    static const int Qz   = 2;
    static const int Qw   = 3;

    // Member variables (public) -----------------------------------------------
    QuaternionType  mQ[4];

    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // QuaternionBase
    // -------------------------------------------------------------------------
    QuaternionBase()
    {
      setZero(mQ);
    }
    // -------------------------------------------------------------------------
    // QuaternionBase
    // -------------------------------------------------------------------------
    QuaternionBase(QuaternionType inX, QuaternionType inY, QuaternionType inZ, QuaternionType inW)
    {
      set(inX, inY, inZ, inW);
    }
    // -------------------------------------------------------------------------
    // QuaternionBase
    // -------------------------------------------------------------------------
    QuaternionBase(const QuaternionType inQ[])
    {
      set(inQ);
    }
    // -------------------------------------------------------------------------
    // QuaternionBase (copy constructor)
    // -------------------------------------------------------------------------
    QuaternionBase(const QuaternionBase<QuaternionType> &inQuaternion)
    {
      set(inQuaternion);
    }
    // -------------------------------------------------------------------------
    // ~QuaternionBase
    // -------------------------------------------------------------------------
    virtual ~QuaternionBase()
    {
    }
    // Operator overloading ----------------------------------------------------
    // -------------------------------------------------------------------------
    // =
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &operator=(const QuaternionBase<QuaternionType> &inQuaternion)
    {
      return set(inQuaternion);
    }
    // -------------------------------------------------------------------------
    // []
    // -------------------------------------------------------------------------
    QuaternionType operator[](size_t inIndex)
    {
      if (inIndex > 3)
        inIndex = 3;
      return mQ[inIndex];
    }
    // -------------------------------------------------------------------------
    // []
    // -------------------------------------------------------------------------
    QuaternionType &operator[](size_t inIndex) const
    {
      if (inIndex > 3)
        inIndex = 3;
      return mQ[inIndex];
    }
    // -------------------------------------------------------------------------
    // +
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> operator+(const QuaternionBase<QuaternionType> &inQuaternion)
    {
      QuaternionBase<QuaternionType> quaternion(*this);
      return quaternion.add(inQuaternion);
    }
    // -------------------------------------------------------------------------
    // +
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> operator+(const QuaternionType inOffset) const
    {
      QuaternionBase<QuaternionType> quaternion(*this);
      return quaternion.add(inOffset);
    }
    // -------------------------------------------------------------------------
    // +=
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &operator+=(const QuaternionType inOffset)
    {
      return add(inOffset);
    }
    // -------------------------------------------------------------------------
    // +=
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &operator+=(const QuaternionBase<QuaternionType> &inQuaternion)
    {
      return add(inQuaternion);
    }
    // -------------------------------------------------------------------------
    // -
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> operator-(const QuaternionBase<QuaternionType> &inQuaternion)
    {
      QuaternionBase<QuaternionType> quaternion(*this);
      return quaternion.sub(inQuaternion);
    }
    // -------------------------------------------------------------------------
    // -
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> operator-(const QuaternionType inOffset) const
    {
      QuaternionBase<QuaternionType> quaternion(*this);
      return quaternion.sub(inOffset);
    }
    // -------------------------------------------------------------------------
    // -=
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &operator-=(const QuaternionBase<QuaternionType> &inQuaternion)
    {
      return sub(inQuaternion);
    }
    // -------------------------------------------------------------------------
    // -=
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &operator-=(const QuaternionType inOffset)
    {
      return sub(inOffset);
    }
    // -------------------------------------------------------------------------
    // *
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> operator*(const QuaternionBase<QuaternionType> &inQuaternion)
    {
      QuaternionBase<QuaternionType> quaternion(*this);
      return quaternion.multi(inQuaternion);
    }
    // -------------------------------------------------------------------------
    // *
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> operator*(const QuaternionType inScale) const
    {
      QuaternionBase<QuaternionType> quaternion(*this);
      return quaternion.scale(inScale);
    }
    // -------------------------------------------------------------------------
    // *=
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &operator*=(const QuaternionBase<QuaternionType> &inQuaternion)
    {
      return multi(inQuaternion);
    }
    // -------------------------------------------------------------------------
    // *=
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &operator*=(const QuaternionType inScale)
    {
      return scale(inScale);
    }
    // -------------------------------------------------------------------------
    // /
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> operator/(const QuaternionType inDiv) const
    {
      QuaternionBase<QuaternionType> quaternion(*this);
      return quaternion.div(inDiv);
    }
    // -------------------------------------------------------------------------
    // /=
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &operator/=(const QuaternionType inDiv)
    {
      return div(inDiv);
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &set(
                    QuaternionType inX, QuaternionType inY,
                    QuaternionType inZ, QuaternionType inW)
    {
      QuaternionBase<QuaternionType>::set(mQ, inX, inY, inZ, inW);
      return *this;
    }
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &set(const QuaternionType inQ[])
    {
      QuaternionBase<QuaternionType>::set(mQ, inQ);
      return *this;
    }
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &set(const QuaternionBase<QuaternionType> &inQuaternion)
    {
      mQ[0] = inQuaternion.mQ[0];
      mQ[1] = inQuaternion.mQ[1];
      mQ[2] = inQuaternion.mQ[2];
      mQ[3] = inQuaternion.mQ[3];
      return *this;
    }
    // -------------------------------------------------------------------------
    // setZero
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &setZero()
    {
      QuaternionBase<QuaternionType>::setZero(mQ);
      return *this;
    }
    // -------------------------------------------------------------------------
    // setIdentity
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &setIdentity()
    {
      QuaternionBase<QuaternionType>::setIdentity(mQ);
      return *this;
    }
    // -------------------------------------------------------------------------
    // setAngle
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &setAngle(
        QuaternionType inAngle, const VectorBase<QuaternionType> &inAxis)
    {
      QuaternionBase<QuaternionType>::setAngle(mQ, inAngle, inAxis.mVec);
      return *this;
    }
    // -------------------------------------------------------------------------
    // add
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &add(const QuaternionBase<QuaternionType> &inQuaternion)
    {
      QuaternionBase<QuaternionType>::add(mQ, mQ, inQuaternion.mQ);
      return *this;
    }
    // -------------------------------------------------------------------------
    // add
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &add(QuaternionType inOffset)
    {
      QuaternionBase<QuaternionType>::add(mQ, mQ, inOffset);
      return *this;
    }
    // -------------------------------------------------------------------------
    // sub
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &sub(const QuaternionBase<QuaternionType> &inQuaternion)
    {
      QuaternionBase<QuaternionType>::sub(mQ, mQ, inQuaternion.mQ);
      return *this;
    }
    // -------------------------------------------------------------------------
    // sub
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &sub(QuaternionType inOffset)
    {
      QuaternionBase<QuaternionType>::sub(mQ, mQ, inOffset);
      return *this;
    }
    // -------------------------------------------------------------------------
    // multi
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &multi(const QuaternionBase<QuaternionType> &inQuaternion)
    {
      QuaternionBase<QuaternionType> q0(*this);
      if (this == &inQuaternion)
      {
        QuaternionBase<QuaternionType> q1(inQuaternion);
        QuaternionBase<QuaternionType>::multi(mQ, q0.mQ, q1.mQ);
      }
      else
      {
        QuaternionBase<QuaternionType>::multi(mQ, q0.mQ, inQuaternion.mQ);
      }
      return *this;
    }
    // -------------------------------------------------------------------------
    // scale
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &scale(QuaternionType inScale)
    {
      QuaternionBase<QuaternionType>::scale(mQ, inScale);
      return *this;
    }
    // -------------------------------------------------------------------------
    // div
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &div(QuaternionType inScale)
    {
      QuaternionBase<QuaternionType>::div(mQ, inScale);
      return *this;
    }
    // -------------------------------------------------------------------------
    // length
    // -------------------------------------------------------------------------
    QuaternionType length() const
    {
      return QuaternionBase<QuaternionType>::length(mQ);
    }
    // -------------------------------------------------------------------------
    // dot
    // -------------------------------------------------------------------------
    QuaternionType dot(const QuaternionBase<QuaternionType> &inQuaternion) const
    {
      return QuaternionBase<QuaternionType>::dot(mQ);
    }
    // -------------------------------------------------------------------------
    // normalize
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> &normalize()
    {
      QuaternionBase<QuaternionType>::normalize(mQ);
      return *this;
    }
    // -------------------------------------------------------------------------
    // normalizedQuaternion
    // -------------------------------------------------------------------------
    QuaternionBase<QuaternionType> normalizedQuaternion() const
    {
      QuaternionBase<QuaternionType> quaternion(*this);
      return quaternion.normalize();
    }
    // -------------------------------------------------------------------------
    // rotationMatrix
    // -------------------------------------------------------------------------
    MatrixBase<QuaternionType> rotationMatrix() const
    {
      MatrixBase<QuaternionType> matrix;
      QuaternionBase<QuaternionType>::rotationMatrix((QuaternionType *)matrix.mMat, mQ);
      return matrix;
    }
    // -------------------------------------------------------------------------
    // get_glRotationMatrix
    // -------------------------------------------------------------------------
    void get_glRotationMatrix(QuaternionType outMat[4][4]) const
    {
      QuaternionBase<QuaternionType>::glRotationMatrix(outMat, mQ);
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    static void  set(QuaternionType outDst[],
                     QuaternionType inX, QuaternionType inY,
                     QuaternionType inZ, QuaternionType inW)
    {
      outDst[Qx] = inX;
      outDst[Qy] = inY;
      outDst[Qz] = inZ;
      outDst[Qw] = inW;
    }
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    static void  set(QuaternionType outDst[], const QuaternionType inSrc[])
    {
      outDst[0] = inSrc[0];
      outDst[1] = inSrc[1];
      outDst[2] = inSrc[2];
      outDst[3] = inSrc[3];
    }
    // -------------------------------------------------------------------------
    // setZero
    // -------------------------------------------------------------------------
    static void  setZero(QuaternionType outDst[])
    {
      set(outDst,
          (QuaternionType )0.0,
          (QuaternionType )0.0,
          (QuaternionType )0.0,
          (QuaternionType )0.0);
    }
    // -------------------------------------------------------------------------
    // setIdentity
    // -------------------------------------------------------------------------
    static void  setIdentity(QuaternionType outDst[])
    {
      set(outDst,
          (QuaternionType )0.0,
          (QuaternionType )0.0,
          (QuaternionType )0.0,
          (QuaternionType )1.0);
    }
    // -------------------------------------------------------------------------
    // setAngle
    // -------------------------------------------------------------------------
    static void  setAngle(QuaternionType outDst[], QuaternionType inAngle, const QuaternionType inAxis[])
    {
      QuaternionType  v[3];
      VectorBase<QuaternionType>::set(v, inAxis);
      VectorBase<QuaternionType>::normalize(v);
      VectorBase<QuaternionType>::scale(v, (QuaternionType )sin(inAngle /2.0));
      set(outDst,
          v[0], v[1], v[2],
          (QuaternionType )cos(inAngle / 2.0));
    }
    // -------------------------------------------------------------------------
    // add
    // -------------------------------------------------------------------------
    static void  add(QuaternionType outDst[], const QuaternionType inSrc0[], const QuaternionType inSrc1[])
    {
      outDst[0] = inSrc0[0] + inSrc1[0];
      outDst[1] = inSrc0[1] + inSrc1[1];
      outDst[2] = inSrc0[2] + inSrc1[2];
      outDst[3] = inSrc0[3] + inSrc1[3];
    }
    // -------------------------------------------------------------------------
    // add
    // -------------------------------------------------------------------------
    static void  add(QuaternionType outDst[], const QuaternionType inSrc[], QuaternionType inOffset)
    {
      outDst[0] = inSrc[0] + inOffset;
      outDst[1] = inSrc[1] + inOffset;
      outDst[2] = inSrc[2] + inOffset;
      outDst[3] = inSrc[3] + inOffset;
    }
    // -------------------------------------------------------------------------
    // sub
    // -------------------------------------------------------------------------
    static void  sub(QuaternionType outDst[], const QuaternionType inSrc0[], const QuaternionType inSrc1[])
    {
      outDst[0] = inSrc0[0] - inSrc1[0];
      outDst[1] = inSrc0[1] - inSrc1[1];
      outDst[2] = inSrc0[2] - inSrc1[2];
      outDst[3] = inSrc0[3] - inSrc1[3];
    }
    // -------------------------------------------------------------------------
    // sub
    // -------------------------------------------------------------------------
    static void  sub(QuaternionType outDst[], const QuaternionType inSrc[], QuaternionType inOffset)
    {
      outDst[0] = inSrc[0] - inOffset;
      outDst[1] = inSrc[1] - inOffset;
      outDst[2] = inSrc[2] - inOffset;
      outDst[3] = inSrc[3] - inOffset;
    }
    // -------------------------------------------------------------------------
    // multi (A = B * C)
    // -------------------------------------------------------------------------
    static void  multi(QuaternionType outA[], const QuaternionType inB[], const QuaternionType inC[])
    {
      outA[Qx] = inB[Qw] * inC[Qx] + inB[Qx] * inC[Qw] + inB[Qy] * inC[Qz] - inB[Qz] * inC[Qy];
      outA[Qy] = inB[Qw] * inC[Qy] - inB[Qx] * inC[Qz] + inB[Qy] * inC[Qw] + inB[Qz] * inC[Qx];
      outA[Qz] = inB[Qw] * inC[Qz] + inB[Qx] * inC[Qy] - inB[Qy] * inC[Qx] + inB[Qz] * inC[Qw];
      outA[Qw] = inB[Qw] * inC[Qw] - inB[Qx] * inC[Qx] - inB[Qy] * inC[Qy] - inB[Qz] * inC[Qz];
    }
    // -------------------------------------------------------------------------
    // scale
    // -------------------------------------------------------------------------
    static void scale(QuaternionType ioQ[], QuaternionType inScale)
    {
      ioQ[0] *= inScale;
      ioQ[1] *= inScale;
      ioQ[2] *= inScale;
      ioQ[3] *= inScale;
    }
    // -------------------------------------------------------------------------
    // div
    // -------------------------------------------------------------------------
    static void div(QuaternionType ioQ[], QuaternionType inDiv)
    {
      ioQ[0] /= inDiv;
      ioQ[1] /= inDiv;
      ioQ[2] /= inDiv;
      ioQ[3] /= inDiv;
    }
    // -------------------------------------------------------------------------
    // length
    // -------------------------------------------------------------------------
    static QuaternionType  length(const QuaternionType inSrc[])
    {
      return (QuaternionType )sqrt((QuaternionType )(
            inSrc[0] * inSrc[0] +
            inSrc[1] * inSrc[1] +
            inSrc[2] * inSrc[2] +
            inSrc[3] * inSrc[3]));
    }
    // -------------------------------------------------------------------------
    // dot
    // -------------------------------------------------------------------------
    static QuaternionType dot(const QuaternionType inSrc0[], const QuaternionType inSrc1[])
    {
      return (inSrc0[0] * inSrc1[0] +
              inSrc0[1] * inSrc1[1] +
              inSrc0[2] * inSrc1[2] +
              inSrc0[3] * inSrc1[3]);
    }
    // -------------------------------------------------------------------------
    // normalize
    // -------------------------------------------------------------------------
    static bool normalize(QuaternionType ioQ[])
    {
      QuaternionType  l = length(ioQ);
      if (l == 0)
        return false;
      div(ioQ, l);
      return true;
    }
    // -------------------------------------------------------------------------
    // rotationMatrix
    // -------------------------------------------------------------------------
    static void rotationMatrix(QuaternionType outMat[], const QuaternionType inQ[])
    {
      QuaternionType glMat[4][4];

      glRotationMatrix(glMat, inQ);
      MatrixBase<QuaternionType>::setTransposedMatrix(outMat, (QuaternionType *)glMat);
    }
    // -------------------------------------------------------------------------
    // glRotationMatrix
    // -------------------------------------------------------------------------
    static void glRotationMatrix(QuaternionType outMat[4][4], const QuaternionType inQ[])
    {
      outMat[0][0] = 1.0 - 2.0 * (inQ[Qy] * inQ[Qy] + inQ[Qz] * inQ[Qz]);
      outMat[0][1] =       2.0 * (inQ[Qx] * inQ[Qy] - inQ[Qz] * inQ[Qw]);
      outMat[0][2] =       2.0 * (inQ[Qz] * inQ[Qx] + inQ[Qy] * inQ[Qw]);
      outMat[0][3] = 0.0;

      outMat[1][0] =       2.0 * (inQ[Qx] * inQ[Qy] + inQ[Qz] * inQ[Qw]);
      outMat[1][1] = 1.0 - 2.0 * (inQ[Qz] * inQ[Qz] + inQ[Qx] * inQ[Qx]);
      outMat[1][2] =       2.0 * (inQ[Qy] * inQ[Qz] - inQ[Qx] * inQ[Qw]);
      outMat[1][3] = 0.0;

      outMat[2][0] =       2.0 * (inQ[Qz] * inQ[Qx] - inQ[Qy] * inQ[Qw]);
      outMat[2][1] =       2.0 * (inQ[Qy] * inQ[Qz] + inQ[Qx] * inQ[Qw]);
      outMat[2][2] = 1.0 - 2.0 * (inQ[Qy] * inQ[Qy] + inQ[Qx] * inQ[Qx]);
      outMat[2][3] = 0.0;

      outMat[3][0] = 0.0;
      outMat[3][1] = 0.0;
      outMat[3][2] = 0.0;
      outMat[3][3] = 1.0;
    }
  };

  // Operator overloading (for other types) ------------------------------------
  // ex) a = 2 + b
  template <typename T0, typename T1> QuaternionBase<T1> operator+(T0 inOffset, const QuaternionBase<T1> &inQuaternion)
  {
    return inQuaternion + (T1 )inOffset;
  }
  // ex) a = 2 - b (this might be misleading...)
  template <typename T0, typename T1> QuaternionBase<T1> operator-(T0 inOffset, const QuaternionBase<T1> &inQuaternion)
  {
    return -1 * inQuaternion + (T1 )inOffset;
  }
  // ex) a = 2 * b
  template <typename T0, typename T1> QuaternionBase<T1> operator*(T0 inScale, const QuaternionBase<T1> &inQuaternion)
  {
    return inQuaternion * (T1 )inScale;
  }
  template <typename T0> std::ostream& operator<<(std::ostream &os, const QuaternionBase<T0> &inQuaternion)
  {
    os << "{";
    os << "x = " << inQuaternion.mQ[QuaternionBase<T0>::Qx] << ", ";
    os << "y = " << inQuaternion.mQ[QuaternionBase<T0>::Qy] << ", ";
    os << "z = " << inQuaternion.mQ[QuaternionBase<T0>::Qz] << ", ";
    os << "w = " << inQuaternion.mQ[QuaternionBase<T0>::Qw];
    os << "}";
    return os;
  }

  // ---------------------------------------------------------------------------
  // Typedefs
  // ---------------------------------------------------------------------------
  typedef QuaternionBase<double>  Quaternion;
  typedef QuaternionBase<float>  QuaternionF;
//typedef QuaternionBase<int>  QuaternionI; // < We don't need this type
 };
};

#endif  // #ifdef IBC_QUATERNION_H_
