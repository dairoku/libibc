// =============================================================================
//  data.h
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
  \file     ibc/gl/models/data.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/05/03
  \brief    Header file for defining OpenGL related types
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
