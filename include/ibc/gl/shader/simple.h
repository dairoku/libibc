// =============================================================================
//  shader_base.h
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
  \file     ibc/gl/models/shader_base.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/24
  \brief    Header file for the Simple shader
*/

#ifndef IBC_GL_SHADER_SIMPLE_H_
#define IBC_GL_SHADER_SIMPLE_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/shader/shader_base.h"


// Namespace -------------------------------------------------------------------
//namespace ibc::gl::shader // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace shader
{
  // ---------------------------------------------------------------------------
  // Simple
  // ---------------------------------------------------------------------------
  class Simple : public virtual ibc::gl::shader::ShaderBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // Simple
    // -------------------------------------------------------------------------
    Simple()
    {
      static const char *vertexShaderStr =
        "#version 330\n"
        "in vec3 position;"
        "in vec3 color;"
        "uniform mat4 modelview;"
        "uniform mat4 projection;"
        "smooth out vec4 vertexColor;"
        "void main() {"
        "  gl_Position = projection * modelview * vec4(position, 1.0);"
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
    // ~Simple
    // -------------------------------------------------------------------------
    virtual ~Simple()
    {
    }
  };
};};};

#endif  // #ifdef IBC_GL_SHADER_SHADER_BASE_H_
