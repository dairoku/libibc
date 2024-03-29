// =============================================================================
//  shader_base.h
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
  \file     ibc/gl/models/shader_base.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/24
  \brief    Header file for the ShaderBase class
*/

#ifndef IBC_GL_SHADER_SHADER_BASE_H_
#define IBC_GL_SHADER_SHADER_BASE_H_

// Includes --------------------------------------------------------------------
#include "ibc/gl/shader_interface.h"


// Namespace -------------------------------------------------------------------
//namespace ibc::gl::shader // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace shader
{
  // ---------------------------------------------------------------------------
  // ShaderBase
  // ---------------------------------------------------------------------------
#ifndef QT_VERSION
  class ShaderBase : public virtual ibc::gl::ShaderInterface
#else
  class ShaderBase : public virtual ibc::gl::ShaderInterface, protected IBC_QOPENGL_CLASS_NAME
#endif
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

#ifdef QT_VERSION
      mOpenGLFunctionsInitialized = false;
#endif
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
#ifdef QT_VERSION
      if (mOpenGLFunctionsInitialized == false)
      {
        initializeOpenGLFunctions();
        mOpenGLFunctionsInitialized = true;
      }
#endif
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
        if (handleShaderCompileError(mVertexShader, "mVertexShader"))
          return false;
        glAttachShader(mShaderProgram, mVertexShader);
      }

      shaderStr[0] = getFragmentShaderStr();
      if (shaderStr[0] != NULL)
      {
        mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(mFragmentShader, 1, shaderStr, NULL);
        glCompileShader(mFragmentShader);
        if (handleShaderCompileError(mFragmentShader, "mFragmentShader"))
          return false;
        glAttachShader(mShaderProgram, mFragmentShader);
      }

      shaderStr[0] = getGeometryShaderStr();
      if (shaderStr[0] != NULL)
      {
        mGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(mGeometryShader, 1, shaderStr, NULL);
        glCompileShader(mGeometryShader);
        if (handleShaderCompileError(mGeometryShader, "mGeometryShader"))
          return false;
        glAttachShader(mShaderProgram, mGeometryShader);
      }

      glLinkProgram(mShaderProgram);
      if (handleProgramError(mShaderProgram))
        return false;

      return true;
    }
    // -------------------------------------------------------------------------
    // handleShaderCompileError
    // -------------------------------------------------------------------------
    virtual bool handleShaderCompileError(GLuint inShader, const char *inShaderNameStr)
    {
      GLchar *log = getShaderCompileError(inShader);
      if (log == NULL)
        return false;

      printf("\nShader compile error: %s\n", inShaderNameStr);
      printf("%s\n", (char *)log);
      delete log;
      return false;
//    return true;
    }
    // -------------------------------------------------------------------------
    // handleProgramError
    // -------------------------------------------------------------------------
    virtual bool handleProgramError(GLuint inProgram)
    {
      GLchar *log = getProgramError(inProgram);
      if (log == NULL)
        return false;

      printf("\nProgram link error\n");
      printf("%s\n", (char *)log);
      delete log;
      return false;
//    return true;
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
#ifdef QT_VERSION
    bool  mOpenGLFunctionsInitialized;
#endif

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
    // -------------------------------------------------------------------------
    // getShaderCompileError
    // -------------------------------------------------------------------------
    GLchar *getShaderCompileError(GLuint inShader)
    {
      GLint result;
      glGetShaderiv(inShader, GL_COMPILE_STATUS, &result);
      if (result != GL_FALSE)
        return NULL;

      GLint logLength;
      GLsizei length;
      GLchar *log;

      glGetShaderiv(inShader, GL_INFO_LOG_LENGTH, &logLength);
      logLength++;  // <- just in case
      log = new GLchar[logLength];
      glGetShaderInfoLog(inShader, logLength, &length, log);

      return log;
    }
    // -------------------------------------------------------------------------
    // getProgramError
    // -------------------------------------------------------------------------
    GLchar *getProgramError(GLuint inProgram)
    {
      GLint result;
      glGetProgramiv(inProgram, GL_LINK_STATUS, &result);
      if (result != GL_FALSE)
        return NULL;

      GLint logLength;
      GLsizei length;
      GLchar *log;

      glGetProgramiv(inProgram, GL_INFO_LOG_LENGTH, &logLength);
      logLength++;  // <- just in case
      log = new GLchar[logLength];
      glGetProgramInfoLog(inProgram, logLength, &length, log);

      return log;
    }
  };
};};};

#endif  // #ifdef IBC_GL_SHADER_SHADER_BASE_H_
