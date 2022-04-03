// =============================================================================
//  minimum.h
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
  \file     ibc/gl/models/minimum.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/24
  \brief    Header file for the Minimum shader (no model view /projection matrix)
*/

#ifndef IBC_GL_SHADER_MINIMUM_H_
#define IBC_GL_SHADER_MINIMUM_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/shader/shader_base.h"


// Namespace -------------------------------------------------------------------
//namespace ibc::gl::shader // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace shader
{
  // ---------------------------------------------------------------------------
  // Minimum
  // ---------------------------------------------------------------------------
  class Minimum : public virtual ibc::gl::shader::ShaderBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // Minimum
    // -------------------------------------------------------------------------
    Minimum()
    {
      static const char *vertexShaderStr =
        "#version 330\n"
        "in vec3 vp;"
        "void main ()"
        "{"
        "  gl_Position = vec4(vp, 1.0);"
        "}";
      static const char *fragmentShaderStr =
        "#version 330\n"
        "out vec4 frag_colour;"
        "void main ()"
        "{"
        "  frag_colour = vec4(1.0, 1.0, 0.0, 1.0);"
        "}";

      mVertexShaderStr = vertexShaderStr;
      mFragmentShaderStr = fragmentShaderStr;
    }
    // -------------------------------------------------------------------------
    // ~Minimum
    // -------------------------------------------------------------------------
    virtual ~Minimum()
    {
    }
  };
};};};

#endif  // #ifdef IBC_GL_SHADER_MINIMUM_H_
