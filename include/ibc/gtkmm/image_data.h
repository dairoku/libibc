// =============================================================================
//  display_buffer.h
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
  \file     ibc/image/display_buffer.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/10
  \brief    Header file for handling the image buffer

  This file defines the image buffer class for the IBC Library
*/

#ifndef IBC_GTKMM_IMAGE_DATA_H_
#define IBC_GTKMM_IMAGE_DATA_H_

// Includes --------------------------------------------------------------------
#include <cstring>
#include <vector>
#include <gtkmm.h>
#include "ibc/image/image_buffer.h"
#include "ibc/gtkmm/view_data_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gtkmm
 {
  // ---------------------------------------------------------------------------
  // ImageData class
  // ---------------------------------------------------------------------------
   class  ImageData : public ibc::image::ImageBuffer
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ImageData
    // -------------------------------------------------------------------------
    ImageData()
    {
    }
    // -------------------------------------------------------------------------
    // ~ImageData
    // -------------------------------------------------------------------------
    virtual ~ImageData()
    {
    }

    // Member functions --------------------------------------------------------
     // -------------------------------------------------------------------------
    // checkImageBufferPtr
    // -------------------------------------------------------------------------
    bool  checkImageData() const
    {
      if (checkImageBufferPtr() == false || !mPixbuf || mImageFormatPtr == NULL)
        return false;
      return true;
    }
   // -------------------------------------------------------------------------
    // updatePixbuf
    // -------------------------------------------------------------------------
    virtual bool  updatePixbuf(bool inForceUpdate = false)
    {
      if (inForceUpdate == false && isImageModified() == false)
        return false;

      if (checkImageData() == false)
        return false;

      if (mPixbuf->get_width() != mImageFormatPtr->mWidth ||
          mPixbuf->get_height() != mImageFormatPtr->mHeight)
        return false;

      unsigned char *srcPtr = (unsigned char *)getImageBufferPixelPtr();
      unsigned char *dstPtr = mPixbuf->get_pixels();
      if (mImageFormatPtr->mType.checkType( ibc::image::ImageType::PIXEL_TYPE_RGB,
                                            ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                            ibc::image::ImageType::DATA_TYPE_8BIT))
      {
        // ToDo check , width_step, pixel_step
        std::memcpy(dstPtr, srcPtr, mImageFormatPtr->mPixelAreaSize);
        clearIsImageModifiedFlag();
        return true;
      }

      return false;
    }
     // -------------------------------------------------------------------------
    // addWidget
    // -------------------------------------------------------------------------
    void  addWidget(ViewDataInterface *inWidget)
    {
      mWidgetList.push_back(inWidget);
    }
     // -------------------------------------------------------------------------
    // removeWidget
    // -------------------------------------------------------------------------
    void  removeWidget(ViewDataInterface *inWidget)
    {
      auto it = std::find(mWidgetList.begin(), mWidgetList.end(), inWidget);
      if (it == mWidgetList.end())
        return;
      mWidgetList.erase(it);
    }
     // -------------------------------------------------------------------------
    // queueRedrawAllWidgets
    // -------------------------------------------------------------------------
    void  queueRedrawAllWidgets()
    {
      for (auto it = mWidgetList.begin(); it != mWidgetList.end(); it++)
        (*it)->queueRedrawWidget();
    }
     // -------------------------------------------------------------------------
    // markAllWidgetsAsImageSizeChanged
    // -------------------------------------------------------------------------
    void  markAllWidgetsAsImageSizeChanged()
    {
      for (auto it = mWidgetList.begin(); it != mWidgetList.end(); it++)
        (*it)->markAsImageSizeChanged();
    }

    // Member variables --------------------------------------------------------
    Glib::RefPtr<Gdk::Pixbuf>  mPixbuf;

  protected:
    // Member variables --------------------------------------------------------
    std::vector<ViewDataInterface *>  mWidgetList;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // parameterModified
    // -------------------------------------------------------------------------
    virtual void  parameterModified()
    {
      mPixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB , false, 8, // does not have alpha and 8bit
                                    mImageFormatPtr->mWidth, mImageFormatPtr->mHeight);
      markAsImageModified();
      markAllWidgetsAsImageSizeChanged();
    }
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_DATA_H_
