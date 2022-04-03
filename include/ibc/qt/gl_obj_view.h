// =============================================================================
//  gl_obj_view.h
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
      mTrackball.mouseWheel(direction, (GLfloat )delta.x(), (GLfloat )delta.y());
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
      mProjection = ibc::gl::Utils::perspective<GLfloat>(mCameraFoV, mWidth / (GLfloat )mHeight, 0.1f, 100.0f);
      ibc::gl::MatrixBase<GLfloat>  translate;
      translate.setTranslationMatrix(0.0f, 0.0f, -5.0f);
      mProjection *= translate;
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
