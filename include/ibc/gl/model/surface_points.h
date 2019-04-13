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
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
*/

#ifndef IBC_GL_MODEL_SURFACE_PLOT_H_
#define IBC_GL_MODEL_SURFACE_PLOT_H_

// Includes --------------------------------------------------------------------
#include <math.h>
#include "ibc/gl/model_interface.h"
#include "ibc/gl/shader_interface.h"
#include "ibc/image/color_map.h"

// Namespace -------------------------------------------------------------------
namespace ibc::gl::model // <- nested namespace (C++17)
{
  // ---------------------------------------------------------------------------
  // SurfacePoints
  // ---------------------------------------------------------------------------
  class SurfacePoints : public virtual ibc::gl::ModelInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // SurfacePoints
    // -------------------------------------------------------------------------
    SurfacePoints()
    {
      mShaderInterface = NULL;
      mVertexData = NULL;
      
      mWidth = 640;
      mHeight = 480;
      
      mColorMap = new unsigned char[mColorMapNum * 3];
      ibc::image::ColorMap::getColorMap(ibc::image::ColorMap::CMIndex_Rainbow,
                                        mColorMapNum, mColorMap);
    }
    // -------------------------------------------------------------------------
    // ~SurfacePoints
    // -------------------------------------------------------------------------
    virtual ~SurfacePoints()
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
      mNumPoints = mWidth * mHeight;
      mVertexData = new unsigned char[mNumPoints];
      size_t  dataSize = sizeof(unsigned char) * mNumPoints;
      double xPitch = 1.0 * 2.0 / (double )mWidth;
      
      for (int i = 0; i < mHeight; i++)
        for (int j = 0; j < mWidth; j++)
        {
          double x = -1.0 + xPitch * j;
          double y = -1.0 + xPitch * i;
          double k = (M_PI * 3.0) * (M_PI * 3.0);
          double z, d;
          if (x == 0 && y == 0)
            z = 1;
          else
          {
            d = sqrt(k*x*x + k*y*y);
            z = sin(d) / d;
          }
          int c = (int )(fabs(z + 0.1) *(mColorMapNum-1.0));
          if (c >= mColorMapNum)
            c = mColorMapNum - 1;
          if (c < 0)
            c = 0;
          //mVertexData[mWidth * i + j].position[0] = x;
          //mVertexData[mWidth * i + j].position[1] = y;
          //mVertexData[mWidth * i + j].position[2] = z;
          //mVertexData[mWidth * i + j].color[0] = mColorMap[c * 3 + 0] / 255.0;
          //mVertexData[mWidth * i + j].color[1] = mColorMap[c * 3 + 1] / 255.0;
          //mVertexData[mWidth * i + j].color[2] = mColorMap[c * 3 + 2] / 255.0;
          z = 255.0 * z;
          if (z > 255)
            z = 255;
          if (z < 0)
            z = 0;
          mVertexData[mWidth * i + j] = (unsigned char )z;
        }

      mShaderProgram = mShaderInterface->getShaderProgram();

      glGenVertexArrays(1, &mVertexArrayObject);
      glBindVertexArray(mVertexArrayObject);

      glGenBuffers(1, &mVertexBufferObject);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glBufferData(GL_ARRAY_BUFFER, dataSize, mVertexData, GL_STATIC_DRAW);

      mDataSizeLocation         = glGetUniformLocation(mShaderProgram, "dataSize");
      mZGainLocation            = glGetUniformLocation(mShaderProgram, "zGain");
      mZOffsetLocation          = glGetUniformLocation(mShaderProgram, "zOffset");
      mZClampLocation           = glGetUniformLocation(mShaderProgram, "zClamp");
      mIntensityMaxLocation     = glGetUniformLocation(mShaderProgram, "intensityMax");
      mIntensityGainLocation    = glGetUniformLocation(mShaderProgram, "intensityGain");
      mIntensityOffsetLocation  = glGetUniformLocation(mShaderProgram, "intensityOffset");
      mIntensityClampLocation   = glGetUniformLocation(mShaderProgram, "intensityClamp");
      //
      mModelViewLocation        = glGetUniformLocation(mShaderProgram, "modelview");
      mProjectionLocation       = glGetUniformLocation(mShaderProgram, "projection");
      //
      guint intensityLocation   = glGetAttribLocation(mShaderProgram, "intensity");

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glEnableVertexAttribArray(intensityLocation);
      glVertexAttribPointer (intensityLocation, 1, GL_UNSIGNED_BYTE, GL_FALSE, 0, NULL);

      glCreateTextures(GL_TEXTURE_1D, 1, &mTexture);
      glTextureStorage1D(mTexture, 1, GL_RGB8, 256);
      glBindTexture(GL_TEXTURE_1D, mTexture);

      unsigned char *colorMap = new unsigned char[256 * 3];
      ibc::image::ColorMap::getColorMap(ibc::image::ColorMap::CMIndex_Rainbow,
                                        256, colorMap);
      glTextureSubImage1D(mTexture, 0, 0, 256, GL_RGB, GL_UNSIGNED_BYTE, colorMap);
      delete colorMap;

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

      glUniform2f(mDataSizeLocation, mWidth, mHeight);
      glUniform1f(mZGainLocation,   1.0 / 255.0);
      glUniform1f(mZOffsetLocation, 0.0);
      glUniform2f(mZClampLocation, 0.0, 1.0);
      glUniform1f(mIntensityMaxLocation,    255.0);
      glUniform1f(mIntensityGainLocation,   1.0);
      glUniform1f(mIntensityOffsetLocation, 0.0);
      glUniform2f(mIntensityClampLocation, 0.0, 255.0);

      glUniformMatrix4fv(mModelViewLocation, 1, GL_FALSE, &(inModelView[0]));
      glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, &(inProjection[0]));
      glBindVertexArray(mVertexArrayObject);
      glDrawArrays(GL_POINTS, 0, mNumPoints);
    }

  protected:
    // Member variables --------------------------------------------------------
    size_t  mWidth, mHeight;
    size_t  mNumPoints;
    unsigned char  *mVertexData;

    const int   mColorMapNum = 65536;
    unsigned char *mColorMap;

    ibc::gl::ShaderInterface *mShaderInterface;
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

    GLint mModelViewLocation;
    GLint mProjectionLocation;
  };
};

#endif  // #ifdef IBC_GL_MODEL_SURFACE_PLOT_H_
