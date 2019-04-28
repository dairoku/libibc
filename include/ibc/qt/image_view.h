// =============================================================================
//  image_view.h
//
//  MIT License
//
//  Copyright (c) 2018 Dairoku Sekiguchi
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
  \file     ibc/qt/image_view.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/18
  \brief    Header file for handling the image buffer

  This file defines the image buffer class for the ibc Library
*/

#ifndef IBC_QT_IMAGE_VIEW_H_
#define IBC_QT_IMAGE_VIEW_H_

// Includes --------------------------------------------------------------------
#include <QtWidgets>
#include <QImage>

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace qt
 {
  // ---------------------------------------------------------------------------
  // ImageView class
  // ---------------------------------------------------------------------------
  class ImageView : public QWidget
  {
    Q_OBJECT

  public:
    // Constructors and Destructor ---------------------------------------------
    ImageView(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~ImageView();

    double  getZoomScale() { return mZoomScale; };
    void    setZoomScale(double inScale);
    double  calcZoomScale(int inStep);

    const static int    ZOOM_STEP_DEFAULT     = 1;
    const static int    MOUSE_WHEEL_ZOOM_STEP = 60;

  protected:
    void paintEvent(QPaintEvent *event) override;

    QImage  *mImage;
    double  mZoomScale;

    private slots:
    void slot_zoomIn(void);
    void slot_zoomOut(void);
  };
 };
};

#endif  // #ifdef IBC_QT_IMAGE_VIEW_H_

