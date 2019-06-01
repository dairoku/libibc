// =============================================================================
//  trackball.h
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
  \file     ibc/gl/trackball.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/10
  \brief    Header file for trackball view interface
*/

#ifndef IBC_TRACKBALL_H_
#define IBC_TRACKBALL_H_

// Includes --------------------------------------------------------------------
#include <math.h>
#include "ibc/gl/quaternion.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gl
 {
  // ---------------------------------------------------------------------------
  // TrackballBase class
  // ---------------------------------------------------------------------------
  template <typename TrackballType> class  TrackballBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // TrackballBase
    // -------------------------------------------------------------------------
    TrackballBase(TrackballType inRotationSensitivity = 1.0)
    {
      mQuat.setIdentity();
      mIsMouseButtonDown = false;
      mRotationSensitivity = inRotationSensitivity;

      mScaleFactor = 1.0;
      mModelView.setIdentity();
      mOffset.setZero();
    }
    // -------------------------------------------------------------------------
    // ~TrackballBase
    // -------------------------------------------------------------------------
    virtual ~TrackballBase()
    {
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // setRotationSensitivity
    // -------------------------------------------------------------------------
    void setRotationSensitivity(TrackballType inRotationSensitivity)
    {
      mRotationSensitivity = inRotationSensitivity;
    }
    // -------------------------------------------------------------------------
    // getRotationSensitivity
    // -------------------------------------------------------------------------
    TrackballType getRotationSensitivity()
    {
      return mRotationSensitivity;
    }
    // -------------------------------------------------------------------------
    // startTrackingMouse
    // -------------------------------------------------------------------------
    // call this function at onMouseBottunDown event
    //
    void startTrackingMouse(TrackballType inMouseX, TrackballType  inMouseY,
                            TrackballType inClientWidth, TrackballType inClientHeight,
                            unsigned int inMouseButton = 1)
    {
      mIsMouseButtonDown = true;
      mPrevMouseX = inMouseX;
      mPrevMouseY = inMouseY;
      mClientWidth = inClientWidth;
      mClientHeight = inClientHeight;
      mMouseButton = inMouseButton;
      //
      mQuat.normalize();  // A bit tricky to call here
    }
    // -------------------------------------------------------------------------
    // stopTrackingMouse
    // -------------------------------------------------------------------------
    // call this function at onMouseBottunUp event
    //
    void stopTrackingMouse()
    {
      mIsMouseButtonDown = false;
    }
    // -------------------------------------------------------------------------
    // trackMouse
    // -------------------------------------------------------------------------
    //
    void  trackMouse(TrackballType inMouseX, TrackballType  inMouseY)
    {
      if (mIsMouseButtonDown == false)
        return;

      if (mMouseButton == 1)
      {
        QuaternionBase<TrackballType> quat;
        quat = getRotation(mPrevMouseX, mPrevMouseY, inMouseX, inMouseY,
                           mClientWidth, mClientHeight, mRotationSensitivity);
        mQuat *= quat;
        mQuat.normalize();
        mModelView = mQuat.rotationMatrix();
      }
      else
      {
        ibc::gl::VectorBase<TrackballType> offset;
        ibc::gl::MatrixBase<TrackballType> mat = mModelView;
        mat.inverse();
        offset[0] = -2.0 * (mPrevMouseX - inMouseX) / mClientHeight / mScaleFactor;
        offset[1] =  2.8 * (mPrevMouseY - inMouseY) / mClientHeight / mScaleFactor;
        offset[2] = 0.0;
        offset = mat * offset;
        mOffset += offset;
      }
      mPrevMouseX = inMouseX;
      mPrevMouseY = inMouseY;
    }
    // -------------------------------------------------------------------------
    // mouseWheel
    // -------------------------------------------------------------------------
    //
    void mouseWheel(int inDirection, TrackballType inDeltaX, TrackballType inDeltaY)
    {
      UNUSED(inDeltaX);
      UNUSED(inDeltaY);
      //
      if (inDirection == 0)
        mScaleFactor /= 1.1;
      else
        mScaleFactor *= 1.1;
    }
    // -------------------------------------------------------------------------
    // getGLRotationMatrix
    // -------------------------------------------------------------------------
    //
    void getGLRotationMatrix(TrackballType outMat[16])
    {
      ibc::gl::VectorBase<TrackballType>  vec = mModelView * mOffset;
      ibc::gl::MatrixBase<TrackballType>  mat = mModelView;
      ibc::gl::MatrixBase<TrackballType>  scale;

      scale.setScaleMatrix(mScaleFactor, mScaleFactor, mScaleFactor);
      mat.setTranslation(vec);
      mat = scale * mat;
      mat.getTransposedMatrix(outMat);
    }

    // Static Functions --------------------------------------------------------
    static  QuaternionBase<TrackballType>  getRotation(
                          TrackballType inX0, TrackballType inY0,
                          TrackballType inX1, TrackballType inY1,
                          TrackballType inWidth, TrackballType inHeight,
                          TrackballType inSensitivity)
    {
      QuaternionBase<TrackballType> quat;
      VectorBase<TrackballType> v0, v1, diff, axis;
      TrackballType angle;

      if (inX0 == inX1 && inY0 == inY1)
      {
        quat.setIdentity();
        return quat;
      }

      v0 = pointToVec(inX0, inY0, inWidth, inHeight);
      v1 = pointToVec(inX1, inY1, inWidth, inHeight);
      axis = v1 * v0;
      diff = v1 - v0;
      //angle = ((M_PI / 2.0) * diff.length()) * inSensitivity / 1.5;   // This approximation works fairly well
      angle = inSensitivity * v0.angle(v1);
      quat.setAngle(angle, axis);

      return quat;
    }
    // -------------------------------------------------------------------------
    // pointToVec
    // -------------------------------------------------------------------------
    static VectorBase<TrackballType>  pointToVec(TrackballType inX, TrackballType inY,
                                                  TrackballType inWidth, TrackballType inHeight)
    {
      VectorBase<TrackballType> vec;
      TrackballType d;

      vec[0] = (2.0 * inX - inWidth) / inWidth;
      vec[1] = (inHeight - 2.0 * inY) / inHeight;
      vec[2] = 0.0;
      d = vec.length();
      if (d > 1.0)
        d = 1.0;
      vec[2] = (TrackballType )::cos((M_PI / 2.0) * d);
      vec.normalize();
      return vec;
    }

  protected:
    // Member variables (protected) --------------------------------------------
    TrackballType mRotationSensitivity;

    QuaternionBase<TrackballType> mQuat;
    bool  mIsMouseButtonDown;
    TrackballType mPrevMouseX, mPrevMouseY, mClientWidth, mClientHeight;
    unsigned int  mMouseButton;

    TrackballType mScaleFactor;
    ibc::gl::VectorBase<TrackballType> mOffset;
    ibc::gl::MatrixBase<TrackballType> mModelView;
  };

  // ---------------------------------------------------------------------------
  // Typedefs
  // ---------------------------------------------------------------------------
  typedef TrackballBase<double>  Trackball;
  typedef TrackballBase<float>  TrackballF;
 };
};

#endif  // #ifdef IBC_TRACKBALL_H_

