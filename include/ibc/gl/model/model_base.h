// =============================================================================
//  model_base.h
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
  class ModelBase : public virtual ibc::gl::ModelInterface, protected IBC_QOPENGL_CLASS_NAME
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
