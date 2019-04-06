// =============================================================================
//  gl_surface_plot.h
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
  \file     ibc/gtkmm/gl_surface_plot.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/24
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
*/

#ifndef IBC_GTKMM_IMAGE_GL_SURFACE_PLOT_H_
#define IBC_GTKMM_IMAGE_GL_SURFACE_PLOT_H_

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
#include "ibc/gl/model/surface_points.h"
#include "ibc/gl/shader/simple.h"
#include "ibc/gl/shader/point_sprite.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gtkmm
 {
  // ---------------------------------------------------------------------------
  // ImageView class
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
      mDataModel.setShader(&mPointSpriteShader);

      addShader(&mShader);
      addShader(&mPointSpriteShader);
      addModel(&mModel);
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
    ibc::gl::model::SurfacePoints  mDataModel;

    ImageData *mImageDataPtr;
    bool      mIsImageSizeChanged;
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_GL_SURFACE_PLOT_H_

