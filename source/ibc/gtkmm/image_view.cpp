// =============================================================================
//  image_view.cpp
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
  \file     ibc/image/image_view.cpp
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/06
  \brief    Header file for ImageView widget

  This file defines the class for the image widget
*/

// Includes --------------------------------------------------------------------
#include <math.h>
#include <cstring>
#include "ibc/gtkmm/image_view.h"

// =============================================================================
// ImageView class
// =============================================================================
ibc::gtkmm::ImageView::ImageView() :
    Glib::ObjectBase("ImageView")
{
  mImageDataPtr = NULL;
  mIsImageSizeChanged = false;

  //signal_realize().connect(sigc::mem_fun(*this, &ibc::gtkmm::ImageView::on_widget_created));
}

ibc::gtkmm::ImageView::~ImageView()
{
}

void ibc::gtkmm::ImageView::setImageDataPtr(ibc::gtkmm::ImageData *inImageDataPtr)
{
  mImageDataPtr = inImageDataPtr;
  mImageDataPtr->addWidget(this);
  markAsImageSizeChanged();
}

bool ibc::gtkmm::ImageView::updateSizeUsingImageData()
{
  if (mImageDataPtr == NULL)
    return false;
  if (mImageDataPtr->checkImageBufferPtr() == false)  // dare to use this instead of checkImageData()
    return false;
 
  mOrgWidth   = mImageDataPtr->getWidth();
  mOrgHeight  = mImageDataPtr->getHeight();
  mWidth       = mOrgWidth;
  mHeight      = mOrgHeight;

  configureHAdjustment();
  configureVAdjustment();
  return true;
}

bool ibc::gtkmm::ImageView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  if (mImageDataPtr == NULL)
    return false;
  if (mImageDataPtr->checkImageData() == false)
    return false;
  if (mIsImageSizeChanged)
  {
    updateSizeUsingImageData();
    mIsImageSizeChanged = false;
  }

  double x = 0, y = 0;

  mImageDataPtr->updatePixbuf();

  if (mWidth <= mWindowWidth)
    x = (mWindowWidth  - mWidth)  / 2;
  else
    x = -1 * mOffsetX;
  if (mHeight <= mWindowHeight)
    y = (mWindowHeight - mHeight) / 2;
  else
    y = -1 * mOffsetY;

  if (mZoom >= 1)
  {
    cr->set_identity_matrix();
    cr->translate(x, y);
    cr->scale(mZoom, mZoom);
    Gdk::Cairo::set_source_pixbuf(cr, mImageDataPtr->mPixbuf, 0, 0);
    Cairo::SurfacePattern pattern(cr->get_source()->cobj());
    pattern.set_filter(Cairo::Filter::FILTER_NEAREST);
  }
  else
  {
    Gdk::Cairo::set_source_pixbuf(cr, mImageDataPtr->mPixbuf->scale_simple(mWidth, mHeight, Gdk::INTERP_NEAREST), x, y);
  }

  cr->paint();

  return true;
}

