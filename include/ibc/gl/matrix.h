// =============================================================================
//  matrix.h
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
  \brief    Header file for handling the image

  This file defines the quaternion class for the IBC Library
*/

#ifndef IBC_MATRIX_H_
#define IBC_MATRIX_H_

// Includes --------------------------------------------------------------------
#include <iostream>

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gl
 {
  // ---------------------------------------------------------------------------
  // MatrixBase class
  // ---------------------------------------------------------------------------
  template <typename MatrixType> class  MatrixBase
  {
  public:
    // Constants ---------------------------------------------------------------
    static const int MATRIX_ROW_SIZE   = 4;
    static const int MATRIX_COL_SIZE   = 4;
    static const int MATRIX_SIZE       = 16;

    // Member variables (public) -----------------------------------------------
    MatrixType  mMat[4][4];

    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // MatrixBase
    // -------------------------------------------------------------------------
    MatrixBase()
    {
      setZero();
    }
    // -------------------------------------------------------------------------
    // MatrixBase
    // -------------------------------------------------------------------------
    MatrixBase(MatrixType inM0,  MatrixType inM1,  MatrixType inM2,  MatrixType inM3,
               MatrixType inM4,  MatrixType inM5,  MatrixType inM6,  MatrixType inM7,
               MatrixType inM8,  MatrixType inM9,  MatrixType inM10, MatrixType inM11,
               MatrixType inM12, MatrixType inM13, MatrixType inM14, MatrixType inM15)
    {
      mMat[0][0] = inM0;  mMat[0][1] = inM1;  mMat[0][2] = inM2;  mMat[0][3] = inM3;
      mMat[1][0] = inM4;  mMat[1][1] = inM5;  mMat[1][2] = inM6;  mMat[1][3] = inM7;
      mMat[2][0] = inM8;  mMat[2][1] = inM9;  mMat[2][2] = inM10; mMat[2][3] = inM11;
      mMat[3][0] = inM12; mMat[3][1] = inM13; mMat[3][2] = inM14; mMat[3][3] = inM15;
    }
    // -------------------------------------------------------------------------
    // MatrixBase
    // -------------------------------------------------------------------------
    MatrixBase(const MatrixType inMat[4][4])
    {
      set(inMat);
    }
    // -------------------------------------------------------------------------
    // MatrixBase
    // -------------------------------------------------------------------------
    MatrixBase(const MatrixType inMat[])
    {
      set(inMat);
    }
    // -------------------------------------------------------------------------
    // MatrixBase (copy constructor)
    // -------------------------------------------------------------------------
    MatrixBase(const MatrixBase<MatrixType> &inMatrix)
    {
      set(inMatrix);
    }
    // -------------------------------------------------------------------------
    // ~MatrixBase
    // -------------------------------------------------------------------------
    virtual ~MatrixBase()
    {
    }
    // Operator overloading ----------------------------------------------------
    // -------------------------------------------------------------------------
    // =
    // -------------------------------------------------------------------------
    /*MatrixBase<MatrixType> &operator=(const MatrixBase<MatrixType> &inMatrix)
    {
      return set(inMatrix);
    }*/
    // -------------------------------------------------------------------------
    // []
    // -------------------------------------------------------------------------
    MatrixType *operator[](size_t inRow)
    {
      if (inRow >= MATRIX_ROW_SIZE)
        inRow = MATRIX_ROW_SIZE - 1;
      return mMat[inRow];
    }
    // -------------------------------------------------------------------------
    // +
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> operator+(const MatrixBase<MatrixType> &inMatrix)
    {
      MatrixBase<MatrixType> matrix(*this);
      return matrix.add(inMatrix);
    }
    // -------------------------------------------------------------------------
    // +
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> operator+(const MatrixType inOffset) const
    {
      MatrixBase<MatrixType> matrix(*this);
      return matrix.add(inOffset);
    }
    // -------------------------------------------------------------------------
    // +=
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &operator+=(const MatrixBase<MatrixType> &inMatrix)
    {
      return add(inMatrix);
    }
    // -------------------------------------------------------------------------
    // +=
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &operator+=(const MatrixType inOffset)
    {
      return add(inOffset);
    }
    // -------------------------------------------------------------------------
    // -
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> operator-(const MatrixBase<MatrixType> &inMatrix)
    {
      MatrixBase<MatrixType> matrix(*this);
      return matrix.sub(inMatrix);
    }
    // -------------------------------------------------------------------------
    // -
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> operator-(const MatrixType inOffset) const
    {
      MatrixBase<MatrixType> matrix(*this);
      return matrix.sub(inOffset);
    }
    // -------------------------------------------------------------------------
    // -=
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &operator-=(const MatrixBase<MatrixType> &inMatrix)
    {
      return sub(inMatrix);
    }
    // -------------------------------------------------------------------------
    // -=
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &operator-=(const MatrixType inOffset)
    {
      return sub(inOffset);
    }
    // -------------------------------------------------------------------------
    // *
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> operator*(const MatrixBase<MatrixType> &inMatrix)
    {
      MatrixBase<MatrixType> matrix(*this);
      return matrix.multi(inMatrix);
    }
    // -------------------------------------------------------------------------
    // *
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> operator*(const MatrixType inScale) const
    {
      MatrixBase<MatrixType> matrix(*this);
      return matrix.scale(inScale);
    }
    // -------------------------------------------------------------------------
    // *=
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &operator*=(const MatrixBase<MatrixType> &inMatrix)
    {
      return multi(inMatrix);
    }
    // -------------------------------------------------------------------------
    // *=
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &operator*=(const MatrixType inScale)
    {
      return scale(inScale);
    }
    // -------------------------------------------------------------------------
    // /
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> operator/(const MatrixType inDiv) const
    {
      MatrixBase<MatrixType> matrix(*this);
      return matrix.div(inDiv);
    }
    // -------------------------------------------------------------------------
    // /=
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &operator/=(const MatrixType inDiv)
    {
      return div(inDiv);
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &set(const MatrixType inMat[4][4])
    {
      MatrixBase<MatrixType>::set((MatrixType *)mMat, (MatrixType *)inMat);
      return *this;
    }
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &set(const MatrixType inMat[])
    {
      MatrixBase<MatrixType>::set((MatrixType *)mMat, inMat);
      return *this;
    }
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &set(const MatrixBase<MatrixType> &inMatrix)
    {
      const MatrixType *srcPtr = (MatrixType *)inMatrix.mMat;
      MatrixType *dstPtr = (MatrixType *)mMat;
      for (int i = 0; i < MATRIX_SIZE; i++)
        dstPtr[i] = srcPtr[i];
      return *this;
    }
    // -------------------------------------------------------------------------
    // setTransposedMatrix
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &setTransposedMatrix(const MatrixType inMat[])
    {
      MatrixBase<MatrixType>::setTransposedMatrix((MatrixType *)mMat, inMat);
      return *this;
    }
    // -------------------------------------------------------------------------
    // getTransposedMatrix
    // -------------------------------------------------------------------------
    void getTransposedMatrix(MatrixType outMat[]) const
    {
      MatrixBase<MatrixType>::setTransposedMatrix(outMat, (MatrixType *)mMat);
      return *this;
    }
    // -------------------------------------------------------------------------
    // setZero
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &setZero()
    {
      MatrixBase<MatrixType>::setZero((MatrixType *)mMat);
      return *this;
    }
    // -------------------------------------------------------------------------
    // add
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &add(const MatrixBase<MatrixType> &inMatrix)
    {
      MatrixBase<MatrixType>::add((MatrixType *)mMat, (MatrixType *)mMat, (MatrixType *)inMatrix.mMat);
      return *this;
    }
    // -------------------------------------------------------------------------
    // add
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &add(MatrixType inOffset)
    {
      MatrixBase<MatrixType>::add((MatrixType *)mMat, (MatrixType *)mMat, inOffset);
      return *this;
    }
    // -------------------------------------------------------------------------
    // sub
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &sub(const MatrixBase<MatrixType> &inMatrix)
    {
      MatrixBase<MatrixType>::sub((MatrixType *)mMat, (MatrixType *)mMat, inMatrix.mMat);
      return *this;
    }
    // -------------------------------------------------------------------------
    // sub
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &sub(MatrixType inOffset)
    {
      MatrixBase<MatrixType>::sub((MatrixType *)mMat, (MatrixType *)mMat, inOffset);
      return *this;
    }
    // -------------------------------------------------------------------------
    // multi
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &multi(const MatrixBase<MatrixType> &inMatrix)
    {
      MatrixBase<MatrixType> matrix0(*this);
      if (this == &inMatrix)
      {
        MatrixBase<MatrixType> matrix1(inMatrix);
        MatrixBase<MatrixType>::multi((MatrixType *)mMat, (MatrixType *)matrix0.mMat, (MatrixType *)matrix1.mMat);
      }
      else
      {
        MatrixBase<MatrixType>::multi((MatrixType *)mMat, (MatrixType *)matrix0.mMat, (MatrixType *)inMatrix.mMat);
      }
      return *this;
    }
    // -------------------------------------------------------------------------
    // scale
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &scale(MatrixType inScale)
    {
      MatrixBase<MatrixType>::scale((MatrixType *)mMat, inScale);
      return *this;
    }
    // -------------------------------------------------------------------------
    // div
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &div(MatrixType inScale)
    {
      MatrixBase<MatrixType>::div((MatrixType *)mMat, inScale);
      return *this;
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    static void  set(MatrixType outDst[], const MatrixType inSrc[])
    {
      for (int i = 0; i < MATRIX_SIZE; i++)
        outDst[i] = inSrc[i];
    }
    // -------------------------------------------------------------------------
    // setTransposedMatrix
    // -------------------------------------------------------------------------
    static void  setTransposedMatrix(MatrixType outDst[], const MatrixType inSrc[])
    {
      const int *index = getTransposedMatrixIndex();
      for (int i = 0; i < MATRIX_SIZE; i++)
        outDst[i] = inSrc[index[i]];
    }
    // -------------------------------------------------------------------------
    // setZero
    // -------------------------------------------------------------------------
    static void  setZero(MatrixType outDst[])
    {
      for (int i = 0; i < MATRIX_SIZE; i++)
        outDst[i] = 0.0;
    }
    // -------------------------------------------------------------------------
    // add
    // -------------------------------------------------------------------------
    static void  add(MatrixType outDst[], const MatrixType inSrc0[], const MatrixType inSrc1[])
    {
      for (int i = 0; i < MATRIX_SIZE; i++)
        outDst[i] = inSrc0[i] + inSrc1[i];
    }
    // -------------------------------------------------------------------------
    // add
    // -------------------------------------------------------------------------
    static void  add(MatrixType outDst[], const MatrixType inSrc[], MatrixType inOffset)
    {
      for (int i = 0; i < MATRIX_SIZE; i++)
        outDst[i] = inSrc[i] + inOffset;
    }
    // -------------------------------------------------------------------------
    // sub
    // -------------------------------------------------------------------------
    static void  sub(MatrixType outDst[], const MatrixType inSrc0[], const MatrixType inSrc1[])
    {
      for (int i = 0; i < MATRIX_SIZE; i++)
        outDst[i] = inSrc0[i] - inSrc1[i];
    }
    // -------------------------------------------------------------------------
    // sub
    // -------------------------------------------------------------------------
    static void  sub(MatrixType outDst[], const MatrixType inSrc[], MatrixType inOffset)
    {
      for (int i = 0; i < MATRIX_SIZE; i++)
        outDst[i] = inSrc[i] - inOffset;
    }
    // -------------------------------------------------------------------------
    // multi
    // -------------------------------------------------------------------------
    static void  multi(MatrixType outDst[], const MatrixType inSrc0[], const MatrixType inSrc1[])
    {
      for (int i = 0; i < MATRIX_ROW_SIZE; i++)
        for (int j = 0; j < MATRIX_COL_SIZE; j++)
        {
          double v = 0;
          for (int k = 0; k < MATRIX_ROW_SIZE; k++)
            v += inSrc0[i * MATRIX_COL_SIZE + k] * inSrc1[k * MATRIX_COL_SIZE + j];
          outDst[i * MATRIX_COL_SIZE + j] = v;
        }
    }
    // -------------------------------------------------------------------------
    // scale
    // -------------------------------------------------------------------------
    static void scale(MatrixType ioMat[], MatrixType inScale)
    {
      for (int i = 0; i < MATRIX_SIZE; i++)
        ioMat[i] *= inScale;
    }
    // -------------------------------------------------------------------------
    // div
    // -------------------------------------------------------------------------
    static void div(MatrixType ioMat[], MatrixType inDiv)
    {
      for (int i = 0; i < MATRIX_SIZE; i++)
        ioMat[i] /= inDiv;
    }
    // -------------------------------------------------------------------------
    // getTransposedMatrixIndex
    // -------------------------------------------------------------------------
    static const int  *getTransposedMatrixIndex()
    {
      const int index[] = {
        0, 4, 8,  12,
        1, 5, 9,  13,
        2, 6, 10, 14,
        3, 7, 11, 15
      };
      return index;
    }
  };

  // Operator overloading (for other types) ------------------------------------
  // ex) a = 2 + b
  template <typename T0, typename T1> MatrixBase<T1> operator+(T0 inOffset, const MatrixBase<T1> &inMatrix)
  {
    return inMatrix + (T1 )inOffset;
  }
  // ex) a = 2 - b (this might be misleading...)
  template <typename T0, typename T1> MatrixBase<T1> operator-(T0 inOffset, const MatrixBase<T1> &inMatrix)
  {
    return -1 * inMatrix + (T1 )inOffset;
  }
  // ex) a = 2 * b
  template <typename T0, typename T1> MatrixBase<T1> operator*(T0 inScale, const MatrixBase<T1> &inMatrix)
  {
    return inMatrix * (T1 )inScale;
  }
  template <typename T0> std::ostream& operator<<(std::ostream &os, const MatrixBase<T0> &inMatrix)
  {
    os << "{";
    for (int i = 0; i < MatrixBase<T0>::MATRIX_ROW_SIZE; i++)
    {
        os << "{";
        for (int j = 0; j < MatrixBase<T0>::MATRIX_COL_SIZE; j++)
        {
          os << inMatrix.mMat[i][j];
          if (j != MatrixBase<T0>::MATRIX_COL_SIZE -1)
            os << ", ";
        }
        if (i != MatrixBase<T0>::MATRIX_ROW_SIZE -1)
          os << "}, " << std::endl << " ";
        else
          os << "}}" << std::endl;
    }
    return os;
  }

  // ---------------------------------------------------------------------------
  // Typedefs
  // ---------------------------------------------------------------------------
  typedef MatrixBase<double>  Matrix;
  typedef MatrixBase<float>  MatrixF;
  typedef MatrixBase<int>  MatrixI;
 };
};

#endif  // #ifdef IBC_MATRIX_H_

