// =============================================================================
//  gl_view.h
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
  \file     ibc/qt/gl_view.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/04/29
  \brief    Header file for ImageViewBase widget
*/

#ifndef IBC_QT_GL_VIEW_H_
#define IBC_QT_GL_VIEW_H_

// Includes --------------------------------------------------------------------
#include <QtWidgets>
#include <QOpenGLWidget>
//#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include "ibc/gl/matrix.h"
#include "ibc/gl/model_interface.h"
#include "ibc/gl/shader_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace qt
 {
  // ---------------------------------------------------------------------------
  // GLView class
  // ---------------------------------------------------------------------------
//class GLView : public QOpenGLWidget, protected QOpenGLExtraFunctions
  class GLView : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
  {
    Q_OBJECT

  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // GLView
    // -------------------------------------------------------------------------
    GLView(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags())
      : QOpenGLWidget(parent, f)
    {
      // Initialize mModelView and mProjection (Set identity matrix)
      ibc::gl::MatrixBase<GLfloat> matrix;
      matrix.setIdentity();
      matrix.getTransposedMatrix(mGLModelView);
      matrix.getTransposedMatrix(mGLProjection);
    }
    // -------------------------------------------------------------------------
    // ~GLView
    // -------------------------------------------------------------------------
    virtual ~GLView()
    {
      disopseGL();
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // addModel
    // -------------------------------------------------------------------------
    void  addModel(ibc::gl::ModelInterface *inModel)
    {
      mModelList.push_back(inModel);
    }
    // -------------------------------------------------------------------------
    // removeModel
    // -------------------------------------------------------------------------
    void  removeModel(ibc::gl::ModelInterface *inModel)
    {
      auto it = std::find(mModelList.begin(), mModelList.end(), inModel);
      if (it == mModelList.end())
        return;
      mModelList.erase(it);
    }
    // -------------------------------------------------------------------------
    // addShader
    // -------------------------------------------------------------------------
    void  addShader(ibc::gl::ShaderInterface *inShader)
    {
      mShaderList.push_back(inShader);
    }
    // -------------------------------------------------------------------------
    // removeShader
    // -------------------------------------------------------------------------
    void  removeShader(ibc::gl::ShaderInterface *inShader)
    {
      auto it = std::find(mShaderList.begin(), mShaderList.end(), inShader);
      if (it == mShaderList.end())
        return;
      mShaderList.erase(it);
    }

  protected:
    // Member variables --------------------------------------------------------
    const GLubyte *mRendererStr;
    const GLubyte *mVersionStr;

    std::vector<ibc::gl::ShaderInterface *>   mShaderList;
    std::vector<ibc::gl::ModelInterface *>    mModelList;

    GLfloat mGLModelView[16], mGLProjection[16];


    // OpenGL related functions ------------------------------------------------
    // -------------------------------------------------------------------------
    // initializeGL (inherited from QOpenGLWidget)
    // -------------------------------------------------------------------------
    virtual void  initializeGL()
    {
      makeCurrent();
      initializeOpenGLFunctions();

      mRendererStr = glGetString(GL_RENDERER);
      mVersionStr = glGetString(GL_VERSION);
      printf("Renderer: %s\n", mRendererStr);
      printf("OpenGL version supported %s\n", mVersionStr);

      glClearColor(0.3, 0.3, 0.3, 0.0);

      glEnable(GL_CCW);
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);

      glClearDepthf(1.0);
      glDepthFunc(GL_LESS);
      glEnable(GL_DEPTH_TEST);

      for (auto it = mShaderList.begin(); it != mShaderList.end(); it++)
        (*it)->initShader();

      for (auto it = mModelList.begin(); it != mModelList.end(); it++)
        (*it)->initModel();
    }
    // -------------------------------------------------------------------------
    // disopseGL
    // -------------------------------------------------------------------------
    virtual void  disopseGL()
    {
      makeCurrent();

      for (auto it = mModelList.begin(); it != mModelList.end(); it++)
        (*it)->disposeModel();

      for (auto it = mShaderList.begin(); it != mShaderList.end(); it++)
        (*it)->disposeShader();
    }
    // -------------------------------------------------------------------------
    // resizeGL (inherited from QOpenGLWidget)
    // -------------------------------------------------------------------------
    virtual void  resizeGL(int w, int h)
    {
      makeCurrent();

      glViewport(0, 0, w, h);
      //glLoadIdentity();
      //glOrtho(-w/300.0, w/300.0, -h/300.0, h/300.0, -1.0, 1.0);
    }
    // -------------------------------------------------------------------------
    // paintGL (inherited from QOpenGLWidget)
    // -------------------------------------------------------------------------
    virtual void  paintGL()
    {
      makeCurrent();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      for (auto it = mModelList.begin(); it != mModelList.end(); it++)
        if ((*it)->isEnabled())
          (*it)->drawModel(mGLModelView, mGLProjection);
    }
  };
 };
};

#endif  // #ifdef IBC_QT_IMAGE_VIEW_H_

