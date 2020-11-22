// =============================================================================
//  surface_points.h
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
  \file     ibc/gl/models/surface_points.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/31
  \brief    Header file for the SurfacePoints model
*/

#ifndef IBC_GL_MODEL_SURFACE_PLOT_H_
#define IBC_GL_MODEL_SURFACE_PLOT_H_

// Includes --------------------------------------------------------------------
#include <math.h>
#include "ibc/gl/model/model_base.h"
#include "ibc/gl/utils.h"
#include "ibc/image/color_map.h"

// Namespace -------------------------------------------------------------------
//namespace ibc::gl::model // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace model
{
  // ---------------------------------------------------------------------------
  // SurfacePoints
  // ---------------------------------------------------------------------------
  class SurfacePoints : public virtual ibc::gl::model::ModelBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // SurfacePoints
    // -------------------------------------------------------------------------
    SurfacePoints()
    {
      mVertexData = NULL;

      mIsDataFormatUpdated  = false;
      mIsDataModified       = false;
      mIsColorMapModified   = false;

      mIsVBOInitialized             = false;
      mIsColoMapTextureInitialized  = false;

      mDataPtr     = NULL;
      mWidth       = 0;
      mHeight      = 0;
      mDataType    = GL_NONE;
      mNumPoints   = 0;
      mDataSize    = 0;
      mColorMapIndex = ibc::image::ColorMap::CMIndex_Rainbow;
      mColorMapSize = 0;
    }
    // -------------------------------------------------------------------------
    // ~SurfacePoints
    // -------------------------------------------------------------------------
    virtual ~SurfacePoints()
    {
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // setDataPtr
    // -------------------------------------------------------------------------
    void setDataPtr(void *inDataPtr, size_t inWidth, size_t inHeight, GLenum inType)
    {
      if (mWidth != inWidth || mHeight != inHeight || mDataType != inType)
        mIsDataFormatUpdated = true;

      mDataPtr    = inDataPtr;
      mWidth      = inWidth;
      mHeight     = inHeight;
      mDataType   = inType;
      mNumPoints  = mWidth * mHeight;
      mDataSize = ibc::gl::Utils::sizeofGLDataType(mDataType) * mNumPoints;
      if (mDataType == GL_BYTE || mDataType == GL_UNSIGNED_BYTE)
        mColorMapSize = 256;
      else
        mColorMapSize = 65536;
      //
      mIsDataModified = true;
    }
    // -------------------------------------------------------------------------
    // setColorMapIndex
    // -------------------------------------------------------------------------
    void setColorMapIndex(ibc::image::ColorMap::ColorMapIndex inColorMapIndex)
    {
      mColorMapIndex = inColorMapIndex;
      mIsColorMapModified = true;
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
      mDataSizeLocation         = glGetUniformLocation(mShaderProgram, "dataSize");
      mZGainLocation            = glGetUniformLocation(mShaderProgram, "zGain");
      mZOffsetLocation          = glGetUniformLocation(mShaderProgram, "zOffset");
      mZClampLocation           = glGetUniformLocation(mShaderProgram, "zClamp");
      mIntensityMaxLocation     = glGetUniformLocation(mShaderProgram, "intensityMax");
      mIntensityGainLocation    = glGetUniformLocation(mShaderProgram, "intensityGain");
      mIntensityOffsetLocation  = glGetUniformLocation(mShaderProgram, "intensityOffset");
      mIntensityClampLocation   = glGetUniformLocation(mShaderProgram, "intensityClamp");
      mLightSourceLocation      = glGetUniformLocation(mShaderProgram, "lightSource");
      mMaterialLocation         = glGetUniformLocation(mShaderProgram, "material");
      mModelViewLocation        = glGetUniformLocation(mShaderProgram, "modelview");
      mProjectionLocation       = glGetUniformLocation(mShaderProgram, "projection");
      GLint intensityLocation   = glGetAttribLocation(mShaderProgram, "intensity");

      // Initialze Vertex Array Object
      glGenVertexArrays(1, &mVertexArrayObject);
      glBindVertexArray(mVertexArrayObject);

      if (mIsDataFormatUpdated)
      {
        initVBO();
        initColoMapTexture();
        mIsDataFormatUpdated = false;
      }

      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(intensityLocation);
      glVertexAttribPointer(intensityLocation, 1, GL_UNSIGNED_BYTE, GL_FALSE, 0, NULL);

      // Initialize Texture
      glEnable(GL_TEXTURE_1D);
      glGenTextures(1, &mTexture);
      //glCreateTextures(GL_TEXTURE_1D, 1, &mTexture);
      //glTextureStorage1D(mTexture, 1, GL_RGB8, 256);
      glBindTexture(GL_TEXTURE_1D, mTexture);

      unsigned char *colorMap = new unsigned char[256 * 3];
      ibc::image::ColorMap::getColorMap(ibc::image::ColorMap::CMIndex_Rainbow,
                                        256, colorMap);
      glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, colorMap);
      //glTextureSubImage1D(mTexture, 0, 0, 256, GL_RGB, GL_UNSIGNED_BYTE, colorMap);
      delete colorMap;

      return true;
    }
    // -------------------------------------------------------------------------
    // disposeModel
    // -------------------------------------------------------------------------
    virtual void disposeModel()
    {
      disposeColoMapTexture();
      disposeVBO();
    }
    // -------------------------------------------------------------------------
    // drawModel
    // -------------------------------------------------------------------------
    virtual void drawModel(const GLfloat inModelView[16], const GLfloat inProjection[16])
    {
      if (mIsDataFormatUpdated)
      {
        initVBO();
        initColoMapTexture();
        mIsDataFormatUpdated = false;
      }
      else
      {
        if (mIsDataModified)
          updateVBO();
        if (mIsColorMapModified)
          updateColoMapTexture();
      }

      glUseProgram(mShaderProgram);

      glUniform2f(mDataSizeLocation, mWidth, mHeight);
      glUniform1f(mZGainLocation,   1.0 / mColorMapSize);
      glUniform1f(mZOffsetLocation, 0.0);
      glUniform2f(mZClampLocation, 0.0, 1.0);
      glUniform1f(mIntensityMaxLocation,    mColorMapSize);
      glUniform1f(mIntensityGainLocation,   1.0);
      glUniform1f(mIntensityOffsetLocation, 0.0);
      glUniform2f(mIntensityClampLocation, 0.0, mColorMapSize);
      glUniform4f(mLightSourceLocation, 0.0, 0.0, 100.0, 1.0);
      glUniform1i(mMaterialLocation, 0);
      glUniformMatrix4fv(mModelViewLocation, 1, GL_FALSE, &(inModelView[0]));
      glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, &(inProjection[0]));

      glBindVertexArray(mVertexArrayObject);
      glDrawArrays(GL_POINTS, 0, mNumPoints);
    }

  protected:
    // Member variables --------------------------------------------------------
    bool  mIsDataFormatUpdated;
    bool  mIsDataModified;
    bool  mIsColorMapModified;

    bool  mIsVBOInitialized;
    bool  mIsColoMapTextureInitialized;

    void  *mDataPtr;
    GLenum  mDataType;
    size_t  mWidth, mHeight;
    size_t  mNumPoints;
    size_t  mDataSize;
    ibc::image::ColorMap::ColorMapIndex mColorMapIndex;
    int mColorMapSize;

    unsigned char  *mVertexData;

    GLuint mShaderProgram;

    GLuint  mVertexArrayObject;
    GLuint  mVertexBufferObject;
    GLuint  mTexture;

    GLint mDataSizeLocation;
    GLint mZGainLocation;
    GLint mZOffsetLocation;
    GLint mZClampLocation;
    GLint mIntensityMaxLocation;
    GLint mIntensityGainLocation;
    GLint mIntensityOffsetLocation;
    GLint mIntensityClampLocation;
    GLint mLightSourceLocation;
    GLint mMaterialLocation;

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
    // -------------------------------------------------------------------------
    // initColoMapTexture
    // -------------------------------------------------------------------------
    void initColoMapTexture()
    {
      if (mIsColoMapTextureInitialized)
        disposeColoMapTexture();
      //
      glEnable(GL_TEXTURE_1D);
      glGenTextures(1, &mTexture);
      //glCreateTextures(GL_TEXTURE_1D, 1, &mTexture);
      //glTextureStorage1D(mTexture, 1, GL_RGB8, mColorMapSize);
      glBindTexture(GL_TEXTURE_1D, mTexture);
      mIsColoMapTextureInitialized = true;
      //
      updateColoMapTexture();
    }
    // -------------------------------------------------------------------------
    // updateColoMapTexture
    // -------------------------------------------------------------------------
    void updateColoMapTexture()
    {
      unsigned char *colorMap = new unsigned char[mColorMapSize * 3];
      ibc::image::ColorMap::getColorMap(ibc::image::ColorMap::CMIndex_Rainbow,
                                        mColorMapSize, colorMap);
      //glTextureSubImage1D(mTexture, 0, 0, mColorMapSize, GL_RGB, GL_UNSIGNED_BYTE, colorMap);
      glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, colorMap);
      delete colorMap;
      mIsColorMapModified = false;
    }
    // -------------------------------------------------------------------------
    // disposeColoMapTexture
    // -------------------------------------------------------------------------
    void disposeColoMapTexture()
    {
      if (mIsColoMapTextureInitialized == false)
        return;
      glBindTexture(GL_TEXTURE_1D, 0);
      glDeleteTextures(1, &mTexture);
      mIsColoMapTextureInitialized = false;
    }
  };
};};};

#endif  // #ifdef IBC_GL_MODEL_SURFACE_PLOT_H_
