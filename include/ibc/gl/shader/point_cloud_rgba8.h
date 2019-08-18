// =============================================================================
//  point_cloud_rgba8.h
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
  \file     ibc/gl/models/point_cloud_rgba8.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/05/19
  \brief    Header file for PointCloud RGBA8 shader
*/

#ifndef IBC_GL_SHADER_POINT_CLOUD_RGBA8_H_
#define IBC_GL_SHADER_POINT_CLOUD_RGBA8_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/shader/shader_base.h"


// Namespace -------------------------------------------------------------------
//namespace ibc::gl::shader // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace shader
{
  // ---------------------------------------------------------------------------
  // PointCloud
  // ---------------------------------------------------------------------------
  class PointCloudRGBA8 : public virtual ibc::gl::shader::ShaderBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // Simple
    // -------------------------------------------------------------------------
    PointCloudRGBA8()
    {
      static const char *vertexShaderStr =
        "#version 330\n"
        "in vec3 position;"
        "in vec4 color;"
        "uniform vec4 fit;"
        "uniform mat4 modelview;"
        "uniform mat4 projection;"
        "uniform float pointSize;"
        "uniform int colorMode;"
        "uniform vec4 colorMapParam;"
        "uniform vec4 singleColor;"
        "uniform highp sampler1D colorMapTexture;"
        "smooth out vec4 vertexColor;"
        "varying out vec3 light;"
        "const vec4 lightSource = vec4(0.0, 0.0, 100.0, 1.0);"
        "void main() {"
        "  vec3 scaled;"
        "  scaled = (position + vec3(fit.x, fit.y, fit.z)) * fit.w;"
        "  gl_Position = projection * modelview * vec4(scaled, 1.0);"
        "  gl_PointSize = pointSize / gl_Position.w;"
        "  light = normalize(vec3(lightSource - modelview * vec4(position, 1.0)));"
        "  float colorIndex;"
        "  if (colorMapParam.x == 0)"
        "    colorIndex = position.x;"
        "  else if (colorMapParam.x == 1)"
        "    colorIndex = position.y;"
        "  else"
        "    colorIndex = position.z;"
        "  colorIndex = (colorIndex - colorMapParam.y) * colorMapParam.z;"
        "  vec4 mappedColor;"
        "  if (colorIndex < 0 || colorIndex > 1)"
        "    mappedColor = vec4(singleColor.rgb, colorMapParam.w);"
        "  else"
        "    mappedColor = texture(colorMapTexture, colorIndex);"
        "  if (colorMode == 0)"
        "    vertexColor = singleColor;"
        "  else if (colorMode == 1)"
        "    vertexColor = mappedColor;"
        "  else"
        "    vertexColor = color / 255.0;"
        "}";
      static const char *fragmentShaderStr =
        "#version 330\n"
        "smooth in vec4 vertexColor;"
        "varying in vec3 light;"
        "out vec4 outputColor;"
        "void main() {"
        "  if (vertexColor.w == 0)"
        "    discard;"
        "  vec3 n;"
        "  n.xy = gl_PointCoord * 2.0 - 1.0;"
        "  n.z = 1.0 - dot(n.xy, n.xy);"
        "  if (n.z < 0.0) discard;" // To make it circle
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
    virtual ~PointCloudRGBA8()
    {
    }
    // Member functions -------------------------------------------------------
    // -------------------------------------------------------------------------
    // initShader
    // -------------------------------------------------------------------------
    virtual bool initShader()
    {
      if (ShaderBase::initShader() == false)
        return false;
      ShaderBase::initShader();
      glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
      glEnable(GL_POINT_SPRITE);
      return true;
    }
  };
};};};

#endif  // #ifdef IBC_GL_SHADER_POINT_CLOUD_RGBA8_H_
