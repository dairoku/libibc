// =============================================================================
//  backdrop.h
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
  \file     ibc/gl/models/backdrop.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/24
  \brief    Header file for the Simple shader
*/

#ifndef IBC_GL_SHADER_BACKDROP_H_
#define IBC_GL_SHADER_BACKDROP_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/shader/shader_base.h"


// Namespace -------------------------------------------------------------------
//namespace ibc::gl::shader // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace shader
{
  // ---------------------------------------------------------------------------
  // Backdrop
  // ---------------------------------------------------------------------------
  class Backdrop : public virtual ibc::gl::shader::ShaderBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // Backdrop
    // -------------------------------------------------------------------------
    Backdrop()
    {
      static const char *vertexShaderStr =
        "#version 330\n"
        "in vec3 position;"
        "in vec3 color;"
        "uniform mat4 modelview;"
        "uniform mat4 projection;"
        "smooth out vec4 vertexColor;"
        "void main() {"
        "  gl_Position = vec4(position.xy, 0.999999, 1.0);"
        //"  vertexColor = vec4(0.2, 0.1, 0.1, 1.0);"
        "  vertexColor = vec4(color, 1.0);"
        "}";
      static const char *fragmentShaderStr =
        "#version 330\n"
        "smooth in vec4 vertexColor;"
        "out vec4 outputColor;"
        "void main() {"
        "  outputColor = vertexColor;"
        "}";

      mVertexShaderStr = vertexShaderStr;
      mFragmentShaderStr = fragmentShaderStr;
    }
    // -------------------------------------------------------------------------
    // ~Backdrop
    // -------------------------------------------------------------------------
    virtual ~Backdrop()
    {
    }
  };
};};};

#endif  // #ifdef IBC_GL_SHADER_BACKDROP_H_
