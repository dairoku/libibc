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
#include "ibc/gl/matrix.h"

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
  template <typename UtilsType> class Utils
  {
  public:
    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // perspective
    // -------------------------------------------------------------------------
    static MatrixBase<UtilsType> perspective(UtilsType inFovY, UtilsType inAspect,
                                         UtilsType inNear, UtilsType inFar)
    {
      UtilsType width, height;

      height = tan(inFovY / 360.0 * GL_UTILS_PI) * inNear;
      width = height * inAspect;
      return frustum(-width, width, -height, height, inNear, inFar);
    }
    // -------------------------------------------------------------------------
    // frustum
    // -------------------------------------------------------------------------
    static MatrixBase<UtilsType> frustum(UtilsType inLeft, UtilsType inRight,
                                      UtilsType inTop, UtilsType inBottom,
                                      UtilsType inNear, UtilsType inFar)
    {
      MatrixBase<UtilsType> mat;

      UtilsType   width  = inRight - inLeft;
      UtilsType   height = inBottom - inTop;
      UtilsType   depth  = inFar - inNear;

      mat[0][0] = 2.0 * inNear / width;
      mat[0][1] = 0.0;
      mat[0][2] = (inRight + inLeft) / width;
      mat[0][3] = 0.0;

      mat[1][0] = 0.0;
      mat[1][1] = 2.0 * inNear / height;
      mat[1][2] = (inTop + inBottom) / height;
      mat[1][3] = 0.0;

      mat[2][0] = 0.0;
      mat[2][1] = 0.0;
      mat[2][2] = -1.0 * (inFar + inNear) / depth;
      mat[2][3] = -2.0 * inFar * inNear / depth;

      mat[3][0] = 0.0;
      mat[3][1] = 0.0;
      mat[3][2] = -1.0;
      mat[3][3] = 0.0;

      return mat;
    }
  };
 };
};

#endif  // #ifdef IBC_UTILS_H_

