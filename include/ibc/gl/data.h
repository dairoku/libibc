// =============================================================================
//  data.h
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
  \file     ibc/gl/models/data.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/05/03
  \brief    Header file for ModelViewBase widget

  This file defines the class for the image widget
*/

#ifndef IBC_GL_DATA_H_
#define IBC_GL_DATA_H_

// Includes --------------------------------------------------------------------
#include <vector>
#include "ibc/base/types.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gl
 {
  // ---------------------------------------------------------------------------
  // Typedefs (structs)
  // ---------------------------------------------------------------------------
  typedef struct
  {
    GLfloat x;
    GLfloat y;
    GLfloat z;
  } glXYZf;
  typedef struct
  {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    //
    GLfloat r;
    GLfloat g;
    GLfloat b;
  } glXYZ_RGBf;
  typedef struct
  {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    //
    GLfloat i;
  } glXYZ_If;
  typedef struct
  {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    //
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
    //
    GLfloat r;
    GLfloat g;
    GLfloat b;
  } glXYZ_NORM_RGBf;
  typedef struct
  {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    //
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
    //
    GLfloat i;
  } glXYZ_NORM_If;
  //
  typedef struct
  {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    //
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
  } glXYZf_RGBAub;
  typedef struct
  {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    //
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
    //
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
  } glXYZ_NORMf_RGBAub;
  typedef struct
  {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    //
    GLubyte i;
  } glXYZf_Iub;
  typedef struct
  {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    //
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
    //
    GLubyte i;
  } glXYZ_NORMf_Iub;
  // ---------------------------------------------------------------------------
  // Typedefs (classes)
  // ---------------------------------------------------------------------------
  typedef std::vector<glXYZf>             glDataXYZf;
  typedef std::vector<glXYZ_RGBf>         glDataXYZ_RGBf;
  typedef std::vector<glXYZf_RGBAub>      glDataXYZf_RGBAub;
  typedef std::vector<glXYZ_If>           glDataXYZ_If;
  typedef std::vector<glXYZf_Iub>         glDataXYZf_Iub;
  typedef std::vector<glXYZ_NORM_RGBf>    glDataXYZ_NORM_RGBf;
  typedef std::vector<glXYZ_NORMf_RGBAub> glDataXYZ_NORMf_RGBub;
  typedef std::vector<glXYZ_NORM_If>      glDataXYZ_NORM_If;
  typedef std::vector<glXYZ_NORMf_Iub>    glDataXYZ_NORMf_Iub;
 };
};

#endif  // #ifdef IBC_GL_DATA_H_
