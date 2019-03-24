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
  \file     ibc/gtkmm/gl_view.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/16
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
*/
// -----------------------------------------------------------------------------
// Appendix
//
// Note that we can't use legacy OpenGL function like glVertex2d()
//
// https://www.bassi.io/articles/2015/02/17/using-opengl-with-gtk/
// http://antongerdelan.net/opengl/index.html
// -----------------------------------------------------------------------------

#ifndef IBC_GTKMM_IMAGE_GL_VIEW_H_
#define IBC_GTKMM_IMAGE_GL_VIEW_H_

// Includes --------------------------------------------------------------------
#include <math.h>
#include <cstring>
#include <epoxy/gl.h>
#include <gtkmm.h>

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gtkmm
 {
  // ---------------------------------------------------------------------------
  // GLView class
  // ---------------------------------------------------------------------------
  class GLView : virtual public Gtk::GLArea
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // GLView
    // -------------------------------------------------------------------------
    GLView() :
      Glib::ObjectBase("GLView")
    {
      /*add_events(Gdk::SCROLL_MASK |
                 Gdk::BUTTON_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
                 Gdk::POINTER_MOTION_MASK/);*/
    }
    // -------------------------------------------------------------------------
    // ~GLView
    // -------------------------------------------------------------------------
    virtual ~GLView()
    {
    }

  protected:
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // on_realize
    // -------------------------------------------------------------------------
    virtual void  on_realize()
    {
      Gtk::GLArea::on_realize();

      make_current();
      glInit();
    }
    // -------------------------------------------------------------------------
    // on_unrealize
    // -------------------------------------------------------------------------
    virtual void  on_unrealize()
    {
      glDispose();
      Gtk::GLArea::on_unrealize();
    }
    // -------------------------------------------------------------------------
    // on_create_context
    // -------------------------------------------------------------------------
    /*virtual Glib::RefPtr<Gdk::GLContext>  on_create_context()
    {
      Glib::RefPtr<Gdk::GLContext> context = Gtk::GLArea::on_create_context();
      return context;
    }*/
    // -------------------------------------------------------------------------
    // on_resize
    // -------------------------------------------------------------------------
    virtual void 	on_resize (int width, int height)
    {
      //Gtk::GLArea::on_resize(width, height);
      glResize(width, height);
    }
    // -------------------------------------------------------------------------
    // on_render
    // -------------------------------------------------------------------------
    virtual bool  on_render(const Glib::RefPtr<Gdk::GLContext> &context)
    {
      glDisplay();
      return true;
    }
    // OpenGL related functions ------------------------------------------------
    // -------------------------------------------------------------------------
    // glInit
    // -------------------------------------------------------------------------
    virtual void  glInit()
    {
      mRendererStr = glGetString(GL_RENDERER);
      mVersionStr = glGetString(GL_VERSION);
      printf("Renderer: %s\n", mRendererStr);
      printf("OpenGL version supported %s\n", mVersionStr);

      glClearColor(0.5, 0.5, 0.5, 1.0);
      glClearDepth(1.0);
      glEnable(GL_DEPTH_TEST);

      initShaders();
      prepareVertexObjects();
    }
    // -------------------------------------------------------------------------
    // glDispose
    // -------------------------------------------------------------------------
    virtual void  glDispose()
    {
    }
    // -------------------------------------------------------------------------
    // glResize
    // -------------------------------------------------------------------------
    virtual void  glResize(int inWidth, int inHeight)
    {
      make_current();

      glViewport(0, 0, inWidth, inHeight);
      glLoadIdentity();
      glOrtho(-inWidth/300.0, inWidth/300.0, -inHeight/300.0, inHeight/300.0, -1.0, 1.0);
    }
    // -------------------------------------------------------------------------
    // glDisplay
    // -------------------------------------------------------------------------
    virtual void  glDisplay()
    {
      make_current();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram(mShaderProgram);
      glBindVertexArray(mVertexArrayObject);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glFlush();
    }
    // -------------------------------------------------------------------------
    // initShaders
    // -------------------------------------------------------------------------
    virtual void  initShaders()
    {
      const char *vertexShaderStr = getVertexShaderStr();
      mVertexShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(mVertexShader, 1, &vertexShaderStr, NULL);
      glCompileShader(mVertexShader);

      const char *fragmentShaderStr = getFragmentShaderStr();
      mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(mFragmentShader, 1, &fragmentShaderStr, NULL);
      glCompileShader(mFragmentShader);

      mShaderProgram = glCreateProgram();
      glAttachShader(mShaderProgram, mFragmentShader);
      glAttachShader(mShaderProgram, mVertexShader);
      glLinkProgram(mShaderProgram);
    }
    // -------------------------------------------------------------------------
    // prepareVertexObjects
    // -------------------------------------------------------------------------
    virtual void  prepareVertexObjects()
    {
      glGenVertexArrays(1, &mVertexArrayObject);
      glBindVertexArray(mVertexArrayObject);

      glGenBuffers(1, &mVertexBufferObject);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glBufferData(GL_ARRAY_BUFFER, getVertexDataSize(), getVertexData(), GL_STATIC_DRAW);

      enableVertexDataAttributes();
    }
    // -------------------------------------------------------------------------
    // getVertexDataSize
    // -------------------------------------------------------------------------
    virtual GLsizeiptr getVertexDataSize()
    {
      return 9 * sizeof(GLfloat);
    }
    // -------------------------------------------------------------------------
    // getVertexData
    // -------------------------------------------------------------------------
    virtual const GLvoid *getVertexData()
    {
      static GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };
      return points;
    }
    // -------------------------------------------------------------------------
    // enableVertexDataAttributes
    // -------------------------------------------------------------------------
    virtual void enableVertexDataAttributes()
    {
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    }
    // -------------------------------------------------------------------------
    // getVertexShaderStr
    // -------------------------------------------------------------------------
    virtual const char *getVertexShaderStr()
    {
      const char *vertexShaderStr = "#version 410\n"
                                    "in vec3 vp;"
                                    "void main ()"
                                    "{"
                                    "  gl_Position = vec4(vp, 1.0);"
                                    "}";
      return vertexShaderStr;
    }
    // -------------------------------------------------------------------------
    // getFragmentShaderStr
    // -------------------------------------------------------------------------
    virtual const char *getFragmentShaderStr()
    {
      const char *fragmentShaderStr = "#version 410\n"
                                      "out vec4 frag_colour;"
                                      "void main ()"
                                      "{"
                                      "  frag_colour = vec4(1.0, 1.0, 0.0, 1.0);"
                                      "}";
      return fragmentShaderStr;
    }

    // Member variables --------------------------------------------------------
    const GLubyte *mRendererStr;
    const GLubyte *mVersionStr;
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mVertexShader, mFragmentShader;
    GLuint mShaderProgram;

  private:
    // Member functions --------------------------------------------------------
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_GL_VIEW_H_

