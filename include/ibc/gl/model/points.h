// =============================================================================
//  points.h
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
  \file     ibc/gl/models/points.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/31
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
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
      mModelViewLocation      = glGetUniformLocation (mShaderProgram, "modelview");
      mProjectionLocation     = glGetUniformLocation (mShaderProgram, "projection");
      GLint positionLocation  = glGetAttribLocation (mShaderProgram, "position");
      GLint colorLocation     = glGetAttribLocation (mShaderProgram, "color");

      // Initialze Vertex Array Object
      glGenVertexArrays(1, &mVertexArrayObject);
      glBindVertexArray(mVertexArrayObject);

      if (mIsDataNumUpdated)
      {
        initVBO();
        mIsDataNumUpdated = false;
      }

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); //<- we don't need this?
      //
      glEnableVertexAttribArray (positionLocation);
      glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE,
                            sizeof (struct vertex_info),
                            (const GLvoid *)offsetof(struct vertex_info, position));
      //
      glEnableVertexAttribArray (colorLocation);
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
      disposeVBO();
    }
    // -------------------------------------------------------------------------
    // drawModel
    // -------------------------------------------------------------------------
    virtual void drawModel(const GLfloat inModelView[16], const GLfloat inProjection[16])
    {
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

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // initVBO
    // -------------------------------------------------------------------------
    void initVBO()
    {
      if (mIsVBOInitialized)
        disposeVBO();
      //
      glGenBuffers(1, &mVertexBufferObject);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glBufferData(GL_ARRAY_BUFFER, mDataSize, NULL, GL_DYNAMIC_DRAW);
      mIsVBOInitialized = true;
      //
      updateVBO();
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
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDeleteBuffers(1, &mVertexBufferObject);
      mIsVBOInitialized = false;
    }
  };
};};};

#endif  // #ifdef IBC_GL_MODEL_POINTS_H_
