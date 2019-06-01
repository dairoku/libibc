// =============================================================================
//  vector.h
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
  \file     ibc/gl/vector.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/02/24
  \brief    Header file for the Vector (3 dimensional) class
*/

#ifndef IBC_VECTOR_H_
#define IBC_VECTOR_H_

// Includes --------------------------------------------------------------------
#include <iostream>
#include <math.h>

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gl
 {
  // ---------------------------------------------------------------------------
  // VectorBase class
  // ---------------------------------------------------------------------------
  template <typename VectorType> class  VectorBase
  {
  public:
    // Member variables (public) -----------------------------------------------
    VectorType  mVec[3];

    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // VectorBase
    // -------------------------------------------------------------------------
    VectorBase()
    {
      setZero(mVec);
    }
    // -------------------------------------------------------------------------
    // VectorBase
    // -------------------------------------------------------------------------
    VectorBase(VectorType inX, VectorType inY, VectorType inZ)
    {
      set(inX, inY, inZ);
    }
    // -------------------------------------------------------------------------
    // VectorBase
    // -------------------------------------------------------------------------
    VectorBase(const VectorType inVec[])
    {
      set(inVec);
    }
    // -------------------------------------------------------------------------
    // VectorBase (copy constructor)
    // -------------------------------------------------------------------------
    VectorBase(const VectorBase<VectorType> &inVector)
    {
      set(inVector);
    }
    // -------------------------------------------------------------------------
    // ~VectorBase
    // -------------------------------------------------------------------------
    virtual ~VectorBase()
    {
    }
    // Operator overloading ----------------------------------------------------
    // -------------------------------------------------------------------------
    // =
    // -------------------------------------------------------------------------
    /*VectorBase<VectorType> &operator=(const VectorBase<VectorType> &inVector)
    {
      return set(inVector);
    }*/
    // -------------------------------------------------------------------------
    // []
    // -------------------------------------------------------------------------
    VectorType operator[](size_t inIndex) const
    {
      if (inIndex > 2)
        inIndex = 2;
      return mVec[inIndex];
    }
    // -------------------------------------------------------------------------
    // []
    // -------------------------------------------------------------------------
    VectorType &operator[](size_t inIndex)
    {
      if (inIndex > 2)
        inIndex = 2;
      return mVec[inIndex];
    }
    // -------------------------------------------------------------------------
    // +
    // -------------------------------------------------------------------------
    VectorBase<VectorType> operator+(const VectorBase<VectorType> &inVector)
    {
      VectorBase<VectorType> vector(*this);
      return vector.add(inVector);
    }
    // -------------------------------------------------------------------------
    // +
    // -------------------------------------------------------------------------
    VectorBase<VectorType> operator+(const VectorType inOffset) const
    {
      VectorBase<VectorType> vector(*this);
      return vector.add(inOffset);
    }
    // -------------------------------------------------------------------------
    // +=
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &operator+=(const VectorType inOffset)
    {
      return add(inOffset);
    }
    // -------------------------------------------------------------------------
    // +=
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &operator+=(const VectorBase<VectorType> &inVector)
    {
      return add(inVector);
    }
    // -------------------------------------------------------------------------
    // -
    // -------------------------------------------------------------------------
    VectorBase<VectorType> operator-(const VectorBase<VectorType> &inVector)
    {
      VectorBase<VectorType> vector(*this);
      return vector.sub(inVector);
    }
    // -------------------------------------------------------------------------
    // -
    // -------------------------------------------------------------------------
    VectorBase<VectorType> operator-(const VectorType inOffset) const
    {
      VectorBase<VectorType> vector(*this);
      return vector.sub(inOffset);
    }
    // -------------------------------------------------------------------------
    // -=
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &operator-=(const VectorBase<VectorType> &inVector)
    {
      return sub(inVector);
    }
    // -------------------------------------------------------------------------
    // -=
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &operator-=(const VectorType inOffset)
    {
      return sub(inOffset);
    }
    // -------------------------------------------------------------------------
    // *
    // -------------------------------------------------------------------------
    VectorBase<VectorType> operator*(const VectorBase<VectorType> &inVector)
    {
      VectorBase<VectorType> vector(*this);
      return vector.cross(inVector);
    }
    // -------------------------------------------------------------------------
    // *
    // -------------------------------------------------------------------------
    VectorBase<VectorType> operator*(const VectorType inScale) const
    {
      VectorBase<VectorType> vector(*this);
      return vector.scale(inScale);
    }
    // -------------------------------------------------------------------------
    // *=
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &operator*=(const VectorBase<VectorType> &inVector)
    {
      return cross(inVector);
    }
    // -------------------------------------------------------------------------
    // *=
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &operator*=(const VectorType inScale)
    {
      return scale(inScale);
    }
    // -------------------------------------------------------------------------
    // /
    // -------------------------------------------------------------------------
    VectorBase<VectorType> operator/(const VectorType inDiv) const
    {
      VectorBase<VectorType> vector(*this);
      return vector.div(inDiv);
    }
    // -------------------------------------------------------------------------
    // /=
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &operator/=(const VectorType inDiv)
    {
      return div(inDiv);
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &set(VectorType inX, VectorType inY, VectorType inZ)
    {
      VectorBase<VectorType>::set(mVec, inX, inY, inZ);
      return *this;
    }
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &set(const VectorType inVec[])
    {
      VectorBase<VectorType>::set(mVec, inVec);
      return *this;
    }
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &set(const VectorBase<VectorType> &inVector)
    {
      mVec[0] = inVector.mVec[0];
      mVec[1] = inVector.mVec[1];
      mVec[2] = inVector.mVec[2];
      return *this;
    }
    // -------------------------------------------------------------------------
    // setZero
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &setZero()
    {
      VectorBase<VectorType>::setZero(mVec);
      return *this;
    }
    // -------------------------------------------------------------------------
    // add
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &add(const VectorBase<VectorType> &inVector)
    {
      VectorBase<VectorType>::add(mVec, mVec, inVector.mVec);
      return *this;
    }
    // -------------------------------------------------------------------------
    // add
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &add(VectorType inOffset)
    {
      VectorBase<VectorType>::add(mVec, mVec, inOffset);
      return *this;
    }
    // -------------------------------------------------------------------------
    // sub
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &sub(const VectorBase<VectorType> &inVector)
    {
      VectorBase<VectorType>::sub(mVec, mVec, inVector.mVec);
      return *this;
    }
    // -------------------------------------------------------------------------
    // sub
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &sub(VectorType inOffset)
    {
      VectorBase<VectorType>::sub(mVec, mVec, inOffset);
      return *this;
    }
    // -------------------------------------------------------------------------
    // cross
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &cross(const VectorBase<VectorType> &inVector)
    {
      VectorBase<VectorType> vector0(*this);
      if (this == &inVector)
      {
        VectorBase<VectorType> vector1(inVector);
        VectorBase<VectorType>::cross(mVec, vector0.mVec, vector1.mVec);
      }
      else
      {
        VectorBase<VectorType>::cross(mVec, vector0.mVec, inVector.mVec);
      }
      return *this;
    }
    // -------------------------------------------------------------------------
    // scale
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &scale(VectorType inScale)
    {
      VectorBase<VectorType>::scale(mVec, inScale);
      return *this;
    }
    // -------------------------------------------------------------------------
    // div
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &div(VectorType inScale)
    {
      VectorBase<VectorType>::div(mVec, inScale);
      return *this;
    }
    // -------------------------------------------------------------------------
    // length
    // -------------------------------------------------------------------------
    VectorType length() const
    {
      return VectorBase<VectorType>::length(mVec);
    }
    // -------------------------------------------------------------------------
    // dot
    // -------------------------------------------------------------------------
    VectorType dot(const VectorBase<VectorType> &inVector) const
    {
      return VectorBase<VectorType>::dot(mVec, inVector.mVec);
    }
    // -------------------------------------------------------------------------
    // angle
    // -------------------------------------------------------------------------
    VectorType angle(const VectorBase<VectorType> &inVector) const
    {
      return VectorBase<VectorType>::angle(mVec, inVector.mVec);
    }
    // -------------------------------------------------------------------------
    // normalize
    // -------------------------------------------------------------------------
    VectorBase<VectorType> &normalize()
    {
      VectorBase<VectorType>::normalize(mVec);
      return *this;
    }
    // -------------------------------------------------------------------------
    // getNormalizedVector
    // -------------------------------------------------------------------------
    VectorBase<VectorType> getNormalizedVector() const
    {
      VectorBase<VectorType> vector(*this);
      return vector.normalize();
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    static void  set(VectorType outDst[], VectorType inX, VectorType inY, VectorType inZ)
    {
      outDst[0] = inX;
      outDst[1] = inY;
      outDst[2] = inZ;
    }
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    static void  set(VectorType outDst[], const VectorType inSrc[])
    {
      outDst[0] = inSrc[0];
      outDst[1] = inSrc[1];
      outDst[2] = inSrc[2];
    }
    // -------------------------------------------------------------------------
    // setZero
    // -------------------------------------------------------------------------
    static void  setZero(VectorType outDst[])
    {
      set(outDst,
          (VectorType )0.0,
          (VectorType )0.0,
          (VectorType )0.0);
    }
    // -------------------------------------------------------------------------
    // add
    // -------------------------------------------------------------------------
    static void  add(VectorType outDst[], const VectorType inSrc0[], const VectorType inSrc1[])
    {
      outDst[0] = inSrc0[0] + inSrc1[0];
      outDst[1] = inSrc0[1] + inSrc1[1];
      outDst[2] = inSrc0[2] + inSrc1[2];
    }
    // -------------------------------------------------------------------------
    // add
    // -------------------------------------------------------------------------
    static void  add(VectorType outDst[], const VectorType inSrc[], VectorType inOffset)
    {
      outDst[0] = inSrc[0] + inOffset;
      outDst[1] = inSrc[1] + inOffset;
      outDst[2] = inSrc[2] + inOffset;
    }
    // -------------------------------------------------------------------------
    // sub
    // -------------------------------------------------------------------------
    static void  sub(VectorType outDst[], const VectorType inSrc0[], const VectorType inSrc1[])
    {
      outDst[0] = inSrc0[0] - inSrc1[0];
      outDst[1] = inSrc0[1] - inSrc1[1];
      outDst[2] = inSrc0[2] - inSrc1[2];
    }
    // -------------------------------------------------------------------------
    // sub
    // -------------------------------------------------------------------------
    static void  sub(VectorType outDst[], const VectorType inSrc[], VectorType inOffset)
    {
      outDst[0] = inSrc[0] - inOffset;
      outDst[1] = inSrc[1] - inOffset;
      outDst[2] = inSrc[2] - inOffset;
    }
    // -------------------------------------------------------------------------
    // cross
    // -------------------------------------------------------------------------
    static void  cross(VectorType outDst[], const VectorType inSrc0[], const VectorType inSrc1[])
    {
      outDst[0] = inSrc0[1] * inSrc1[2] - inSrc0[2] * inSrc1[1];
      outDst[1] = inSrc0[2] * inSrc1[0] - inSrc0[0] * inSrc1[2];
      outDst[2] = inSrc0[0] * inSrc1[1] - inSrc0[1] * inSrc1[0];
    }
    // -------------------------------------------------------------------------
    // scale
    // -------------------------------------------------------------------------
    static void scale(VectorType ioVec[], VectorType inScale)
    {
      ioVec[0] *= inScale;
      ioVec[1] *= inScale;
      ioVec[2] *= inScale;
    }
    // -------------------------------------------------------------------------
    // div
    // -------------------------------------------------------------------------
    static void div(VectorType ioVec[], VectorType inDiv)
    {
      ioVec[0] /= inDiv;
      ioVec[1] /= inDiv;
      ioVec[2] /= inDiv;
    }
    // -------------------------------------------------------------------------
    // length
    // -------------------------------------------------------------------------
    static VectorType  length(const VectorType inSrc[])
    {
      return (VectorType )sqrt((VectorType )(
            inSrc[0] * inSrc[0] +
            inSrc[1] * inSrc[1] +
            inSrc[2] * inSrc[2]));
    }
    // -------------------------------------------------------------------------
    // dot
    // -------------------------------------------------------------------------
    static VectorType dot(const VectorType inVec0[], const VectorType inVec1[])
    {
      return (inVec0[0] * inVec1[0] +
              inVec0[1] * inVec1[1] +
              inVec0[2] * inVec1[2]);
    }
    // -------------------------------------------------------------------------
    // angle
    // -------------------------------------------------------------------------
    static VectorType angle(const VectorType inVec0[], const VectorType inVec1[])
    {
      VectorType  v, cosTheta;
      //
      v = length(inVec0) * length(inVec1);
      if (v == 0.0)
        return 0.0;
      cosTheta = dot(inVec0, inVec1) / v;
      if (cosTheta >= 1.0)
        return 0.0;
      if (cosTheta <= -1.0)
        return M_PI;
      return (VectorType )::acos(cosTheta);
    }
    // -------------------------------------------------------------------------
    // normalize
    // -------------------------------------------------------------------------
    static bool normalize(VectorType ioVec[])
    {
      VectorType  l = length(ioVec);
      if (l == 0)
        return false;
      div(ioVec, l);
      return true;
    }
  };

  // Operator overloading (for other types) ------------------------------------
  // ex) a = 2 + b
  template <typename T0, typename T1> VectorBase<T1> operator+(T0 inOffset, const VectorBase<T1> &inVector)
  {
    return inVector + (T1 )inOffset;
  }
  // ex) a = 2 - b (this might be misleading...)
  template <typename T0, typename T1> VectorBase<T1> operator-(T0 inOffset, const VectorBase<T1> &inVector)
  {
    return -1 * inVector + (T1 )inOffset;
  }
  // ex) a = 2 * b
  template <typename T0, typename T1> VectorBase<T1> operator*(T0 inScale, const VectorBase<T1> &inVector)
  {
    return inVector * (T1 )inScale;
  }
  template <typename T0> std::ostream& operator<<(std::ostream &os, const VectorBase<T0> &inVector)
  {
    os << "{";
    os << inVector.mVec[0] << ", ";
    os << inVector.mVec[1] << ", ";
    os << inVector.mVec[2];
    os << "}";
    return os;
  }

  // ---------------------------------------------------------------------------
  // Typedefs
  // ---------------------------------------------------------------------------
  typedef VectorBase<double>  Vector;
  typedef VectorBase<float>  VectorF;
  typedef VectorBase<int>  VectorI;
  //
  typedef VectorBase<GLdouble>  glVector3d;
  typedef VectorBase<GLfloat>   glVector3f;
  typedef VectorBase<GLint>     glVector3i;
 };
};

#endif  // #ifdef IBC_VECTOR_H_

