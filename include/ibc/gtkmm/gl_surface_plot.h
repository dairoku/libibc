// =============================================================================
//  gl_surface_plot.h
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
  \file     ibc/gtkmm/gl_surface_plot.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/24
  \brief    Header file for the OpenGL Surface Plotting widget
*/

#ifndef IBC_GTKMM_GL_SURFACE_PLOT_H_
#define IBC_GTKMM_GL_SURFACE_PLOT_H_

// Includes --------------------------------------------------------------------
#include <math.h>
#include <cstring>
#include <gtkmm.h>
#include "ibc/gl/matrix.h"
#include "ibc/gl/utils.h"
#include "ibc/gl/trackball.h"
#include "ibc/gtkmm/gl_obj_view.h"
#include "ibc/gtkmm/image_data.h"
#include "ibc/gtkmm/view_data_interface.h"
#include "ibc/gl/model/color_cube.h"
#include "ibc/gl/model/xyz_axis.h"
#include "ibc/gl/model/surface_points.h"
#include "ibc/gl/shader/simple.h"
#include "ibc/gl/shader/point_sprite.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gtkmm
 {
  // ---------------------------------------------------------------------------
  // GLSurfacePlot class
  // ---------------------------------------------------------------------------
  class GLSurfacePlot : virtual public GLObjView, virtual public ViewDataInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // GLSurfacePlot
    // -------------------------------------------------------------------------
    GLSurfacePlot() :
      Glib::ObjectBase("GLSurfacePlot")
    {
      mImageDataPtr = NULL;
      mIsImageSizeChanged = false;
      
      mModel.setShader(&mShader);
      mAxis.setShader(&mShader);
      mDataModel.setShader(&mPointSpriteShader);

      addShader(&mShader);
      addShader(&mPointSpriteShader);
      addModel(&mModel);
      addModel(&mAxis);
      addModel(&mDataModel);
    }
    // -------------------------------------------------------------------------
    // ~GLSurfacePlot
    // -------------------------------------------------------------------------
    virtual ~GLSurfacePlot()
    {
    }
    // ViewDataInterface -------------------------------------------------------
    // -------------------------------------------------------------------------
    // queueRedrawWidget
    // -------------------------------------------------------------------------
    virtual void  queueRedrawWidget()
    {
      queue_render();
    }
    // -------------------------------------------------------------------------
    // markAsImageSizeChanged
    // -------------------------------------------------------------------------
    virtual void  markAsImageSizeChanged()
    {
      mIsImageSizeChanged = true;
      queue_render();
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // setImageDataPtr
    // -------------------------------------------------------------------------
    void  setImageDataPtr(ibc::gtkmm::ImageData *inImageDataPtr)
    {
      mImageDataPtr = inImageDataPtr;
      mImageDataPtr->addWidget(this);
      markAsImageSizeChanged();

      GLenum type = ibc::gl::Utils::toGLDataType(mImageDataPtr->getImageType().mDataType);
      mDataModel.setDataPtr(mImageDataPtr->getImageBufferPtr(),
                            mImageDataPtr->getWidth(),
                            mImageDataPtr->getHeight(),
                            type);
    }
    // -------------------------------------------------------------------------
    // isImageSizeChanged
    // -------------------------------------------------------------------------
    bool  isImageSizeChanged() const
    {
      return mIsImageSizeChanged;
    }

  protected:
    // Member variables --------------------------------------------------------
    ibc::gl::shader::Simple  mShader;
    ibc::gl::shader::PointSprite  mPointSpriteShader;
    ibc::gl::model::ColorCube  mModel;
    ibc::gl::model::XYZAxis  mAxis;
    ibc::gl::model::SurfacePoints  mDataModel;

    ImageData *mImageDataPtr;
    bool      mIsImageSizeChanged;
  };
 };
};

#endif  // #ifdef IBC_GTKMM_GL_SURFACE_PLOT_H_

