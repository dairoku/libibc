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
      mIsColorMapIndexModified = false;

      mDataNum = 0;
      mDataSize = 0;

      mPointSize = 5.0;
      mColorMode = 0;
      mSingleColor[0] = 1.0;
      mSingleColor[1] = 1.0;
      mSingleColor[2] = 1.0;
      mSingleColor[3] = 1.0;

      mColorMapIndex = ibc::image::ColorMap::CMIndex_SpectrumWide;
      mColorMapRepeatNum = 1;
      mColorMapSize = 1024; // =2^10

      mModelFitParam[0] = 0.0;
      mModelFitParam[1] = 0.0;
      mModelFitParam[2] = 0.0;
      mModelFitParam[3] = 1.0;

      mColorMapParam[0] = 2.0;
      mColorMapParam[1] = 0.0;
      mColorMapParam[2] = 1.0;
      mColorMapParam[3] = 0.0;
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
      mModelFitLocation     = glGetUniformLocation(mShaderProgram, "fit");
      mModelViewLocation    = glGetUniformLocation(mShaderProgram, "modelview");
      mProjectionLocation   = glGetUniformLocation(mShaderProgram, "projection");
      mPositionLocation     = glGetAttribLocation(mShaderProgram, "position");
      mColorLocation        = glGetAttribLocation(mShaderProgram, "color");
      //
      mPointSizeLocation    = glGetUniformLocation(mShaderProgram, "pointSize");
      mColorModeLocation    = glGetUniformLocation(mShaderProgram, "colorMode");
      mColorMapParamLocation = glGetUniformLocation(mShaderProgram, "colorMapParam");
      mSingleColorLocation  = glGetUniformLocation(mShaderProgram, "singleColor");

      // Initialze Vertex Array Object
      glGenVertexArrays(1, &mVertexArrayObject);
      glBindVertexArray(mVertexArrayObject);

      initTexture();
      updateTexture();

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

      if (mIsColorMapIndexModified)
        updateTexture();

      glUseProgram(mShaderProgram);
      glUniformMatrix4fv(mModelViewLocation, 1, GL_FALSE, &(inModelView[0]));
      glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, &(inProjection[0]));

      glUniform4fv(mModelFitLocation, 1, mModelFitParam);
      glUniform1f(mPointSizeLocation, mPointSize);
      glUniform1i(mColorModeLocation, mColorMode);
      glUniform4fv(mColorMapParamLocation, 1, mColorMapParam);
      glUniform4fv(mSingleColorLocation, 1, mSingleColor);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_1D, mColorMapTexture);

      glBindVertexArray(mVertexArrayObject);
      glDrawArrays(GL_POINTS, 0, (GLsizei )mDataNum);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_1D, 0);
    }
    // -------------------------------------------------------------------------
    // getModelFitParam
    // -------------------------------------------------------------------------
    const float *getModelFitParam()
    {
      return mModelFitParam;
    }
    // -------------------------------------------------------------------------
    // setModelFitParam
    // -------------------------------------------------------------------------
    void setModelFitParam(const GLfloat inParm[4])
    {
      for (int i = 0; i < 4; i++)
        mModelFitParam[i] = inParm[i];
    }
    // -------------------------------------------------------------------------
    // getColorMapAxis
    // -------------------------------------------------------------------------
    int getColorMapAxis()
    {
      return mColorMapParam[0];
    }
    // -------------------------------------------------------------------------
    // setColorMapAxis
    // -------------------------------------------------------------------------
    void setColorMapAxis(int inMapAxis)
    {
      mColorMapParam[0] = inMapAxis;
    }
    // -------------------------------------------------------------------------
    // getColorMapOffset
    // -------------------------------------------------------------------------
    float getColorMapOffset()
    {
      return mColorMapParam[1];
    }
    // -------------------------------------------------------------------------
    // setColorMapOffset
    // -------------------------------------------------------------------------
    void setColorMapOffset(float inOffset)
    {
      mColorMapParam[1] = inOffset;
    }
    // -------------------------------------------------------------------------
    // getColorMapGain
    // -------------------------------------------------------------------------
    float getColorMapGain()
    {
      return mColorMapParam[2];
    }
    // -------------------------------------------------------------------------
    // setColorMapGain
    // -------------------------------------------------------------------------
    void setColorMapGain(float inGain)
    {
      mColorMapParam[2] = inGain;
    }
    // -------------------------------------------------------------------------
    // getColorMapUnmapMode
    // -------------------------------------------------------------------------
    // Mode = 0 : display nothing when the colormap is out of range (un-mapped)
    // Mode = 1 : display using single color when the colormap is out of range
    //
    int getColorMapUnmapMode()
    {
      return mColorMapParam[3];
    }
    // -------------------------------------------------------------------------
    // setColorMapUnmapMode
    // -------------------------------------------------------------------------
    void setColorMapUnmapMode(int inMode)
    {
      mColorMapParam[3] = inMode;
    }
    // -------------------------------------------------------------------------
    // getPointSize
    // -------------------------------------------------------------------------
    float getPointSize()
    {
      return mPointSize;
    }
    // -------------------------------------------------------------------------
    // setPointSize
    // -------------------------------------------------------------------------
    void setPointSize(float inSize)
    {
      mPointSize = inSize;
    }
    // -------------------------------------------------------------------------
    // getColorMode
    // -------------------------------------------------------------------------
    float getColorMode()
    {
      return mColorMode;
    }
    // -------------------------------------------------------------------------
    // setColorMode
    // -------------------------------------------------------------------------
    void setColorMode(int inMode)
    {
      if (inMode < 0)
        inMode = 0;
      if (inMode > 2)
        inMode = 2;
      mColorMode = inMode;
    }
    // -------------------------------------------------------------------------
    // getSingleColor
    // -------------------------------------------------------------------------
    const float *getSingleColor()
    {
      return mSingleColor;
    }
    // -------------------------------------------------------------------------
    // setSingleColor
    // -------------------------------------------------------------------------
    void setSingleColor(const float inColor[])
    {
      mSingleColor[0] = inColor[0];
      mSingleColor[1] = inColor[1];
      mSingleColor[2] = inColor[2];
      mSingleColor[3] = inColor[3];
    }
    // -------------------------------------------------------------------------
    // getColorMapIndex
    // -------------------------------------------------------------------------
    ibc::image::ColorMap::ColorMapIndex getColorMapIndex()
    {
      return mColorMapIndex;
    }
    // -------------------------------------------------------------------------
    // setColorMapIndex
    // -------------------------------------------------------------------------
    void setColorMapIndex(ibc::image::ColorMap::ColorMapIndex inIndex)
    {
      mColorMapIndex = inIndex;
      mIsColorMapIndexModified = true;
    }
    // -------------------------------------------------------------------------
    // getColorMapRepeatNum
    // -------------------------------------------------------------------------
    int getColorMapRepeatNum()
    {
      return mColorMapRepeatNum;
    }
    // -------------------------------------------------------------------------
    // setColorMapRepeatNum
    // -------------------------------------------------------------------------
    void setColorMapRepeatNum(int inNum)
    {
      mColorMapRepeatNum = inNum;
      mIsColorMapIndexModified = true;
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
    bool  mIsColorMapIndexModified;

    void  *mDataPtr;
    size_t  mDataNum;
    size_t  mDataSize;

    float mPointSize;
    GLint  mColorMode;
    GLfloat mSingleColor[4];

    ibc::image::ColorMap::ColorMapIndex mColorMapIndex;
    int   mColorMapRepeatNum;
    size_t  mColorMapSize;
    GLuint  mColorMapTexture;

    GLfloat mModelFitParam[4];
    GLfloat mColorMapParam[4];

    GLuint mShaderProgram;
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;

    GLint mModelFitLocation;
    GLint mModelViewLocation;
    GLint mProjectionLocation;
    GLint mPositionLocation;
    GLint mColorLocation;
    GLint mColorModeLocation;
    GLint mColorMapParamLocation;
    GLint mSingleColorLocation;

    GLint mPointSizeLocation;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // initTexture
    // -------------------------------------------------------------------------
    void initTexture()
    {
      glActiveTexture(GL_TEXTURE0);
      glEnable(GL_TEXTURE_1D);
      glGenTextures(1, &mColorMapTexture);

      glBindTexture(GL_TEXTURE_1D, mColorMapTexture);
      glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, (GLsizei )mColorMapSize, 0,
                  GL_RGB, GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      GLint location = glGetUniformLocation(mShaderProgram, "colorMapTexure");
      glUseProgram(mShaderProgram);
      glUniform1i(location, 0); // since we are using GL_TEXTURE0
      glBindTexture(GL_TEXTURE_1D, 0);
    }
    // -------------------------------------------------------------------------
    // updateTexture
    // -------------------------------------------------------------------------
    void updateTexture()
    {
      unsigned char *colorMap = new unsigned char[mColorMapSize * 3];
      //getMonoMap <- gamma
      ibc::image::ColorMap::getColorMap(mColorMapIndex, (GLsizei )mColorMapSize, colorMap,
                              mColorMapRepeatNum, 1.0, 0);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_1D, mColorMapTexture);
      glTexSubImage1D(GL_TEXTURE_1D, 0, 0, (GLsizei )mColorMapSize, GL_RGB, GL_UNSIGNED_BYTE, colorMap);
      glBindTexture(GL_TEXTURE_1D, 0);
      delete colorMap;
      mIsColorMapIndexModified = false;
    }
    // -------------------------------------------------------------------------
    // disposeTexture
    // -------------------------------------------------------------------------
    void disposeTexture()
    {
    }
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
