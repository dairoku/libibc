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
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
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
      mModelView.setIdentity();
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
    // structs -----------------------------------------------------------------
    // -------------------------------------------------------------------------
    // vertex_info
    // -------------------------------------------------------------------------
    struct vertex_info
    {
      GLfloat position[3];
      GLfloat color[3];
    };
    // -------------------------------------------------------------------------
    // on_button_press_event
    // -------------------------------------------------------------------------
    virtual bool  on_button_press_event(GdkEventButton* button_event)
    {
      mTrackball.startTrackingMouse(button_event->x, button_event->y, mWidth, mHeight);
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
      mModelView = mTrackball.trackMouse(motion_event->x, motion_event->y);
      queue_render();
      return true;
    }
    // -------------------------------------------------------------------------
    // on_scroll_event
    // -------------------------------------------------------------------------
    virtual bool  on_scroll_event(GdkEventScroll *event)
    {
      //std::cout << "wheel event\n"
      //          << "time = " << event->time << std::endl
      //          << "x = " << event->x << std::endl
      //          << "y = " << event->y << std::endl
      //          << "state = " << event->state << std::endl
      //          << "direction = " << event->direction << std::endl
      //          << "delta_x = " << event->delta_x << std::endl
      //          << "delta_y = " << event->delta_y << std::endl;

      if (event->direction == 0)
        mCameraFoV -= 1;
      else
        mCameraFoV += 1;

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
      mProjection = ibc::gl::Utils<GLfloat>::perspective(mCameraFoV, mWidth / (GLfloat )mHeight, 1.0, 100);
      ibc::gl::MatrixBase<GLfloat>  translate;
      translate.setTranslationMatrix(0.0, 0.0, -5.0);
      mProjection *= translate;
    }
    // -------------------------------------------------------------------------
    // glDisplay
    // -------------------------------------------------------------------------
    virtual void  glDisplay()
    {
      GLfloat glMat[16];

      make_current();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram(mShaderProgram);
      mModelView.getTransposedMatrix(glMat);
      glUniformMatrix4fv(mModelViewLocation, 1, GL_FALSE, &(glMat[0]));
      mProjection.getTransposedMatrix(glMat);
      glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, &(glMat[0]));
      glBindVertexArray(mVertexArrayObject);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glFlush();
    }
    // -------------------------------------------------------------------------
    // getVertexDataSize
    // -------------------------------------------------------------------------
    virtual GLsizeiptr getVertexDataSize()
    {
      return 9 * 2 * sizeof(GLfloat);
    }
    // -------------------------------------------------------------------------
    // getVertexData
    // -------------------------------------------------------------------------
    virtual const GLvoid *getVertexData()
    {
      static const struct vertex_info vertexData[] =
      {
        { {  0.0f,  0.500f, 0.0f }, { 1.f, 0.f, 0.f } },
        { {  0.5f, -0.366f, 0.0f }, { 0.f, 1.f, 0.f } },
        { { -0.5f, -0.366f, 0.0f }, { 0.f, 0.f, 1.f } },
      };
      return vertexData;
    }
    // -------------------------------------------------------------------------
    // enableVertexDataAttributes
    // -------------------------------------------------------------------------
    virtual void enableVertexDataAttributes()
    {
      // get the location of the "modelview" uniform
      mModelViewLocation = glGetUniformLocation (mShaderProgram, "modelview");

      // get the location of the "projection" uniform
      mProjectionLocation = glGetUniformLocation (mShaderProgram, "projection");

      // get the location of the "position" and "color" attributes
      guint positionLocation = glGetAttribLocation (mShaderProgram, "position");
      guint colorLocation = glGetAttribLocation (mShaderProgram, "color");

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

      // enable and set the position attribute
      glEnableVertexAttribArray (positionLocation);
      glVertexAttribPointer (positionLocation, 3, GL_FLOAT, GL_FALSE,
                             sizeof (struct vertex_info),
                             (GLvoid *) (G_STRUCT_OFFSET (struct vertex_info, position)));

      // enable and set the color attribute
      glEnableVertexAttribArray (colorLocation);
      glVertexAttribPointer (colorLocation, 3, GL_FLOAT, GL_FALSE,
                             sizeof (struct vertex_info),
                             (GLvoid *) (G_STRUCT_OFFSET (struct vertex_info, color)));
    }
    // -------------------------------------------------------------------------
    // getVertexShaderStr
    // -------------------------------------------------------------------------
    virtual const char *getVertexShaderStr()
    {
      const char *vertexShaderStr = "#version 130\n"
                                    "in vec3 position;"
                                    "in vec3 color;"
                                    "uniform mat4 modelview;"
                                    "uniform mat4 projection;"
                                    "smooth out vec4 vertexColor;"
                                    "void main() {"
                                    "  gl_Position = projection * modelview * vec4(position, 1.0);"
                                    "  vertexColor = vec4(color, 1.0);"
                                    "}";
      return vertexShaderStr;
    }
    // -------------------------------------------------------------------------
    // getFragmentShaderStr
    // -------------------------------------------------------------------------
    virtual const char *getFragmentShaderStr()
    {
      const char *fragmentShaderStr = "#version 130\n"
                                      "smooth in vec4 vertexColor;"
                                      "out vec4 outputColor;"
                                      "void main() {"
                                      "  outputColor = vertexColor;"
                                      "}";
      return fragmentShaderStr;
    }

    // Member variables --------------------------------------------------------
    GLfloat mZoom;
    GLfloat mWidth, mHeight;

    GLfloat mCameraFoV;

    ibc::gl::TrackballBase<GLfloat> mTrackball;
    ibc::gl::MatrixBase<GLfloat> mModelView;
    ibc::gl::MatrixBase<GLfloat> mProjection;

    guint mModelViewLocation;
    guint mProjectionLocation;
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_GL_OBJ_VIEW_H_

