// =============================================================================
//  gl_view.h
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
  \file     ibc/qt/gl_view.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/04/29
  \brief    Header file for ImageViewBase widget
*/

#ifndef IBC_QT_GL_VIEW_H_
#define IBC_QT_GL_VIEW_H_

// Macros ----------------------------------------------------------------------
#include "ibc/base/types.h"
#define IBC_GET_QOPENGL_API_i(major, minor)   QOpenGLFunctions_##major##_##minor##_Core
#define IBC_GET_QOPENGL_API(major, minor)     IBC_GET_QOPENGL_API_i(major,minor)
#define IBC_QOPENGL_CLASS_NAME                IBC_GET_QOPENGL_API(LIBIBC_OPENGL_MAJOR_VER,LIBIBC_OPENGL_MINOR_VER)

// Includes --------------------------------------------------------------------
#include <stdlib.h>
#include <QObject>
#include <QtWidgets>
#include <QOpenGLWidget>
#include IBC_MACRO_TOSTRING(IBC_QOPENGL_CLASS_NAME)
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
  class GLView : public QOpenGLWidget, protected IBC_QOPENGL_CLASS_NAME
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
      {
        // We need to do a OpenGL version sanity check here
        QOpenGLContext *context = QOpenGLContext::currentContext();
        if (context == NULL)
        {
          qCritical("Error: QOpenGLContext() returned null");
          exit(1);
        }
        if (context->versionFunctions<IBC_QOPENGL_CLASS_NAME>() == NULL)
        {
          qCritical("Error: Could not obtain " IBC_MACRO_TOSTRING(IBC_QOPENGL_CLASS_NAME));
          qCritical("The system doesn't support the required OpenGL version");
          exit(1);
        }
      }
      initializeOpenGLFunctions();

      mRendererStr = glGetString(GL_RENDERER);
      mVersionStr = glGetString(GL_VERSION);
      qInfo("Renderer: %s", mRendererStr);
      qInfo("OpenGL version supported %s", mVersionStr);

      glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

      glEnable(GL_CCW);
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);

      glClearDepth(1.0);
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
