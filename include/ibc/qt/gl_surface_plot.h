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
  \file     ibc/qt/gl_surface_plot.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/04/30
  \brief    Header file for the OpenGL Surface Plotting widget
*/

#ifndef IBC_QT_GL_SURFACE_PLOT_H_
#define IBC_QT_GL_SURFACE_PLOT_H_

// Includes --------------------------------------------------------------------
#include <QtWidgets>
#include "ibc/gl/matrix.h"
#include "ibc/gl/utils.h"
#include "ibc/gl/trackball.h"
#include "ibc/qt/gl_obj_view.h"
#include "ibc/qt/image_data.h"
#include "ibc/qt/view_data_interface.h"
#include "ibc/gl/model/color_cube.h"
#include "ibc/gl/model/surface_points.h"
#include "ibc/gl/shader/simple.h"
#include "ibc/gl/shader/point_sprite.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace qt
 {
  // ---------------------------------------------------------------------------
  // GLSurfacePlot class
  // ---------------------------------------------------------------------------
  class GLSurfacePlot : virtual public GLObjView, virtual public ViewDataInterface
  {
    Q_OBJECT

  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // GLSurfacePlot
    // -------------------------------------------------------------------------
    GLSurfacePlot(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags())
      : GLObjView(parent, f)
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
      update();
    }
    // -------------------------------------------------------------------------
    // markAsImageSizeChanged
    // -------------------------------------------------------------------------
    virtual void  markAsImageSizeChanged()
    {
      mIsImageSizeChanged = true;
      update();
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // setImageDataPtr
    // -------------------------------------------------------------------------
    void  setImageDataPtr(ibc::qt::ImageData *inImageDataPtr)
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
    ibc::gl::model::SurfacePoints  mDataModel;

    ibc::qt::ImageData *mImageDataPtr;
    bool      mIsImageSizeChanged;
  };
 };
};

#endif  // #ifdef IBC_QT_GL_SURFACE_PLOT_H_

