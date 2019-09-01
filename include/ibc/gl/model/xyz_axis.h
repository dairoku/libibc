// =============================================================================
//  color_cube.h
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
  \file     ibc/gl/models/xyz_axis.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/08/31
  \brief    Header file for XYZAxis model
*/

#ifndef IBC_GL_MODEL_XYZ_AXIS_H_
#define IBC_GL_MODEL_XYZ_AXIS_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/model/model_base.h"

// Namespace -------------------------------------------------------------------
//namespace ibc::gl::model // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace model
{
  // ---------------------------------------------------------------------------
  // XYZAxis class
  // ---------------------------------------------------------------------------
  class XYZAxis : public virtual ibc::gl::model::ModelBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // XYZAxis
    // -------------------------------------------------------------------------
    XYZAxis()
    {
      mShaderInterface = NULL;
    }
    // -------------------------------------------------------------------------
    // ~XYZAxis
    // -------------------------------------------------------------------------
    virtual ~XYZAxis()
    {
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // initModel
    // -------------------------------------------------------------------------
    virtual bool initModel()
    {
      if (ModelBase::initModel() == false)
        return false;

      static const struct vertex_info vertexData[] =
      {
        { { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.7f, 0.7f } },
        { {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f } },
        { {  0.0f, -1.0f,  0.0f }, { 0.7f, 1.0f, 0.7f } },
        { {  0.0f,  1.0f,  0.0f }, { 0.0f, 1.0f, 0.0f } },
        { {  0.0f,  0.0f, -1.0f }, { 0.7f, 0.7f, 1.0f } },
        { {  0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f, 1.0f } },
        //
        { {  0.95f,  0.025f,  0.0f }, { 1.0f, 0.0f, 0.0f } },
        { {  0.95f, -0.025f,  0.0f }, { 1.0f, 0.0f, 0.0f } },

        { {  0.025f,  0.95f,  0.0f }, { 0.0f, 1.0f, 0.0f } },
        { { -0.025f,  0.95f,  0.0f }, { 0.0f, 1.0f, 0.0f } },
        { {  0.0f,  0.025f,  0.95f }, { 0.0f, 0.0f, 1.0f } },
        { {  0.0f, -0.025f,  0.95f }, { 0.0f, 0.0f, 1.0f } }
      };
      static const GLuint indexData[] =
      {
        0, 1,
        2, 3,
        4, 5,
        //
        1, 6,
        1, 7,
        3, 8,
        3, 9,
        5, 10,
        5, 11
      };

      mShaderProgram = mShaderInterface->getShaderProgram();

      glGenVertexArrays(1, &mVertexArrayObject);
      glBindVertexArray(mVertexArrayObject);

      glGenBuffers(1, &mVertexBufferObject);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

      glGenBuffers(1, &mIndexBufferObject);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

      // get the location of the "modelview" uniform
      mModelViewLocation = glGetUniformLocation (mShaderProgram, "modelview");

      // get the location of the "projection" uniform
      mProjectionLocation = glGetUniformLocation (mShaderProgram, "projection");

      // get the location of the "position" and "color" attributes
      GLint positionLocation = glGetAttribLocation (mShaderProgram, "position");
      GLint colorLocation = glGetAttribLocation (mShaderProgram, "color");

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

      // enable and set the position attribute
      glEnableVertexAttribArray(positionLocation);
      glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE,
                             sizeof (struct vertex_info),
                             (const GLvoid *)offsetof(struct vertex_info, position));

      // enable and set the color attribute
      glEnableVertexAttribArray(colorLocation);
      glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE,
                             sizeof (struct vertex_info),
                             (const GLvoid *)offsetof(struct vertex_info, color));
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
      glDrawElements(GL_LINES, 18, GL_UNSIGNED_INT, 0);
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
    GLuint mShaderProgram;
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mIndexBufferObject;

    GLint mModelViewLocation;
    GLint mProjectionLocation;
  };
};};};

#endif  // #ifdef IBC_GL_MODEL_XYZ_AXIS_H_
