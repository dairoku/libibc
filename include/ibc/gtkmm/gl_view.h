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
  \file     ibc/gtkmm/gl_view.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/16
  \brief    Header file for ImageViewBase widget
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
#include "ibc/gl/shader_interface.h"

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
    virtual Glib::RefPtr<Gdk::GLContext>  on_create_context()
    {
      Glib::RefPtr<Gdk::GLContext> context = Gtk::GLArea::on_create_context();

      // Surprisingly depth buffer will not be created by the default setting
      // We need to change this
      set_has_depth_buffer(true);
      return context;
    }
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

      glClearColor(0.3, 0.3, 0.3, 0.0);

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
    // glDispose
    // -------------------------------------------------------------------------
    virtual void  glDispose()
    {
      for (auto it = mModelList.begin(); it != mModelList.end(); it++)
        (*it)->disposeModel();

      for (auto it = mShaderList.begin(); it != mShaderList.end(); it++)
        (*it)->disposeShader();
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
        (*it)->drawModel(mGLModelView, mGLProjection);
    }

    // Member variables --------------------------------------------------------
    const GLubyte *mRendererStr;
    const GLubyte *mVersionStr;

    std::vector<ibc::gl::ShaderInterface *>   mShaderList;
    std::vector<ibc::gl::ModelInterface *>    mModelList;

    GLfloat mGLModelView[16], mGLProjection[16];

  private:
    // Member functions --------------------------------------------------------
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_GL_VIEW_H_

