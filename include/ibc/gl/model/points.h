// =============================================================================
//  points.h
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
  \file     ibc/gl/models/points.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/31
  \brief    Header file for the Points model
*/

#ifndef IBC_GL_MODEL_POINTS_H_
#define IBC_GL_MODEL_POINTS_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/model/model_base.h"
#include "ibc/image/color_map.h"

// Namespace -------------------------------------------------------------------
//namespace ibc::gl::model // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace model
{
  // ---------------------------------------------------------------------------
  // Points
  // ---------------------------------------------------------------------------
  class Points : public virtual ibc::gl::model::ModelBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // Points
    // -------------------------------------------------------------------------
    Points()
    {
      mIsDataNumUpdated = false;
      mIsDataModified   = false;
      mIsVBOInitialized = false;

      mDataNum = 0;
      mDataSize = 0;
    }
    // -------------------------------------------------------------------------
    // ~Points
    // -------------------------------------------------------------------------
    virtual ~Points()
    {
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // setDataPtr
    // -------------------------------------------------------------------------
    // The data format should be
    // { float x, float y, float z}, {float r, float g, float b}
    //
    void setDataPtr(float *inDataPtr, size_t inDataNum)
    {
      if (mDataNum != inDataNum)
        mIsDataNumUpdated = true;

      mDataPtr  = inDataPtr;
      mDataNum  = inDataNum;
      mDataSize = sizeof(float) * mDataNum * 6;
      mIsDataModified = true;
    }
    // -------------------------------------------------------------------------
    // markAsDataModified
    // -------------------------------------------------------------------------
    void markAsDataModified()
    {
      mIsDataModified = true;
    }
    // -------------------------------------------------------------------------
    // initModel
    // -------------------------------------------------------------------------
    virtual bool initModel()
    {
      if (ModelBase::initModel() == false)
        return false;

      // Shader program related initialization
      mShaderProgram = mShaderInterface->getShaderProgram();
      mModelViewLocation  = glGetUniformLocation(mShaderProgram, "modelview");
      mProjectionLocation = glGetUniformLocation(mShaderProgram, "projection");
      mPositionLocation   = glGetAttribLocation (mShaderProgram, "position");
      mColorLocation      = glGetAttribLocation (mShaderProgram, "color");

      // Initialze Vertex Array Object
      glGenVertexArrays(1, &mVertexArrayObject);
      glBindVertexArray(mVertexArrayObject);

      if (mIsDataNumUpdated)
      {
        initVBO();
        mIsDataNumUpdated = false;
      }

      return true;
    }
    // -------------------------------------------------------------------------
    // disposeModel
    // -------------------------------------------------------------------------
    virtual void disposeModel()
    {
      disposeVBO();
    }
    // -------------------------------------------------------------------------
    // drawModel
    // -------------------------------------------------------------------------
    virtual void drawModel(const GLfloat inModelView[16], const GLfloat inProjection[16])
    {
      if (mDataNum == 0)
        return;

      if (mIsDataNumUpdated)
      {
        initVBO();
        mIsDataNumUpdated = false;
      }
      else
      {
        if (mIsDataModified)
          updateVBO();
      }

      glUseProgram(mShaderProgram);

      glUniformMatrix4fv(mModelViewLocation, 1, GL_FALSE, &(inModelView[0]));
      glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, &(inProjection[0]));
      glBindVertexArray(mVertexArrayObject);
      glDrawArrays(GL_POINTS, 0, mDataNum);
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
    bool  mIsDataNumUpdated;
    bool  mIsDataModified;
    bool  mIsVBOInitialized;

    void  *mDataPtr;
    size_t  mDataNum;
    size_t  mDataSize;

    GLuint mShaderProgram;
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;

    GLint mModelViewLocation;
    GLint mProjectionLocation;
    GLint mPositionLocation;
    GLint mColorLocation;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // initVBO
    // -------------------------------------------------------------------------
    void initVBO()
    {
      if (mIsVBOInitialized)
        disposeVBO();
      //
      glBindVertexArray(mVertexArrayObject);
      glGenBuffers(1, &mVertexBufferObject);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glBufferData(GL_ARRAY_BUFFER, mDataSize, NULL, GL_DYNAMIC_DRAW);
      mIsVBOInitialized = true;
      //
      updateVBO();

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glEnableVertexAttribArray(mPositionLocation);
      glVertexAttribPointer(mPositionLocation, 3, GL_FLOAT, GL_FALSE,
                            sizeof (struct vertex_info),
                            (const GLvoid *)offsetof(struct vertex_info, position));
      glEnableVertexAttribArray(mColorLocation);
      glVertexAttribPointer(mColorLocation, 3, GL_FLOAT, GL_FALSE,
                            sizeof (struct vertex_info),
                            (const GLvoid *)offsetof(struct vertex_info, color));
    }
    // -------------------------------------------------------------------------
    // updateVBO
    // -------------------------------------------------------------------------
    void updateVBO()
    {
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glBufferSubData(GL_ARRAY_BUFFER, 0, mDataSize, mDataPtr);
      mIsDataModified = false;
    }
    // -------------------------------------------------------------------------
    // disposeVBO
    // -------------------------------------------------------------------------
    void disposeVBO()
    {
      if (mIsVBOInitialized == false)
        return;
      //
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
      glDeleteBuffers(1, &mVertexBufferObject);
      mIsVBOInitialized = false;
    }
  };
};};};

#endif  // #ifdef IBC_GL_MODEL_POINTS_H_
