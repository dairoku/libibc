// =============================================================================
//  image_scroll_area.cpp
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
  \file     ibc/qt/image_scroll_area.cpp
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/21
  \brief    Header file for handling the image buffer

  This file defines the image buffer class for the IBC Library
*/

// Includes --------------------------------------------------------------------
#include <math.h>
#include "ibc/qt/image_scroll_area.h"


// =============================================================================
// ImageScrollArea class
// =============================================================================
ibc::qt::ImageScrollArea::ImageScrollArea(ImageView *image, QWidget *parent)
      : QScrollArea(parent)
{
  mImageView = image;

  setBackgroundRole(QPalette::Dark);
  setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  setWidget(mImageView);
}

ibc::qt::ImageScrollArea::~ImageScrollArea()
{
}

void ibc::qt::ImageScrollArea::wheelEvent(QWheelEvent *wEvent)
{
  QPoint delta = wEvent->angleDelta();
  QPoint pos = mImageView->mapFromGlobal(wEvent->globalPos());
  QSize size = mImageView->size();
  //double hOffset = (horizontalScrollBar()->value() + pos.x()) * (100.0 / mImageView->getImageScale());
  //double vOffset = (verticalScrollBar()->value() + pos.y()) * (100.0 / mImageView->getImageScale());
  double hOffset = pos.x() * (1.0 / mImageView->getZoomScale()); // Offset from the origin
  double vOffset = pos.y() * (1.0 / mImageView->getZoomScale());
  int x_offset = pos.x() - horizontalScrollBar()->value();  // Offset on the display
  int y_offset = pos.y() - verticalScrollBar()->value();

  printf("pos.x: %d, pos.y: %d\n", pos.x(), pos.y());
  printf("x_offset: %d, y_offset: %d\n", x_offset, y_offset);
  printf("h_val: %d, v_val: %d\n", horizontalScrollBar()->value(), verticalScrollBar()->value());
  printf("hmin: %d, vmin: %d\n", horizontalScrollBar()->minimum(), verticalScrollBar()->minimum());
  printf("hmax: %d, vmax: %d\n", horizontalScrollBar()->maximum(), verticalScrollBar()->maximum());
  printf("hOffset: %f, vOffset: %f\n", hOffset, vOffset);

  int step = delta.y() / ImageView::MOUSE_WHEEL_ZOOM_STEP;
  double scale = mImageView->calcZoomScale(step);
  mImageView->setZoomScale(scale);

  if (pos.x() >= 0 && pos.x() < size.width() &&
      pos.y() >= 0 && pos.y() < size.height())
  {
    int h = (int )((hOffset) * mImageView->getZoomScale()) - x_offset;
    int v = (int)((vOffset)  * mImageView->getZoomScale()) - y_offset;
    printf(" h: %d, v: %d\n", h, v);
    printf(" hmax: %d, vmax: %d\n", horizontalScrollBar()->maximum(), verticalScrollBar()->maximum());
    printf("\n\n");
    QScrollBar *bar = verticalScrollBar();
    if (v < 0)
      v = 0;
    if (v > bar->maximum())
      v = bar->maximum();
    bar->setValue(v);

    bar = horizontalScrollBar();
    if (h < 0)
      h = 0;
    if (h > bar->maximum())
      h = bar->maximum();
    bar->setValue(h);
  }

}
