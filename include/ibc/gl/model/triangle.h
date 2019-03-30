// =============================================================================
//  triangle.h
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
  \file     ibc/gl/models/triangle.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/24
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
*/
// -----------------------------------------------------------------------------
// Appendix
//
// Note that this model does not support modelview and projection matrix
// to be the simplest example
// -----------------------------------------------------------------------------

#ifndef IBC_GL_MODEL_TRIANGLE_H_
#define IBC_GL_MODEL_TRIANGLE_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/model_interface.h"
#include "ibc/gl/shader_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc::gl::model // <- nested namespace (C++17)
{
  // ---------------------------------------------------------------------------
  // triangle class
  // ---------------------------------------------------------------------------
  class Triangle : public virtual ibc::gl::ModelInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // Triangle
    // -------------------------------------------------------------------------
    Triangle(ibc::gl::ShaderInterface *inShaderInterface)
    {
      mShaderInterface = inShaderInterface;
    }
    // -------------------------------------------------------------------------
    // ~Mono_to_RGB
    // -------------------------------------------------------------------------
    virtual ~Triangle()
    {
    }
    // Member functions -------------------------------------------------------
    // -------------------------------------------------------------------------
    // init
    // -------------------------------------------------------------------------
    virtual void init()
    {
      static GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };

      mShaderProgram = mShaderInterface->getShaderProgram();

      glGenBuffers(1, &mVertexBufferObject);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glBufferData(GL_ARRAY_BUFFER, (9 * sizeof(GLfloat)), points, GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    }
    // -------------------------------------------------------------------------
    // dispose
    // -------------------------------------------------------------------------
    virtual void dispose()
    {
    }
    // -------------------------------------------------------------------------
    // draw
    // -------------------------------------------------------------------------
    virtual void draw(const GLfloat inModelView[16], const GLfloat inProjection[16])
    {
      glUseProgram(mShaderProgram);
      glBindVertexArray(mVertexArrayObject);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

  protected:
    // Member variables --------------------------------------------------------
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mShaderProgram;
  };
};

#endif  // #ifdef IBC_GL_MODEL_TRIANGLE_H_
