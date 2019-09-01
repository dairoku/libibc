// =============================================================================
//  model_base.h
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
  \file     ibc/gl/models/model_base.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/04/30
  \brief    Header file for the ModelBase class
*/

#ifndef IBC_GL_MODEL_BASE_H_
#define IBC_GL_MODEL_BASE_H_

// Includes --------------------------------------------------------------------
#include "ibc/base/types.h"
#include "ibc/gl/shader_interface.h"
#include "ibc/gl/model_interface.h"

// Namespace -------------------------------------------------------------------
//namespace ibc::gl::model // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace model
{
  // ---------------------------------------------------------------------------
  // ModelBase class
  // ---------------------------------------------------------------------------
#ifndef QT_VERSION
  class ModelBase : public virtual ibc::gl::ModelInterface
#else
//class ModelBase : public virtual ibc::gl::ModelInterface, protected QOpenGLExtraFunctions
  class ModelBase : public virtual ibc::gl::ModelInterface, protected QOpenGLFunctions_4_5_Core
#endif
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ModelBase
    // -------------------------------------------------------------------------
    ModelBase()
    {
      mShaderInterface = NULL;
      mIsEnabled = true;

#ifdef QT_VERSION
      mOpenGLFunctionsInitialized = false;
#endif
    }
    // -------------------------------------------------------------------------
    // ~ModelBase
    // -------------------------------------------------------------------------
    virtual ~ModelBase()
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
#ifdef QT_VERSION
      if (mOpenGLFunctionsInitialized == false)
      {
        initializeOpenGLFunctions();
        mOpenGLFunctionsInitialized = true;
      }
#endif
      return true;
    }
    // -------------------------------------------------------------------------
    // disposeModel
    // -------------------------------------------------------------------------
    virtual void disposeModel()
    {
    }
    // -------------------------------------------------------------------------
    // setEnabled
    // -------------------------------------------------------------------------
    virtual void setEnabled(bool inEnabled)
    {
      mIsEnabled = inEnabled;
    }
    // -------------------------------------------------------------------------
    // isEnabled
    // -------------------------------------------------------------------------
    virtual bool isEnabled()
    {
      return mIsEnabled;
    }
    // -------------------------------------------------------------------------
    // drawModel
    // -------------------------------------------------------------------------
    virtual void drawModel(const GLfloat inModelView[16], const GLfloat inProjection[16])
    {
      UNUSED(inModelView);
      UNUSED(inProjection);
    }

  protected:
    // Member variables --------------------------------------------------------
    ibc::gl::ShaderInterface *mShaderInterface;
    bool    mIsEnabled;
#ifdef QT_VERSION
    bool  mOpenGLFunctionsInitialized;
#endif
  };
};};};

#endif  // #ifdef IBC_GL_MODEL_BASE_H_
