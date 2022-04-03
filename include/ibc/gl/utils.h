// =============================================================================
//  utils.h
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
  \file     ibc/gl/utils.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/10
  \brief    Header file for OpenGL related utility routines
*/

#ifndef IBC_UTILS_H_
#define IBC_UTILS_H_

// Includes --------------------------------------------------------------------
#include <math.h>
#include "ibc/gl/matrix.h"
#include "ibc/image/image.h"

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
    template <typename UtilsType> static MatrixBase<UtilsType> perspective(UtilsType inFovY, UtilsType inAspect,
                                         UtilsType inNear, UtilsType inFar)
    {
      UtilsType width, height;

      height = tan(inFovY / 360.0 * M_PI) * inNear;
      width = height * inAspect;
      return frustum<UtilsType>(-width, width, -height, height, inNear, inFar);
    }
    // -------------------------------------------------------------------------
    // frustum
    // -------------------------------------------------------------------------
    template <typename UtilsType> static MatrixBase<UtilsType> frustum(UtilsType inLeft, UtilsType inRight,
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
    // -------------------------------------------------------------------------
    // sizeofGLDataType
    // -------------------------------------------------------------------------
    static size_t sizeofGLDataType(GLenum inType)
    {
      switch (inType)
      {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
          return 1;
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
          return 2;
        case GL_INT:
        case GL_UNSIGNED_INT:
          return 4;
        case GL_HALF_FLOAT:
          return 2;
        case GL_FLOAT:
          return 4;
        case GL_FIXED:
          return 4;
        case GL_DOUBLE:
          return 8;
        default:
          return 0;
      }
      return 0;
    }
    // -------------------------------------------------------------------------
    // toGLDataType
    // -------------------------------------------------------------------------
    static GLenum toGLDataType(ibc::image::ImageType::DataType inType)
    {
      switch (inType)
      {
        case ibc::image::ImageType::DATA_TYPE_8BIT:
          return GL_UNSIGNED_BYTE;
        case ibc::image::ImageType::DATA_TYPE_16BIT:
          return GL_UNSIGNED_SHORT;
        case ibc::image::ImageType::DATA_TYPE_32BIT:
          return GL_UNSIGNED_INT;
        case ibc::image::ImageType::DATA_TYPE_FLOAT:
          return GL_FLOAT;
        case ibc::image::ImageType::DATA_TYPE_DOUBLE:
          return GL_DOUBLE;
        default:
          break;
      }
      return GL_NONE;
    }
  };
 };
};

#endif  // #ifdef IBC_UTILS_H_

