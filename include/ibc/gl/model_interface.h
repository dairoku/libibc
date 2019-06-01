// =============================================================================
//  model_interface.h
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
  \file     ibc/gl/models/model_interface.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/16
  \brief    Header file for defining the ModelInterface
*/

#ifndef IBC_GL_MODEL_INTERFACE_H_
#define IBC_GL_MODEL_INTERFACE_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/shader_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gl
 {
  // ---------------------------------------------------------------------------
  // Model interface class
  // ---------------------------------------------------------------------------
  class ModelInterface
  {
  public:
    virtual void setShader(ibc::gl::ShaderInterface *inShaderInterface) = 0;
    virtual ibc::gl::ShaderInterface *getShader() = 0;
    virtual bool initModel() = 0;
    virtual void disposeModel() = 0;
    virtual void drawModel(const GLfloat inModelView[16], const GLfloat inProjection[16]) = 0;
  };
 };
};

#endif  // #ifdef IBC_GL_MODEL_INTERFACE_H_
