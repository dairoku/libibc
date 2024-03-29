// =============================================================================
//  matrix.h
//
//  Written in 2019 by Dairoku Sekiguchi (sekiguchi at acm dot org)
//
//  To the extent possible under law, the author(s) have dedicated all copyright
//  and related and neighboring rights to this software to the public domain worldwide.
//  This software is distributed without any warranty.
//
//  You should have received a copy of the CC0 Public Domain Dedication along with
//  this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// =============================================================================
/*!
  \file     ibc/gl/vector.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/02/24
  \brief    Header file for the Matrix (4x4) class
*/

#ifndef IBC_MATRIX_H_
#define IBC_MATRIX_H_

// Includes --------------------------------------------------------------------
#include <iostream>
#include "ibc/gl/vector.h"

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
    MatrixBase<MatrixType> &operator=(const MatrixBase<MatrixType> &inMatrix)
    {
      return set(inMatrix);
    }
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
    // []
    // -------------------------------------------------------------------------
    const MatrixType *operator[](size_t inRow) const
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
    MatrixBase<MatrixType> operator+(const VectorBase<MatrixType> &inVector)
    {
      MatrixBase<MatrixType> matrix(*this);
      return matrix.translate(inVector);
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
    MatrixBase<MatrixType> &operator+=(const VectorBase<MatrixType> &inVector)
    {
      return translate(inVector);
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
    VectorBase<MatrixType> operator*(const VectorBase<MatrixType> &inVector)
    {
      return multiByVector(inVector);
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
    // setIdentity
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &setIdentity()
    {
      MatrixBase<MatrixType>::setIdentity((MatrixType *)mMat);
      return *this;
    }
    // -------------------------------------------------------------------------
    // setScaleFactor
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &setScaleFactor(MatrixType inX, MatrixType inY, MatrixType inZ)
    {
      MatrixBase<MatrixType>::setScaleFactor((MatrixType *)mMat, inX, inY, inZ);
      return *this;
    }
    // -------------------------------------------------------------------------
    // setScaleFactor
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &setScaleFactor(const VectorBase<MatrixType> &inVector)
    {
      MatrixBase<MatrixType>::setScaleFactor((MatrixType *)mMat, (MatrixType *)inVector.mVec);
      return *this;
    }
    // -------------------------------------------------------------------------
    // setScaleMatrix
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &setScaleMatrix(MatrixType inX, MatrixType inY, MatrixType inZ)
    {
      setIdentity();
      MatrixBase<MatrixType>::setScaleFactor((MatrixType *)mMat, inX, inY, inZ);
      return *this;
    }
    // -------------------------------------------------------------------------
    // setScaleMatrix
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &setScaleMatrix(const VectorBase<MatrixType> &inVector)
    {
      setIdentity();
      MatrixBase<MatrixType>::setScaleFactor((MatrixType *)mMat, (MatrixType *)inVector.mVec);
      return *this;
    }
    // -------------------------------------------------------------------------
    // setTranslation
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &setTranslation(MatrixType inX, MatrixType inY, MatrixType inZ)
    {
      MatrixBase<MatrixType>::setTranslation((MatrixType *)mMat, inX, inY, inZ);
      return *this;
    }
    // -------------------------------------------------------------------------
    // setTranslation
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &setTranslation(const VectorBase<MatrixType> &inVector)
    {
      MatrixBase<MatrixType>::setTranslation((MatrixType *)mMat, (MatrixType *)inVector.mVec);
      return *this;
    }
    // -------------------------------------------------------------------------
    // setTranslationMatrix
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &setTranslationMatrix(MatrixType inX, MatrixType inY, MatrixType inZ)
    {
      setIdentity();
      MatrixBase<MatrixType>::setTranslation((MatrixType *)mMat, inX, inY, inZ);
      return *this;
    }
    // -------------------------------------------------------------------------
    // setTranslationMatrix
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &setTranslationMatrix(const VectorBase<MatrixType> &inVector)
    {
      setIdentity();
      MatrixBase<MatrixType>::setTranslation((MatrixType *)mMat, (MatrixType *)inVector.mVec);
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
    // multiByVector
    // -------------------------------------------------------------------------
    VectorBase<MatrixType> multiByVector(const VectorBase<MatrixType> &inVector)
    {
      VectorBase<MatrixType> r;
      MatrixBase<MatrixType>::multiMatrixByVector(
        (MatrixType *)r.mVec, (MatrixType *)mMat, (MatrixType *)inVector.mVec);
      return r;
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
    // -------------------------------------------------------------------------
    // translate
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &translate(MatrixType inX, MatrixType inY, MatrixType inZ)
    {
      MatrixBase<MatrixType>::translate((MatrixType *)mMat, inX, inY, inZ);
      return *this;
    }
    // -------------------------------------------------------------------------
    // translate
    // -------------------------------------------------------------------------
    MatrixBase<MatrixType> &translate(const VectorBase<MatrixType> &inVector)
    {
      MatrixBase<MatrixType>::translate((MatrixType *)mMat, (MatrixType *)inVector.mVec);
      return *this;
    }
    // -------------------------------------------------------------------------
    // inverse
    // -------------------------------------------------------------------------
    bool inverse()
    {
      return MatrixBase<MatrixType>::inverse((MatrixType *)mMat, (MatrixType *)mMat);
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
    // setIdentity
    // -------------------------------------------------------------------------
    static void  setIdentity(MatrixType outDst[])
    {
      setZero(outDst);
      for (int i = 0; i < MATRIX_ROW_SIZE; i++)
        outDst[i*MATRIX_ROW_SIZE+i] = 1.0;
    }
    // -------------------------------------------------------------------------
    // setScaleFactor
    // -------------------------------------------------------------------------
    static void  setScaleFactor(MatrixType ioMat[], MatrixType inX, MatrixType inY, MatrixType inZ)
    {
      ioMat[0]  = inX;
      ioMat[5]  = inY;
      ioMat[10] = inZ;
    }
    // -------------------------------------------------------------------------
    // setScaleFactor
    // -------------------------------------------------------------------------
    static void setScaleFactor(MatrixType ioMat[], const MatrixType inVec[])
    {
      setTranslation(inVec[0], inVec[1], inVec[2]);
    }
    // -------------------------------------------------------------------------
    // setTranslation
    // -------------------------------------------------------------------------
    static void  setTranslation(MatrixType ioMat[], MatrixType inX, MatrixType inY, MatrixType inZ)
    {
      ioMat[3]  = inX;
      ioMat[7]  = inY;
      ioMat[11] = inZ;
    }
    // -------------------------------------------------------------------------
    // setTranslation
    // -------------------------------------------------------------------------
    static void setTranslation(MatrixType ioMat[], const MatrixType inVec[])
    {
      setTranslation(ioMat, inVec[0], inVec[1], inVec[2]);
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
    // multiMatrixByVector
    // -------------------------------------------------------------------------
    static void multiMatrixByVector(MatrixType outVec[], const MatrixType inMat[], const MatrixType inVec[])
    {
      outVec[0] = inMat[0] * inVec[0] + inMat[1] * inVec[1] + inMat[2]  * inVec[2] + inMat[3];
      outVec[1] = inMat[4] * inVec[0] + inMat[5] * inVec[1] + inMat[6]  * inVec[2] + inMat[7];
      outVec[2] = inMat[8] * inVec[0] + inMat[9] * inVec[1] + inMat[10] * inVec[2] + inMat[11];
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
    // translate
    // -------------------------------------------------------------------------
    static void translate(MatrixType ioMat[], MatrixType inX, MatrixType inY, MatrixType inZ)
    {
      ioMat[3]  += inX;
      ioMat[7]  += inY;
      ioMat[11] += inZ;
    }
    // -------------------------------------------------------------------------
    // translate
    // -------------------------------------------------------------------------
    static void translate(MatrixType ioMat[], const MatrixType inVec[])
    {
      translate(inVec[0], inVec[1], inVec[2]);
    }
    // -------------------------------------------------------------------------
    // getTransposedMatrixIndex
    // -------------------------------------------------------------------------
    static const int  *getTransposedMatrixIndex()
    {
      static const int index[] = {
        0, 4, 8,  12,
        1, 5, 9,  13,
        2, 6, 10, 14,
        3, 7, 11, 15
      };
      return index;
    }
    // -------------------------------------------------------------------------
    // inverse
    // -------------------------------------------------------------------------
    // The following code is based on the old gluInvertMatrix implementation of Mesa
    //
    static bool inverse(MatrixType outDst[], const MatrixType inSrc[])
    {
      MatrixType  t[16], v;

      t[0] =  inSrc[5]  * inSrc[10] * inSrc[15] -
              inSrc[5]  * inSrc[11] * inSrc[14] -
              inSrc[9]  * inSrc[6]  * inSrc[15] +
              inSrc[9]  * inSrc[7]  * inSrc[14] +
              inSrc[13] * inSrc[6]  * inSrc[11] -
              inSrc[13] * inSrc[7]  * inSrc[10];
      t[1] = -inSrc[1]  * inSrc[10] * inSrc[15] +
              inSrc[1]  * inSrc[11] * inSrc[14] +
              inSrc[9]  * inSrc[2]  * inSrc[15] -
              inSrc[9]  * inSrc[3]  * inSrc[14] -
              inSrc[13] * inSrc[2]  * inSrc[11] +
              inSrc[13] * inSrc[3]  * inSrc[10];
      t[2] =  inSrc[1]  * inSrc[6]  * inSrc[15] -
              inSrc[1]  * inSrc[7]  * inSrc[14] -
              inSrc[5]  * inSrc[2]  * inSrc[15] +
              inSrc[5]  * inSrc[3]  * inSrc[14] +
              inSrc[13] * inSrc[2]  * inSrc[7] -
              inSrc[13] * inSrc[3]  * inSrc[6];
      t[3] = -inSrc[1]  * inSrc[6]  * inSrc[11] +
              inSrc[1]  * inSrc[7]  * inSrc[10] +
              inSrc[5]  * inSrc[2]  * inSrc[11] -
              inSrc[5]  * inSrc[3]  * inSrc[10] -
              inSrc[9]  * inSrc[2]  * inSrc[7] +
              inSrc[9]  * inSrc[3]  * inSrc[6];
      //
      t[4] = -inSrc[4]  * inSrc[10] * inSrc[15] +
              inSrc[4]  * inSrc[11] * inSrc[14] +
              inSrc[8]  * inSrc[6]  * inSrc[15] -
              inSrc[8]  * inSrc[7]  * inSrc[14] -
              inSrc[12] * inSrc[6]  * inSrc[11] +
              inSrc[12] * inSrc[7]  * inSrc[10];
      t[5] =  inSrc[0]  * inSrc[10] * inSrc[15] -
              inSrc[0]  * inSrc[11] * inSrc[14] -
              inSrc[8]  * inSrc[2]  * inSrc[15] +
              inSrc[8]  * inSrc[3]  * inSrc[14] +
              inSrc[12] * inSrc[2]  * inSrc[11] -
              inSrc[12] * inSrc[3]  * inSrc[10];
      t[6] = -inSrc[0]  * inSrc[6]  * inSrc[15] +
              inSrc[0]  * inSrc[7]  * inSrc[14] +
              inSrc[4]  * inSrc[2]  * inSrc[15] -
              inSrc[4]  * inSrc[3]  * inSrc[14] -
              inSrc[12] * inSrc[2]  * inSrc[7] +
              inSrc[12] * inSrc[3]  * inSrc[6];
      t[7] =  inSrc[0]  * inSrc[6]  * inSrc[11] -
              inSrc[0]  * inSrc[7]  * inSrc[10] -
              inSrc[4]  * inSrc[2]  * inSrc[11] +
              inSrc[4]  * inSrc[3]  * inSrc[10] +
              inSrc[8]  * inSrc[2]  * inSrc[7] -
              inSrc[8]  * inSrc[3]  * inSrc[6];
      //
      t[8] =  inSrc[4]  * inSrc[9]  * inSrc[15] -
              inSrc[4]  * inSrc[11] * inSrc[13] -
              inSrc[8]  * inSrc[5]  * inSrc[15] +
              inSrc[8]  * inSrc[7]  * inSrc[13] +
              inSrc[12] * inSrc[5]  * inSrc[11] -
              inSrc[12] * inSrc[7]  * inSrc[9];
      t[9] = -inSrc[0]  * inSrc[9]  * inSrc[15] +
              inSrc[0]  * inSrc[11] * inSrc[13] +
              inSrc[8]  * inSrc[1]  * inSrc[15] -
              inSrc[8]  * inSrc[3]  * inSrc[13] -
              inSrc[12] * inSrc[1]  * inSrc[11] +
              inSrc[12] * inSrc[3]  * inSrc[9];
      t[10] = inSrc[0]  * inSrc[5]  * inSrc[15] -
              inSrc[0]  * inSrc[7]  * inSrc[13] -
              inSrc[4]  * inSrc[1]  * inSrc[15] +
              inSrc[4]  * inSrc[3]  * inSrc[13] +
              inSrc[12] * inSrc[1]  * inSrc[7] -
              inSrc[12] * inSrc[3]  * inSrc[5];
      t[11] =-inSrc[0]  * inSrc[5]  * inSrc[11] +
              inSrc[0]  * inSrc[7]  * inSrc[9] +
              inSrc[4]  * inSrc[1]  * inSrc[11] -
              inSrc[4]  * inSrc[3]  * inSrc[9] -
              inSrc[8]  * inSrc[1]  * inSrc[7] +
              inSrc[8]  * inSrc[3]  * inSrc[5];
      //
      t[12] =-inSrc[4]  * inSrc[9]  * inSrc[14] +
              inSrc[4]  * inSrc[10] * inSrc[13] +
              inSrc[8]  * inSrc[5]  * inSrc[14] -
              inSrc[8]  * inSrc[6]  * inSrc[13] -
              inSrc[12] * inSrc[5]  * inSrc[10] +
              inSrc[12] * inSrc[6]  * inSrc[9];
      t[13] = inSrc[0]  * inSrc[9]  * inSrc[14] -
              inSrc[0]  * inSrc[10] * inSrc[13] -
              inSrc[8]  * inSrc[1]  * inSrc[14] +
              inSrc[8]  * inSrc[2]  * inSrc[13] +
              inSrc[12] * inSrc[1]  * inSrc[10] -
              inSrc[12] * inSrc[2]  * inSrc[9];
      t[14] =-inSrc[0]  * inSrc[5]  * inSrc[14] +
              inSrc[0]  * inSrc[6]  * inSrc[13] +
              inSrc[4]  * inSrc[1]  * inSrc[14] -
              inSrc[4]  * inSrc[2]  * inSrc[13] -
              inSrc[12] * inSrc[1]  * inSrc[6] +
              inSrc[12] * inSrc[2]  * inSrc[5];
      t[15] = inSrc[0]  * inSrc[5]  * inSrc[10] -
              inSrc[0]  * inSrc[6]  * inSrc[9] -
              inSrc[4]  * inSrc[1]  * inSrc[10] +
              inSrc[4]  * inSrc[2]  * inSrc[9] +
              inSrc[8]  * inSrc[1]  * inSrc[6] -
              inSrc[8]  * inSrc[2]  * inSrc[5];

      v = inSrc[0] * t[0] + inSrc[1] * t[4] + inSrc[2] * t[8] + inSrc[3] * t[12];
      if (v == 0)
          return false;
      v = 1.0 / v;

      for (int i = 0; i < 16; i++)
          outDst[i] = t[i] * v;
      return true;
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
  //
  typedef MatrixBase<GLdouble>  glMatrix4d;
  typedef MatrixBase<GLfloat>   glMatrix4f;
  typedef MatrixBase<GLint>     glMatrix4i;
 };
};

#endif  // #ifdef IBC_MATRIX_H_
