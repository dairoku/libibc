// =============================================================================
//  shader_base.h
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
  \file     ibc/gl/models/shader_base.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/24
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
*/

#ifndef IBC_GL_SHADER_SHADER_BASE_H_
#define IBC_GL_SHADER_SHADER_BASE_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/shader_interface.h"


// Namespace -------------------------------------------------------------------
namespace ibc::gl::shader // <- nested namespace (C++17)
{
  // ---------------------------------------------------------------------------
  // ShaderBase
  // ---------------------------------------------------------------------------
  class ShaderBase : public virtual ibc::gl::ShaderInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ShaderBase
    // -------------------------------------------------------------------------
    ShaderBase(const char *inVetexShaderStr     = NULL,
               const char *inFragmentShaderStr  = NULL,
               const char *inGeometoryShaderStr = NULL)
    {
      mVertexShader     = 0;
      mFragmentShader   = 0;
      mGeometryShader  = 0;
      mShaderProgram    = 0;

      mVertexShaderStr     = inVetexShaderStr;
      mFragmentShaderStr  = inFragmentShaderStr;
      mGeometryShaderStr = inGeometoryShaderStr;
    }
    // -------------------------------------------------------------------------
    // ~ShaderBase
    // -------------------------------------------------------------------------
    virtual ~ShaderBase()
    {
    }
    // Member functions -------------------------------------------------------
    // -------------------------------------------------------------------------
    // initShader
    // -------------------------------------------------------------------------
    virtual bool initShader()
    {
      if (initCheck() == false)
        return false;

      mShaderProgram = glCreateProgram();
      if (mShaderProgram == 0)
        return false;

      const GLchar  *shaderStr[1];
      shaderStr[0] = getVertexShaderStr();
      if (shaderStr[0] != NULL)
      {
        mVertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(mVertexShader, 1, shaderStr, NULL);
        glCompileShader(mVertexShader);
        glAttachShader(mShaderProgram, mVertexShader);
      }

      shaderStr[0] = getFragmentShaderStr();
      if (shaderStr[0] != NULL)
      {
        mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(mFragmentShader, 1, shaderStr, NULL);
        glCompileShader(mFragmentShader);
        glAttachShader(mShaderProgram, mFragmentShader);
      }

      shaderStr[0] = getGeometryShaderStr();
      if (shaderStr[0] != NULL)
      {
        mGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(mGeometryShader, 1, shaderStr, NULL);
        glCompileShader(mGeometryShader);
        glAttachShader(mShaderProgram, mGeometryShader);
      }

      glLinkProgram(mShaderProgram);

      return true;
    }
    // -------------------------------------------------------------------------
    // disposeShader
    // -------------------------------------------------------------------------
    virtual void disposeShader()
    {
    }
    // -------------------------------------------------------------------------
    // getShaderProgram
    // -------------------------------------------------------------------------
    virtual GLuint  getShaderProgram()
    {
      return mShaderProgram;
    }
    // -------------------------------------------------------------------------
    // getVertexShader
    // -------------------------------------------------------------------------
    virtual GLuint  getVertexShader()
    {
      return mVertexShader;
    }
    // -------------------------------------------------------------------------
    // getFragmentShader
    // -------------------------------------------------------------------------
    virtual GLuint  getFragmentShader()
    {
      return mFragmentShader;
    }
    // -------------------------------------------------------------------------
    // geGeometryShader
    // -------------------------------------------------------------------------
    virtual GLuint  geGeometryShader()
    {
      return mGeometryShader;
    }

  protected:
    // Member variables --------------------------------------------------------
    GLuint mVertexShader;
    const char  *mVertexShaderStr;
    GLuint mFragmentShader;
    const char  *mFragmentShaderStr;
    GLuint mGeometryShader;
    const char  *mGeometryShaderStr;
    GLuint mShaderProgram;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // getVertexShaderStr
    // -------------------------------------------------------------------------
    virtual const char *getVertexShaderStr()
    {
      return mVertexShaderStr;
    }
    // -------------------------------------------------------------------------
    // getFragmentShaderStr
    // -------------------------------------------------------------------------
    virtual const char *getFragmentShaderStr()
    {
      return mFragmentShaderStr;
    }
    // -------------------------------------------------------------------------
    // getFragmentShaderStr
    // -------------------------------------------------------------------------
    virtual const char *getGeometryShaderStr()
    {
      return mGeometryShaderStr;
    }
    // -------------------------------------------------------------------------
    // initCheck
    // -------------------------------------------------------------------------
    virtual bool initCheck()
    {
      if (mShaderProgram != 0)
        return false;
      if (getVertexShaderStr()    == NULL &&
          getFragmentShaderStr()  == NULL &&
          getGeometryShaderStr()  == NULL)
        return false;
      return true;
    }
  };
};

#endif  // #ifdef IBC_GL_SHADER_SHADER_BASE_H_
