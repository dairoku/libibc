// =============================================================================
//  model_interface.h
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
  \file     ibc/gl/models/model_interface.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/16
  \brief    Header file for defining the ModelInterface
*/

#ifndef IBC_GL_MODEL_INTERFACE_H_
#define IBC_GL_MODEL_INTERFACE_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/shader_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gl
 {
  // ---------------------------------------------------------------------------
  // Model interface class
  // ---------------------------------------------------------------------------
  class ModelInterface
  {
  public:
    virtual void setShader(ibc::gl::ShaderInterface *inShaderInterface) = 0;
    virtual ibc::gl::ShaderInterface *getShader() = 0;
    virtual bool initModel() = 0;
    virtual void disposeModel() = 0;
    virtual bool isEnabled() = 0;
    virtual void drawModel(const GLfloat inModelView[16], const GLfloat inProjection[16]) = 0;
  };
 };
};

#endif  // #ifdef IBC_GL_MODEL_INTERFACE_H_
