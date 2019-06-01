// =============================================================================
//  shader_interface.h
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
  \file     ibc/gl/models/shader_interface.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/26
  \brief    Header file for defining ShaderInterface
*/

#ifndef IBC_GL_SHADER_INTERFACE_H_
#define IBC_GL_SHADER_INTERFACE_H_

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gl
 {
  // ---------------------------------------------------------------------------
  // Shader interface class
  // ---------------------------------------------------------------------------
  class ShaderInterface
  {
  public:
    virtual bool initShader() = 0;
    virtual void disposeShader() = 0;
    virtual GLuint  getShaderProgram() = 0;
    virtual GLuint  getVertexShader() = 0;
    virtual GLuint  getFragmentShader() = 0;
    virtual GLuint  geGeometryShader() = 0;
  };
 };
};

#endif  // #ifdef IBC_GL_SHADER_INTERFACE_H_
