// =============================================================================
//  point_cloud.h
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
  \file     ibc/gl/models/point_cloud.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/31
  \brief    Header file for PointCloud shader
*/

#ifndef IBC_GL_SHADER_POINT_CLOUD_H_
#define IBC_GL_SHADER_POINT_CLOUD_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/shader/shader_base.h"


// Namespace -------------------------------------------------------------------
//namespace ibc::gl::shader // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace shader
{
  // ---------------------------------------------------------------------------
  // PointCloud
  // ---------------------------------------------------------------------------
  class PointCloud : public virtual ibc::gl::shader::ShaderBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // Simple
    // -------------------------------------------------------------------------
    PointCloud()
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
        "  gl_PointSize = 25 / gl_Position.w;"
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
    virtual ~PointCloud()
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

#endif  // #ifdef IBC_GL_SHADER_POINT_CLOUD_H_
