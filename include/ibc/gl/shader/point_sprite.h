// =============================================================================
//  point_sprite.h
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
  \file     ibc/gl/models/point_sprite.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/31
  \brief    Header file for the PointSprite shader (for the SurfacePlot)
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
        "in float intensity;\n"
        "uniform vec2 dataSize;\n"
        "uniform float zGain;\n"
        "uniform float zOffset;\n"
        "uniform vec2  zClamp;\n"
        "uniform float intensityMax;\n"
        "uniform float intensityGain;\n"
        "uniform float intensityOffset;\n"
        "uniform vec2  intensityClamp;\n"
        "uniform vec4 lightSource;\n"
        "uniform mat4 modelview;\n"
        "uniform mat4 projection;\n"
        "flat out int colorMapIndex;\n"
        "out vec3 light;\n"
        "void main() {\n"
        "  float pitch;\n"
        "  int i, j;\n"
        "  float v;\n"
        "  vec3 position;\n"
        "  pitch = 2.0 / dataSize.x;\n"
        "  i = gl_VertexID / int(dataSize.x);\n"
        "  j = gl_VertexID - int(dataSize.x) * i;\n"
        "  position.x = -1.0 + pitch * j;\n"
        "  position.y = -1.0 + pitch * i;\n"
        "  position.z = clamp(intensity * zGain + zOffset, zClamp.x, zClamp.y);\n"
        "  gl_Position = projection * modelview * vec4(position, 1.0);\n"
        "  gl_PointSize = 25 / gl_Position.w;\n"
        "  light = normalize(vec3(lightSource - modelview * vec4(position, 1.0)));\n"
        "  v = intensity * intensityGain + intensityOffset;\n"
        "  v = clamp(v, intensityClamp.x, intensityClamp.y);\n"
        "  colorMapIndex = int(v);\n"
        "}\n";

      static const char *fragmentShaderStr =
        "#version 330\n"
        "flat in int colorMapIndex;\n"
        "uniform sampler1D s;\n"
        "uniform int material;\n"
        "in vec3 light;\n"
        "out vec4 outputColor;\n"
        "void main() {\n"
        "  vec3 n;\n"
        "  n.xy = gl_PointCoord * 2.0 - 1.0;\n"
        "  n.z = 1.0 - dot(n.xy, n.xy);\n"
        "  if (n.z < 0.0) discard;\n"
        "  n.z = sqrt(n.z);\n"
        "  vec3 m = normalize(n);\n"
        "  float d;\n"
        "  if (material == 0)\n"
        "    d = 1;\n"
        "  else\n"
        "    d = dot(light, m);\n"
        "  outputColor = texelFetch(s, colorMapIndex, 0) * d;\n"
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
      if (ShaderBase::initShader() == false)
        return false;
      glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
      glEnable(GL_POINT_SPRITE);
      return true;
    }
  };
};};};

#endif  // #ifdef IBC_GL_SHADER_POINT_SPRITE_H_
