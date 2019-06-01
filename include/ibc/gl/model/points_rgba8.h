// =============================================================================
//  points_rgba8.h
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
  \file     ibc/gl/models/points_rgba8.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/05/19
  \brief    Header file for the Points model (RGBA8 version)
*/

#ifndef IBC_GL_MODEL_POINTS_RGBA8H_
#define IBC_GL_MODEL_POINTS_RGBA8H_

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
  class PointsRGBA8 : public virtual ibc::gl::model::ModelBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // PointsRGBA8
    // -------------------------------------------------------------------------
    PointsRGBA8()
    {
      mIsDataNumUpdated = false;
      mIsDataModified   = false;
      mIsVBOInitialized = false;

      mDataNum = 0;
      mDataSize = 0;

      mModelFitParam[0] = 0.0;
      mModelFitParam[1] = 0.0;
      mModelFitParam[2] = 0.0;
      mModelFitParam[3] = 1.0;
    }
    // -------------------------------------------------------------------------
    // ~PointsRGBA8
    // -------------------------------------------------------------------------
    virtual ~PointsRGBA8()
    {
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // setDataPtr
    // -------------------------------------------------------------------------
    // The data format should be
    // { GLfloat x, GLfloat y, GLfloat z},
    //  {GLubyte r, GLubyte g, GLubyte b, GLubyte a}
    //
    void setDataPtr(float *inDataPtr, size_t inDataNum)
    {
      if (mDataNum != inDataNum)
        mIsDataNumUpdated = true;

      mDataPtr  = inDataPtr;
      mDataNum  = inDataNum;
      mDataSize = sizeof(vertex_info) * mDataNum;
      mIsDataModified = true;
    }
    // -------------------------------------------------------------------------
    // setModelFitParam
    // -------------------------------------------------------------------------
    void setModelFitParam(const GLfloat inModelFitParam[4])
    {
      for (int i = 0; i < 4; i++)
        mModelFitParam[i] = inModelFitParam[i];
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
      mModelFitLocation   = glGetUniformLocation(mShaderProgram, "fit");
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

      glUniform4fv(mModelFitLocation, 1, &(mModelFitParam[0]));
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
      GLubyte color[4];
    };

    // Member variables --------------------------------------------------------
    bool  mIsDataNumUpdated;
    bool  mIsDataModified;
    bool  mIsVBOInitialized;

    void  *mDataPtr;
    size_t  mDataNum;
    size_t  mDataSize;

    GLfloat mModelFitParam[4];

    GLuint mShaderProgram;
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;

    GLint mModelFitLocation;
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
      glVertexAttribPointer(mColorLocation, 4, GL_UNSIGNED_BYTE, GL_FALSE,
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

#endif  // #ifdef IBC_GL_MODEL_POINTS_RGBA8H_
