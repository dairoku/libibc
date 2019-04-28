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
  \file     ibc/image/image_view.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/18
  \brief    Header file for handling the image buffer

  This file defines the image buffer class for the IBC Library
*/

// Includes --------------------------------------------------------------------
#include <math.h>
#include "ibc/qt/image_view.h"


// =============================================================================
// ImageView class
// =============================================================================
ibc::qt::ImageView::ImageView(QWidget *parent, Qt::WindowFlags f)
      : QWidget(parent, f)
{
  mImage = new QImage(256, 256, QImage::Format_Grayscale8);
    
  uchar *pixPtr = mImage->bits();
  for (int i = 0; i < 256; i++)
    for (int j = 0; j < 256; j++, pixPtr++)
      *pixPtr = ((uchar)j) ^ ((uchar)i);

  mZoomScale = 1.0;
  resize(256, 256);
}

ibc::qt::ImageView::~ImageView()
{
}

void ibc::qt::ImageView::paintEvent(QPaintEvent *event)
{
  QRect rect(0, 0, size().width(), size().height());
  QPainter painter(this);
    
  painter.drawImage(rect, *mImage);
  //painter.drawText(rect, Qt::AlignCenter, "Hello, world");
  //painter.restore();
}

double ibc::qt::ImageView::calcZoomScale(int inStep)
{
  double	val, scale;

  //val = log10(mZoomScale*100.0) + inStep/100.0;
  //scale = pow(10, val)/100.0;
  scale = mZoomScale * pow(10, inStep / 100.0);

  // 100% snap & 1% limit (just in case...)
  if (fabs(scale - 1.0) <= 0.01)
    scale = 1.0;
  if (scale <= 0.01)
    scale = 0.01;

  return scale;
}

void ibc::qt::ImageView::setZoomScale(double inScale)
{
  double	prevZoomScale = mZoomScale;

  if (inScale <= 0.01)
    inScale = 0.01;
  mZoomScale = inScale;

  double scale = mZoomScale;
  int width = (int)(256.0 * scale);
  int height = (int)(256.0 * scale);
  resize(width, height);

  //checkImageViewOffset();
  //updateStatusBar();

  /*double	scale = mZoomScale / 100.0;
  if (prevZoomScale > mZoomScale &&
    ((int)(getWidth() * scale) <= mImageViewRect.right - mImageViewRect.left ||
    (int)(getHeight() * scale) <= mImageViewRect.bottom - mImageViewRect.top))
    updateImageView(true);
  else
    updateImageView();

  updateMouseCursor();*/
}

void ibc::qt::ImageView::slot_zoomIn(void)
{
  double scale = calcZoomScale(ZOOM_STEP_DEFAULT);
  setZoomScale(scale);
}

void ibc::qt::ImageView::slot_zoomOut(void)
{
  double scale = calcZoomScale(-ZOOM_STEP_DEFAULT);
  setZoomScale(scale);
}
