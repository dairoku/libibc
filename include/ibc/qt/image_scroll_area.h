// =============================================================================
//  image_scroll_area.h
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
  \file     ibc/qt/image_scroll_area.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/21
  \brief    Header file for the ImageScrollArea widget
*/

#ifndef IBC_QT_IMAGE_SCROLL_AREA_H_
#define IBC_QT_IMAGE_SCROLL_AREA_H_

// Includes --------------------------------------------------------------------
#include <QtWidgets>
#include <QImage>
#include "ibc/qt/image_view.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace qt
 {
  // ---------------------------------------------------------------------------
  // ImageScrollArea class
  // ---------------------------------------------------------------------------
  class ImageScrollArea : public QScrollArea
  {
    Q_OBJECT

  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ImageScrollArea
    // -------------------------------------------------------------------------
    ImageScrollArea(ImageView *image, QWidget *parent = Q_NULLPTR)
      : QScrollArea(parent)
    {
      mImageView = image;

      setBackgroundRole(QPalette::Dark);
      setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
      setWidget(mImageView);
    }
    // -------------------------------------------------------------------------
    // ~ImageScrollArea
    // -------------------------------------------------------------------------
    virtual ~ImageScrollArea()
    {
    }

  protected:
    // Member variables --------------------------------------------------------
    ImageView *mImageView;
    QPoint  mMousePreviousPos;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // mousePressEvent
    // -------------------------------------------------------------------------
    virtual void  mousePressEvent(QMouseEvent *event)
    {
      if (event->button() == Qt::LeftButton)
      {
        mMousePreviousPos = event->pos();
      }
    }
    // -------------------------------------------------------------------------
    // mouseMoveEvent
    // -------------------------------------------------------------------------
    virtual void  mouseMoveEvent(QMouseEvent *event)
    {
      QPoint  diff = mMousePreviousPos - event->pos();
      setScrollBarValueDiff(horizontalScrollBar(), diff.x());
      setScrollBarValueDiff(verticalScrollBar(), diff.y());
      mMousePreviousPos = event->pos();
    }
    // -------------------------------------------------------------------------
    // mouseReleaseEvent
    // -------------------------------------------------------------------------
    virtual void  mouseReleaseEvent(QMouseEvent *event)
    {
      UNUSED(event);
    }
    // -------------------------------------------------------------------------
    // wheelEvent
    // -------------------------------------------------------------------------
    void wheelEvent(QWheelEvent *wEvent) override
    {
      QPoint delta = wEvent->angleDelta();
      QPoint pos = mImageView->mapFromGlobal(wEvent->globalPos());
      QSize size = mImageView->size();
      double hOffset = pos.x() * (1.0 / mImageView->getZoomScale()); // Offset from the origin
      double vOffset = pos.y() * (1.0 / mImageView->getZoomScale());
      int x_offset = pos.x() - horizontalScrollBar()->value();  // Offset on the display
      int y_offset = pos.y() - verticalScrollBar()->value();

      int step = delta.y() / ImageView::MOUSE_WHEEL_ZOOM_STEP;
      double scale = mImageView->calcZoomScale(step);
      mImageView->setZoomScale(scale);

      if (pos.x() >= 0 && pos.x() < size.width() &&
        pos.y() >= 0 && pos.y() < size.height())
      {
        int h = (int)(hOffset * mImageView->getZoomScale()) - x_offset;
        int v = (int)(vOffset * mImageView->getZoomScale()) - y_offset;
        setScrollBarValue(horizontalScrollBar(), h);
        setScrollBarValue(verticalScrollBar(), v);
      }
    }

  private:
    // Member functions --------------------------------------------------------
    void  setScrollBarValue(QScrollBar *inBar, int inNewValue)
    {
      if (inNewValue < inBar->minimum())
        inNewValue = inBar->minimum();
      if (inNewValue > inBar->maximum())
        inNewValue = inBar->maximum();
      inBar->setValue(inNewValue);
    }
    void  setScrollBarValueDiff(QScrollBar *inBar, int inDiff)
    {
      setScrollBarValue(inBar, inBar->value() + inDiff);
    }
  };
 };
};

#endif  // #ifdef IBC_QT_IMAGE_SCROLL_AREA_H_

