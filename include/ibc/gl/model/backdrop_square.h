// =============================================================================
//  solid_square.h
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
  \file     ibc/gl/models/backdrop_square.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/08/18
  \brief    Header file for BackdropSquare model
*/

#ifndef IBC_GL_MODEL_BACKDROP_SQUARE_H_
#define IBC_GL_MODEL_BACKDROP_SQUARE_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/model/model_base.h"

// Namespace -------------------------------------------------------------------
//namespace ibc::gl::model // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace model
{
  // ---------------------------------------------------------------------------
  // BackdropSquare class
  // ---------------------------------------------------------------------------
  class BackdropSquare : public virtual ibc::gl::model::ModelBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // BackdropSquare
    // -------------------------------------------------------------------------
    BackdropSquare()
    {
      mShaderInterface = NULL;
      mIsBackdropColorModified = false;
    }
    // -------------------------------------------------------------------------
    // ~BackdropSquare
    // -------------------------------------------------------------------------
    virtual ~BackdropSquare()
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

      static const GLuint indexData[] =
      {
        2, 1, 0,
        3, 2, 0
      };

      mShaderProgram = mShaderInterface->getShaderProgram();

      glGenVertexArrays(1, &mVertexArrayObject);
      glBindVertexArray(mVertexArrayObject);

      glGenBuffers(1, &mVertexBufferObject);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glBufferData(GL_ARRAY_BUFFER, (4 * 3 * 2 * sizeof(GLfloat)), NULL, GL_DYNAMIC_DRAW);
      updateVBO();

      glGenBuffers(1, &mIndexBufferObject);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, (2 * 3 * sizeof(GLuint)), indexData, GL_STATIC_DRAW);

      // get the location of the "modelview" uniform
      mModelViewLocation = glGetUniformLocation (mShaderProgram, "modelview");

      // get the location of the "projection" uniform
      mProjectionLocation = glGetUniformLocation (mShaderProgram, "projection");

      // get the location of the "position" and "color" attributes
      GLint positionLocation = glGetAttribLocation (mShaderProgram, "position");
      GLint colorLocation = glGetAttribLocation (mShaderProgram, "color");

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

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
    // setBackdropColor
    // -------------------------------------------------------------------------
    void setBackdropColor(const GLfloat inTopColor[3], const GLfloat inBottomColor[3])
    {
      struct vertex_info *vertexData = getVertexData();
      for (int i = 0; i < 3; i++)
      {
        vertexData[0].color[i] = inBottomColor[i];
        vertexData[1].color[i] = inTopColor[i];
        vertexData[2].color[i] = inTopColor[i];
        vertexData[3].color[i] = inBottomColor[i];
      }
      mIsBackdropColorModified = true;
    }
    // -------------------------------------------------------------------------
    // getBackdropColor
    // -------------------------------------------------------------------------
    void getBackdropColor(GLfloat outTopColor[3], GLfloat outBottomColor[3])
    {
      struct vertex_info *vertexData = getVertexData();
      for (int i = 0; i < 3; i++)
      {
        outBottomColor[i] = vertexData[0].color[i];
        outTopColor[i] = vertexData[1].color[i];
        outTopColor[i] = vertexData[2].color[i];
        outBottomColor[i] = vertexData[3].color[i];
      }
    }
    // -------------------------------------------------------------------------
    // drawModel
    // -------------------------------------------------------------------------
    virtual void drawModel(const GLfloat inModelView[16], const GLfloat inProjection[16])
    {
      if (mIsBackdropColorModified)
        updateVBO();

      glUseProgram(mShaderProgram);
      glUniformMatrix4fv(mModelViewLocation, 1, GL_FALSE, &(inModelView[0]));
      glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, &(inProjection[0]));
      glBindVertexArray(mVertexArrayObject);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

    // -------------------------------------------------------------------------
    // getVertexData
    // -------------------------------------------------------------------------
    struct vertex_info  *getVertexData()
    {
      static struct vertex_info vertexData[] =
      {
        { { -1.0f, -1.0f,  0.0f }, { 0.3f, 0.3f, 0.3f } },
        { { -1.0f,  1.0f,  0.0f }, { 0.3f, 0.3f, 0.3f } },
        { {  1.0f,  1.0f,  0.0f }, { 0.3f, 0.3f, 0.3f } },
        { {  1.0f, -1.0f,  0.0f }, { 0.3f, 0.3f, 0.3f } }
      };
      return vertexData;
    };
    // -------------------------------------------------------------------------
    // updateVBO
    // -------------------------------------------------------------------------
    void updateVBO()
    {
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glBufferSubData(GL_ARRAY_BUFFER, 0, (4 * 3 * 2 * sizeof(GLfloat)), getVertexData());
      mIsBackdropColorModified = false;
    }

    // Member variables --------------------------------------------------------
    bool  mIsBackdropColorModified;

    GLuint mShaderProgram;
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mIndexBufferObject;

    GLint mModelViewLocation;
    GLint mProjectionLocation;
  };
};};};

#endif  // #ifdef IBC_GL_MODEL_BACKDROP_SQUARE_H_
