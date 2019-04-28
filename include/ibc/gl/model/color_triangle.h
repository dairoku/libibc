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
  \file     ibc/gl/models/color_triangle.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/24
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
*/

#ifndef IBC_GL_MODEL_COLOR_TRIANGLE_H_
#define IBC_GL_MODEL_COLOR_TRIANGLE_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/model_interface.h"
#include "ibc/gl/shader_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc::gl::model // <- nested namespace (C++17)
{
  // ---------------------------------------------------------------------------
  // ColorTriangle
  // ---------------------------------------------------------------------------
  class ColorTriangle : public virtual ibc::gl::ModelInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ColorTriangle
    // -------------------------------------------------------------------------
    ColorTriangle()
    {
      mShaderInterface = NULL;
    }
    // -------------------------------------------------------------------------
    // ~ColorTriangle
    // -------------------------------------------------------------------------
    virtual ~ColorTriangle()
    {
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // setShader
    // -------------------------------------------------------------------------
    virtual void setShader(ibc::gl::ShaderInterface *inShaderInterface)
    {
      mShaderInterface = inShaderInterface;
    }
    // -------------------------------------------------------------------------
    // getShader
    // -------------------------------------------------------------------------
    virtual ibc::gl::ShaderInterface *getShader()
    {
      return mShaderInterface;
    }
    // -------------------------------------------------------------------------
    // initModel
    // -------------------------------------------------------------------------
    virtual bool initModel()
    {
      static const struct vertex_info vertexData[] =
      {
        { {  0.0f,  0.500f, 0.0f }, { 1.f, 0.f, 0.f } },
        { {  0.5f, -0.366f, 0.0f }, { 0.f, 1.f, 0.f } },
        { { -0.5f, -0.366f, 0.0f }, { 0.f, 0.f, 1.f } },
      };

      mShaderProgram = mShaderInterface->getShaderProgram();

      glGenVertexArrays(1, &mVertexArrayObject);
      glBindVertexArray(mVertexArrayObject);

      glGenBuffers(1, &mVertexBufferObject);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glBufferData(GL_ARRAY_BUFFER, (9 * 2 * sizeof(GLfloat)), vertexData, GL_STATIC_DRAW);

      // get the location of the "modelview" uniform
      mModelViewLocation = glGetUniformLocation (mShaderProgram, "modelview");

      // get the location of the "projection" uniform
      mProjectionLocation = glGetUniformLocation (mShaderProgram, "projection");

      // get the location of the "position" and "color" attributes
      guint positionLocation = glGetAttribLocation (mShaderProgram, "position");
      guint colorLocation = glGetAttribLocation (mShaderProgram, "color");

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

      // enable and set the position attribute
      glEnableVertexAttribArray (positionLocation);
      glVertexAttribPointer (positionLocation, 3, GL_FLOAT, GL_FALSE,
                             sizeof (struct vertex_info),
                             (GLvoid *) (G_STRUCT_OFFSET (struct vertex_info, position)));

      // enable and set the color attribute
      glEnableVertexAttribArray (colorLocation);
      glVertexAttribPointer (colorLocation, 3, GL_FLOAT, GL_FALSE,
                             sizeof (struct vertex_info),
                             (GLvoid *) (G_STRUCT_OFFSET (struct vertex_info, color)));

      return true;
    }
    // -------------------------------------------------------------------------
    // disposeModel
    // -------------------------------------------------------------------------
    virtual void disposeModel()
    {
    }
    // -------------------------------------------------------------------------
    // drawModel
    // -------------------------------------------------------------------------
    virtual void drawModel(const GLfloat inModelView[16], const GLfloat inProjection[16])
    {
      glUseProgram(mShaderProgram);
      glUniformMatrix4fv(mModelViewLocation, 1, GL_FALSE, &(inModelView[0]));
      glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, &(inProjection[0]));
      glBindVertexArray(mVertexArrayObject);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

  protected:
    // structs -----------------------------------------------------------------
    // -------------------------------------------------------------------------
    // vertex_info
    // -------------------------------------------------------------------------
    struct vertex_info
    {
      GLfloat position[3];
      GLfloat color[3];
    };

    // Member variables --------------------------------------------------------
    ibc::gl::ShaderInterface *mShaderInterface;
    GLuint mShaderProgram;

    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;

    guint mModelViewLocation;
    guint mProjectionLocation;
  };
};

#endif  // #ifdef IBC_GL_MODEL_COLOR_TRIANGLE_H_