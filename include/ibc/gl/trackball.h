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
  \brief    Header file for handling the image

  This file defines the quaternion class for the IBC Library
*/

#ifndef IBC_TRACKBALL_H_
#define IBC_TRACKBALL_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/quaternion.h"
#include "ibc/gl/utils.h"

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
    // Member variables (public) -----------------------------------------------
    TrackballType   mGLRotationMatrix[4][4];

    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // TrackballBase
    // -------------------------------------------------------------------------
    TrackballBase()
    {
      mQuat.setIdentity();
      mQuat.get_glRotationMatrix(mGLRotationMatrix);
      mIsMouseButtonDown = false;
    }
    // -------------------------------------------------------------------------
    // ~TrackballBase
    // -------------------------------------------------------------------------
    virtual ~TrackballBase()
    {
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // startTrackingMouse
    // -------------------------------------------------------------------------
    // call this function at onMouseBottunDown event
    //
    void startTrackingMouse(int inMouseX, int  inMouseY,
                            int inClientWidth, int inClientHeight)
    {
      mIsMouseButtonDown = true;
      mPrevMouseX = inMouseX;
      mPrevMouseY = inMouseY;
      mClientWidth = inClientWidth;
      mClientHeight = inClientHeight;
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
    // setRotationByMouse
    // -------------------------------------------------------------------------
    //
    void setRotationByMouse(int inMouseX, int  inMouseY)
    {
      if (mIsMouseButtonDown == false)
        return;

      QuaternionBase<TrackballType> quat;
      quat = getRotation(mPrevMouseX, mPrevMouseY, inMouseX, inMouseY,
                         mClientWidth, mClientHeight);
      mQuat *= quat;
    //mQuat.normalize();
      mQuat.get_glRotationMatrix(mGLRotationMatrix);
      mPrevMouseX = inMouseX;
      mPrevMouseY = inMouseY;
    }

    // Static Functions --------------------------------------------------------
    static  QuaternionBase<TrackballType>  getRotation(
                          int inX0, int inY0, int inX1, int inY1,
                          int inWidth, int inHeight)
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
      axis = v0 * v1;
      diff = v1 - v0;
      angle = ((-GL_UTILS_PI / 2.0) * diff.length()); // <- TODO
      quat.setAngle(angle, axis);

      return quat;
    }
    // -------------------------------------------------------------------------
    // pointToVec
    // -------------------------------------------------------------------------
    static VectorBase<TrackballType>  pointToVec(int inX, int inY,
                                                int inWidth, int inHeight)
    {
      VectorBase<TrackballType> vec;
      TrackballType d;

      vec[0] = (TrackballType )((2.0 * inX - inWidth) / inWidth);
      vec[1] = (TrackballType )((inHeight - 2.0 * inY) / inHeight);
      vec[2] = 0.0;
      d = vec.length();
      if (d > 1.0)
        d = 1.0;
      vec[2] = (TrackballType )::cos((GL_UTILS_PI / 2.0) * d);
      vec.normalize();
      return vec;
    }

  protected:
    // Member variables (protected) --------------------------------------------
    QuaternionBase<TrackballType> mQuat;
    bool  mIsMouseButtonDown;
    int mPrevMouseX, mPrevMouseY, mClientWidth, mClientHeight;
  };

  // ---------------------------------------------------------------------------
  // Typedefs
  // ---------------------------------------------------------------------------
  typedef TrackballBase<double>  Trackball;
  typedef TrackballBase<float>  TrackballF;
 };
};

#endif  // #ifdef IBC_TRACKBALL_H_

