// =============================================================================
//  gl_point_cloud_view.h
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
  \file     ibc/qt/gl_point_cloud_view.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/05/01
  \brief    Header file for the OpenGL Point Cloud Viewer widget
*/

#ifndef IBC_QT_GL_POINT_CLOUD_VIEW_H_
#define IBC_QT_GL_POINT_CLOUD_VIEW_H_

// Includes --------------------------------------------------------------------
#include <QtWidgets>
#include "ibc/gl/matrix.h"
#include "ibc/gl/utils.h"
#include "ibc/gl/trackball.h"
#include "ibc/qt/gl_obj_view.h"
#include "ibc/qt/image_data.h"
//#include "ibc/qt/view_data_interface.h"
#include "ibc/gl/model/color_cube.h"
#include "ibc/gl/model/xyz_axis.h"
#include "ibc/gl/model/points_rgba8.h"
#include "ibc/gl/shader/simple.h"
#include "ibc/gl/shader/point_cloud_rgba8.h"

#include "ibc/gl/model/backdrop_square.h"
#include "ibc/gl/shader/backdrop.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace qt
 {
  // ---------------------------------------------------------------------------
  // GLPointCloudView class
  // ---------------------------------------------------------------------------
//class GLPointCloudView : virtual public GLObjView, virtual public ViewDataInterface
  class GLPointCloudView : virtual public GLObjView
  {
    Q_OBJECT

  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // GLPointCloudView
    // -------------------------------------------------------------------------
    GLPointCloudView(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags())
      : GLObjView(parent, f)
    {
      mBackdropModel.setShader(&mBackdropShader);
      mCubeModel.setShader(&mShader);
      mAxisModel.setShader(&mShader);
      mDataModel.setShader(&mPointCloudShader);

      addShader(&mBackdropShader);
      addShader(&mShader);
      addShader(&mPointCloudShader);

      addModel(&mCubeModel);
      addModel(&mAxisModel);
      addModel(&mDataModel);
      addModel(&mBackdropModel);
    }
    // -------------------------------------------------------------------------
    // ~GLPointCloudView
    // -------------------------------------------------------------------------
    virtual ~GLPointCloudView()
    {
    }

    // Member variables --------------------------------------------------------
    ibc::gl::shader::PointCloudRGBA8  mPointCloudShader;
    ibc::gl::model::PointsRGBA8  mDataModel;
    ibc::gl::model::ColorCube mCubeModel;
    ibc::gl::model::XYZAxis   mAxisModel;
    ibc::gl::model::BackdropSquare mBackdropModel;

  protected:
    // Member variables --------------------------------------------------------
    ibc::gl::shader::Simple   mShader;

    ibc::gl::shader::Backdrop   mBackdropShader;
  };
 };
};

#endif  // #ifdef IBC_QT_GL_POINT_CLOUD_VIEW_H_

