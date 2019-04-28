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
  \brief    Header file for handling the image buffer

  This file defines the image buffer class for the ibc Library
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
    ImageScrollArea(ImageView *image, QWidget *parent = Q_NULLPTR);
    virtual ~ImageScrollArea();

  protected:
    void wheelEvent(QWheelEvent *wEvent) override;

    ImageView *mImageView;
  };
 };
};

#endif  // #ifdef IBC_QT_IMAGE_SCROLL_AREA_H_

