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
  \file     ibc/gtkmm/gl_obj_view.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/16
  \brief    Header file for the OpenGL Object Viewer widget
*/

#ifndef IBC_GTKMM_IMAGE_GL_OBJ_VIEW_H_
#define IBC_GTKMM_IMAGE_GL_OBJ_VIEW_H_

// Includes --------------------------------------------------------------------
#include <math.h>
#include <cstring>
#include <gtkmm.h>
#include "ibc/gl/matrix.h"
#include "ibc/gl/utils.h"
#include "ibc/gl/trackball.h"
#include "ibc/gtkmm/gl_view.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gtkmm
 {
  // ---------------------------------------------------------------------------
  // ImageView class
  // ---------------------------------------------------------------------------
  class GLObjView : virtual public GLView
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // GLObjView
    // -------------------------------------------------------------------------
    GLObjView() :
      Glib::ObjectBase("GLObjView")
    {
      mProjection.setIdentity();

      mCameraFoV = 30.0;

      add_events(Gdk::SCROLL_MASK |
                 Gdk::BUTTON_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
                 Gdk::POINTER_MOTION_MASK);
    }
    // -------------------------------------------------------------------------
    // ~GLObjView
    // -------------------------------------------------------------------------
    virtual ~GLObjView()
    {
    }

  protected:
    // -------------------------------------------------------------------------
    // on_button_press_event
    // -------------------------------------------------------------------------
    virtual bool  on_button_press_event(GdkEventButton* button_event)
    {
      mTrackball.startTrackingMouse(button_event->x, button_event->y,
                                    mWidth, mHeight, button_event->button);
      return true;
    }
    // -------------------------------------------------------------------------
    // on_button_release_event
    // -------------------------------------------------------------------------
    virtual bool  on_button_release_event(GdkEventButton* release_event)
    {
      mTrackball.stopTrackingMouse();
      return true;
    }
    // -------------------------------------------------------------------------
    // on_motion_notify_event
    // -------------------------------------------------------------------------
    virtual bool  on_motion_notify_event(GdkEventMotion* motion_event)
    {
      mTrackball.trackMouse(motion_event->x, motion_event->y);
      queue_render();
      return true;
    }
    // -------------------------------------------------------------------------
    // on_scroll_event
    // -------------------------------------------------------------------------
    virtual bool  on_scroll_event(GdkEventScroll *event)
    {
      mTrackball.mouseWheel(event->direction, event->delta_x, event->delta_y);
      glUpdaetProjection();
      queue_render();
      return true;
    }
    // -------------------------------------------------------------------------
    // glResize
    // -------------------------------------------------------------------------
    virtual void  glResize(int inWidth, int inHeight)
    {
      make_current();
      mWidth = inWidth;
      mHeight = inHeight;
      glViewport(0, 0, mWidth, mHeight);
      glUpdaetProjection();
    }
    // -------------------------------------------------------------------------
    // glUpdaetProjection
    // -------------------------------------------------------------------------
    virtual void  glUpdaetProjection()
    {
      mProjection = ibc::gl::Utils::perspective<GLfloat>(mCameraFoV, mWidth / (GLfloat )mHeight, 0.1, 100);
      ibc::gl::MatrixBase<GLfloat>  translate, scale;
      translate.setTranslationMatrix(0.0, 0.0, -5);
      mProjection *= translate;
    }
    // -------------------------------------------------------------------------
    // glDisplay
    // -------------------------------------------------------------------------
    virtual void  glDisplay()
    {
      // Update ModelView and Projection matrix
      mTrackball.getGLRotationMatrix(mGLModelView);
      mProjection.getTransposedMatrix(mGLProjection);

      ibc::gtkmm::GLView::glDisplay();
    }
    // -------------------------------------------------------------------------
    // enableVertexDataAttributes
    // -------------------------------------------------------------------------
    virtual void enableVertexDataAttributes()
    {
    }

    // Member variables --------------------------------------------------------
    GLfloat mWidth, mHeight;
    GLfloat mCameraFoV;

    ibc::gl::TrackballBase<GLfloat> mTrackball;
    ibc::gl::MatrixBase<GLfloat> mProjection;
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_GL_OBJ_VIEW_H_

