// =============================================================================
//  color_triangle.h
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
  \file     ibc/gl/models/color_triangle.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/24
  \brief    Header file for the ColorTriangle model
*/

#ifndef IBC_GL_MODEL_COLOR_TRIANGLE_H_
#define IBC_GL_MODEL_COLOR_TRIANGLE_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/model/model_base.h"

// Namespace -------------------------------------------------------------------
//namespace ibc::gl::model // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace model
{
  // ---------------------------------------------------------------------------
  // ColorTriangle
  // ---------------------------------------------------------------------------
  class ColorTriangle : public virtual ibc::gl::model::ModelBase
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
    // initModel
    // -------------------------------------------------------------------------
    virtual bool initModel()
    {
      if (ModelBase::initModel() == false)
        return false;

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
    GLuint mShaderProgram;
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;

    GLint mModelViewLocation;
    GLint mProjectionLocation;
  };
};};};

#endif  // #ifdef IBC_GL_MODEL_COLOR_TRIANGLE_H_
