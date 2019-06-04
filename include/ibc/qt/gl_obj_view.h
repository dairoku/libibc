// =============================================================================
//  gl_obj_view.h
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
  \file     ibc/qt/gl_obj_view.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/04/30
  \brief    Header file for the OpenGL Object Viewer widget
*/

#ifndef IBC_QT_GL_OBJ_VIEW_H_
#define IBC_QT_GL_OBJ_VIEW_H_

// Includes --------------------------------------------------------------------
#include <QtWidgets>
#include "ibc/gl/matrix.h"
#include "ibc/gl/utils.h"
#include "ibc/gl/trackball.h"
#include "ibc/qt/gl_view.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace qt
 {
  // ---------------------------------------------------------------------------
  // GLView class
  // ---------------------------------------------------------------------------
  class GLObjView : virtual public GLView
  {
    Q_OBJECT

  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // GLObjView
    // -------------------------------------------------------------------------
    GLObjView(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags())
      : GLView(parent, f)
    {
      mProjection.setIdentity();

      mCameraFoV = 30.0;
    }
    // -------------------------------------------------------------------------
    // ~GLObjView
    // -------------------------------------------------------------------------
    virtual ~GLObjView()
    {
    }

  protected:
    // Member variables --------------------------------------------------------
    GLfloat mWidth, mHeight;
    GLfloat mCameraFoV;

    ibc::gl::TrackballBase<GLfloat> mTrackball;
    ibc::gl::MatrixBase<GLfloat> mProjection;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // mousePressEvent
    // -------------------------------------------------------------------------
    virtual void  mousePressEvent(QMouseEvent *event)
    {
      unsigned int  button = 0;
      if (event->button() == Qt::LeftButton)
        button = 1;

      mTrackball.startTrackingMouse(event->pos().x(), event->pos().y(),
                                    mWidth, mHeight, button);
    }
    // -------------------------------------------------------------------------
    // mouseReleaseEvent
    // -------------------------------------------------------------------------
    virtual void  mouseReleaseEvent(QMouseEvent *event)
    {
      UNUSED(event);
      //
      mTrackball.stopTrackingMouse();
    }
    // -------------------------------------------------------------------------
    // mouseMoveEvent
    // -------------------------------------------------------------------------
    virtual void  mouseMoveEvent(QMouseEvent *event)
    {
      mTrackball.trackMouse(event->pos().x(), event->pos().y());
      update();
    }
    // -------------------------------------------------------------------------
    // wheelEvent
    // -------------------------------------------------------------------------
    void wheelEvent(QWheelEvent *wEvent) override
    {
      QPoint delta = wEvent->angleDelta();
      int direction = 1;
      if (delta.x() < 0 || delta.y() < 0)
        direction = 0;
      mTrackball.mouseWheel(direction, delta.x(), delta.y());
      glUpdaetProjection();
      update();
    }
    // OpenGL related functions ------------------------------------------------
    // -------------------------------------------------------------------------
    // resizeGL (inherited from QOpenGLWidget)
    // -------------------------------------------------------------------------
    virtual void  resizeGL(int w, int h)
    {
      makeCurrent();

      mWidth = w;
      mHeight = h;
      glViewport(0, 0, mWidth, mHeight);
      glUpdaetProjection();
    }
    // -------------------------------------------------------------------------
    // glUpdaetProjection
    // -------------------------------------------------------------------------
    virtual void  glUpdaetProjection()
    {
//      mProjection = ibc::gl::Utils::perspective<GLfloat>(mCameraFoV, mWidth / (GLfloat )mHeight, 1.0, 100);
      mProjection = ibc::gl::Utils::perspective<GLfloat>(mCameraFoV, mWidth / (GLfloat )mHeight, 0.1, 100);
      ibc::gl::MatrixBase<GLfloat>  translate, scale;
      translate.setTranslationMatrix(0.0, 0.0, -5);
      //translate.setTranslationMatrix(0.0, 0.0, -2);
      mProjection *= translate;
      //scale.setScaleMatrix(mScaleFactor, mScaleFactor, mScaleFactor);
      //mProjection *= scale;
    }
    // -------------------------------------------------------------------------
    // paintGL (inherited from QOpenGLWidget)
    // -------------------------------------------------------------------------
    virtual void  paintGL()
    {
      // Update ModelView and Projection matrix
      mTrackball.getGLRotationMatrix(mGLModelView);
      mProjection.getTransposedMatrix(mGLProjection);

      ibc::qt::GLView::paintGL();
    }
  };
 };
};

#endif  // #ifdef IBC_QT_GL_OBJ_VIEW_H_

