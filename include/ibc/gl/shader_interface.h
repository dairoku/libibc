// =============================================================================
//  shader_interface.h
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
  \file     ibc/gl/models/shader_interface.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/26
  \brief    Header file for defining ShaderInterface
*/

#ifndef IBC_GL_SHADER_INTERFACE_H_
#define IBC_GL_SHADER_INTERFACE_H_

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gl
 {
  // ---------------------------------------------------------------------------
  // Shader interface class
  // ---------------------------------------------------------------------------
  class ShaderInterface
  {
  public:
    virtual bool initShader() = 0;
    virtual void disposeShader() = 0;
    virtual GLuint  getShaderProgram() = 0;
    virtual GLuint  getVertexShader() = 0;
    virtual GLuint  getFragmentShader() = 0;
    virtual GLuint  geGeometryShader() = 0;
  };
 };
};

#endif  // #ifdef IBC_GL_SHADER_INTERFACE_H_
