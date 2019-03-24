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
#include <vector>
#include <math.h>
#include <cstring>
#include <epoxy/gl.h>
#include <gtkmm.h>
#include "ibc/gl/matrix.h"
#include "ibc/gl/model_interface.h"

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
    void  removeImageConverter(ibc::gl::ModelInterface *inModel)
    {
      auto it = std::find(mModelList.begin(), mModelList.end(), inModel);
      if (it == mModelList.end())
        return;
      mModelList.erase(it);
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

      for (auto it = mModelList.begin(); it != mModelList.end(); it++)
        (*it)->init();
    }
    // -------------------------------------------------------------------------
    // glDispose
    // -------------------------------------------------------------------------
    virtual void  glDispose()
    {
      for (auto it = mModelList.begin(); it != mModelList.end(); it++)
        (*it)->dispose();
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
      for (auto it = mModelList.begin(); it != mModelList.end(); it++)
        (*it)->draw(mGLModelView, mGLProjection);
    }

    // Member variables --------------------------------------------------------
    const GLubyte *mRendererStr;
    const GLubyte *mVersionStr;

    std::vector<ibc::gl::ModelInterface *>  mModelList;
    GLfloat mGLModelView[16], mGLProjection[16];

  private:
    // Member functions --------------------------------------------------------
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_GL_VIEW_H_

