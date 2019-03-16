// =============================================================================
//  utils.h
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
  \file     ibc/gl/utils.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/10
  \brief    Header file for handling the image

  This file defines the quaternion class for the IBC Library
*/

#ifndef IBC_UTILS_H_
#define IBC_UTILS_H_

// Includes --------------------------------------------------------------------
#include <math.h>
#include <GL/gl.h>
#include "ibc/gl/vector.h"

// Macros ----------------------------------------------------------------------
#define GL_UTILS_PI           3.141524

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gl
 {
  // ---------------------------------------------------------------------------
  // Utils class
  // ---------------------------------------------------------------------------
  class Utils
  {
  public:
    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // perspective
    // -------------------------------------------------------------------------
    static void  perspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
    {
      GLdouble fW, fH;

      fH = tan(fovY / 360.0 * GL_UTILS_PI) * zNear;
      fW = fH * aspect;
      glFrustum(-fW, fW, -fH, fH, zNear, zFar);
    }
    // -------------------------------------------------------------------------
    // lookAt
    // -------------------------------------------------------------------------
    static void lookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
                      GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz)
    {
      Vector  forward(eyex, eyey, eyez);
      Vector  center(centerx, centery, centerz);
      Vector  up(upx, upy, upz);
      Vector  side;

      forward = center - forward;
      forward.normalize();
      side = forward * up;
      side.normalize();
      up = side * forward;

      GLdouble m[] =
      {
        side[0], up[0], -forward[0], 0,
        side[1], up[1], -forward[1], 0,
        side[2], up[2], -forward[2], 0,
        0, 0, 0, 1
      };

      glMultMatrixd(m);
      glTranslated(-eyex, -eyey, -eyez);
    }
  };
 };
};

#endif  // #ifdef IBC_UTILS_H_

