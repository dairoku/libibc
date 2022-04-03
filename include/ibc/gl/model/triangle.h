// =============================================================================
//  triangle.h
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
  \file     ibc/gl/models/triangle.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/24
  \brief    Header file for the Triangle model
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
#include "ibc/gl/model/model_base.h"

// Namespace -------------------------------------------------------------------
//namespace ibc::gl::model // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace model
{
  // ---------------------------------------------------------------------------
  // triangle class
  // ---------------------------------------------------------------------------
  class Triangle : public virtual ibc::gl::model::ModelBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // Triangle
    // -------------------------------------------------------------------------
    Triangle()
    {
      mShaderInterface = NULL;
    }
    // -------------------------------------------------------------------------
    // ~Triangle
    // -------------------------------------------------------------------------
    virtual ~Triangle()
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

      static GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };

      mShaderProgram = mShaderInterface->getShaderProgram();

      glGenVertexArrays(1, &mVertexArrayObject);
      glBindVertexArray(mVertexArrayObject);

      glGenBuffers(1, &mVertexBufferObject);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glBufferData(GL_ARRAY_BUFFER, (9 * sizeof(GLfloat)), points, GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

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
      UNUSED(inModelView);
      UNUSED(inProjection);
      //
      glUseProgram(mShaderProgram);
      glBindVertexArray(mVertexArrayObject);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

  protected:
    // Member variables --------------------------------------------------------
    GLuint mShaderProgram;
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
  };
};};};

#endif  // #ifdef IBC_GL_MODEL_TRIANGLE_H_
