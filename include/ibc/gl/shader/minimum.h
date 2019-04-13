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
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
*/

#ifndef IBC_GL_SHADER_MINIMUM_H_
#define IBC_GL_SHADER_MINIMUM_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/shader/shader_base.h"


// Namespace -------------------------------------------------------------------
namespace ibc::gl::shader // <- nested namespace (C++17)
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
};

#endif  // #ifdef IBC_GL_SHADER_MINIMUM_H_
