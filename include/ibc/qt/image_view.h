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
  \brief    Header file for the ImageView widget
*/

#ifndef IBC_QT_IMAGE_VIEW_H_
#define IBC_QT_IMAGE_VIEW_H_

// Includes --------------------------------------------------------------------
#include <QtWidgets>
#include <QImage>
#include "ibc/qt/image_data.h"
#include "ibc/qt/view_data_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace qt
 {
  // ---------------------------------------------------------------------------
  // ImageView class
  // ---------------------------------------------------------------------------
  class ImageView : public QWidget, virtual public ViewDataInterface
  {
    Q_OBJECT

  public:
    // Constants ---------------------------------------------------------------
    const static int    ZOOM_STEP_DEFAULT     = 1;
    const static int    MOUSE_WHEEL_ZOOM_STEP = 60;

    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ImageView
    // -------------------------------------------------------------------------
    ImageView(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags())
      : QWidget(parent, f)
    {
      mImageDataPtr = NULL;
      mIsImageSizeChanged = false;
      mZoomScale = 1.0;
    }
    // -------------------------------------------------------------------------
    // ~ImageView
    // -------------------------------------------------------------------------
    virtual ~ImageView()
    {
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // setImageDataPtr
    // -------------------------------------------------------------------------
    void  setImageDataPtr(ibc::qt::ImageData *inImageDataPtr)
    {
      if (mImageDataPtr == NULL)
      {
        mImageDataPtr = inImageDataPtr;
        mImageDataPtr->addWidget(this);
        updateSizeUsingImageData();
        return;
      }

      mImageDataPtr->removeWidget(this);
      mImageDataPtr = inImageDataPtr;
      mImageDataPtr->addWidget(this);
      markAsImageSizeChanged();
    }
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
    // -------------------------------------------------------------------------
    // isImageSizeChanged
    // -------------------------------------------------------------------------
    bool  isImageSizeChanged() const
    {
      return mIsImageSizeChanged;
    }
    // -------------------------------------------------------------------------
    // getZoomScale
    // -------------------------------------------------------------------------
    double  getZoomScale()
    {
      return mZoomScale;
    };
    // -------------------------------------------------------------------------
    // setZoomScale
    // -------------------------------------------------------------------------
    void  setZoomScale(double inScale)
    {
      if (mImageDataPtr == NULL)
        return;

      if (inScale <= 0.01)
        inScale = 0.01;
      mZoomScale = inScale;

      double scale = mZoomScale;
      int width = (int )(mImageDataPtr->getWidth() * scale);
      int height = (int )(mImageDataPtr->getHeight() * scale);
      resize(width, height);
    }
    // -------------------------------------------------------------------------
    // calcZoomScale
    // -------------------------------------------------------------------------
    double  calcZoomScale(int inStep)
    {
      double  scale = mZoomScale * pow(10, inStep / 100.0);

      // 100% snap & 1% limit (just in case...)
      if (fabs(scale - 1.0) <= 0.01)
        scale = 1.0;
      if (scale <= 0.01)
        scale = 0.01;

      return scale;
    }

  protected:
    // Member variables --------------------------------------------------------
    ImageData *mImageDataPtr;
    bool      mIsImageSizeChanged;
    double    mZoomScale;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // updateSizeUsingImageData
    // -------------------------------------------------------------------------
    bool  updateSizeUsingImageData()
    {
      if (mImageDataPtr == NULL)
        return false;
      if (mImageDataPtr->checkImageBufferPtr() == false)  // dare to use this instead of checkImageData()
        return false;
     
      setZoomScale(mZoomScale);
      return true;
    }
    // -------------------------------------------------------------------------
    // paintEvent
    // -------------------------------------------------------------------------
    void paintEvent(QPaintEvent *event) override
    {
      UNUSED(event);
      //
      if (mImageDataPtr == NULL)
        return;
      if (mImageDataPtr->checkImageData() == false)
        return;
      if (mIsImageSizeChanged)
      {
        updateSizeUsingImageData();
        mIsImageSizeChanged = false;
      }

      mImageDataPtr->updateQImage();
      QRect rect(0, 0, size().width(), size().height());
      QPainter painter(this);

      painter.drawImage(rect, *mImageDataPtr->mQImage);
      //painter.drawText(rect, Qt::AlignCenter, "Hello, world");
      //painter.restore();
    }

  private slots:
    // Qt slot member functions ------------------------------------------------
    // -------------------------------------------------------------------------
    // slot_zoomIn
    // -------------------------------------------------------------------------
    void slot_zoomIn(void)
    {
      double scale = calcZoomScale(ZOOM_STEP_DEFAULT*5);
      setZoomScale(scale);
    }
    // -------------------------------------------------------------------------
    // slot_zoomOut
    // -------------------------------------------------------------------------
    void slot_zoomOut(void)
    {
      double scale = calcZoomScale(-ZOOM_STEP_DEFAULT*5);
      setZoomScale(scale);
    }
  };
 };
};

#endif  // #ifdef IBC_QT_IMAGE_VIEW_H_

