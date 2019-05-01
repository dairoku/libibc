// =============================================================================
//  point_sprite.h
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
  \file     ibc/gl/models/point_sprite.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/31
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
*/

#ifndef IBC_GL_SHADER_POINT_SPRITE_H_
#define IBC_GL_SHADER_POINT_SPRITE_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/shader/shader_base.h"


// Namespace -------------------------------------------------------------------
//namespace ibc::gl::shader // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace shader
{
  // ---------------------------------------------------------------------------
  // Simple
  // ---------------------------------------------------------------------------
  class PointSprite : public virtual ibc::gl::shader::ShaderBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // Simple
    // -------------------------------------------------------------------------
    PointSprite()
    {
      static const char *vertexShaderStr =
        "#version 330\n"
        "in vec3 position;"
        "in vec3 color;"
        "uniform mat4 modelview;"
        "uniform mat4 projection;"
        "smooth out vec4 vertexColor;"
        "varying out vec3 light;"
        "const vec4 lightSource = vec4(0.0, 0.0, 100.0, 1.0);"
        "void main() {"
        "  gl_Position = projection * modelview * vec4(position, 1.0);"
        //"  gl_PointSize = 1 * (5 - gl_Position.z);"
        "  gl_PointSize = 25 / gl_Position.w;"
        //"  gl_PointSize = 15 / gl_Position.w * (projection[0][0] - projection[0][2] * gl_Position.w);"
        //"  gl_PointSize = 20;"
        "  light = normalize(vec3(lightSource - modelview * vec4(position, 1.0)));"
        "  vertexColor = vec4(color, 1.0);"
        "}";
      static const char *fragmentShaderStr =
        "#version 330\n"
        "smooth in vec4 vertexColor;"
        "varying in vec3 light;"
        "out vec4 outputColor;"
        "void main() {"
        "  vec3 n;"
        "  n.xy = gl_PointCoord * 2.0 - 1.0;"
        "  n.z = 1.0 - dot(n.xy, n.xy);"
        "  if (n.z < 0.0) discard;"
        "  n.z = sqrt(n.z);"
        "  vec3 m = normalize(n);"
        "  float d = dot(light, m);"
        //"  outputColor = vertexColor * d;"
        "  outputColor = vertexColor;"
        "}";

      mVertexShaderStr = vertexShaderStr;
      mFragmentShaderStr = fragmentShaderStr;
    }
    // -------------------------------------------------------------------------
    // ~PointSprite
    // -------------------------------------------------------------------------
    virtual ~PointSprite()
    {
    }
    // Member functions -------------------------------------------------------
    // -------------------------------------------------------------------------
    // initShader
    // -------------------------------------------------------------------------
    virtual bool initShader()
    {
      ShaderBase::initShader();
      glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
      //glEnable(GL_POINT_SPRITE);
    }
  };
};};};

#endif  // #ifdef IBC_GL_SHADER_POINT_SPRITE_H_
