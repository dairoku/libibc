// =============================================================================
//  image_data.h
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
  \file     ibc/image/image_data.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/14
  \brief    Header file for handling the image buffer

  This file defines the image buffer class for the IBC Library
*/

#ifndef IBC_GTKMM_IMAGE_DATA_H_
#define IBC_GTKMM_IMAGE_DATA_H_

// Includes --------------------------------------------------------------------
#include <cstring>
#include <vector>
#include <gtkmm.h>
#include "ibc/image/display_buffer.h"
#include "ibc/gtkmm/view_data_interface.h"
//
#include "ibc/image/converter/rgb_to_rgb.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gtkmm
 {
  // ---------------------------------------------------------------------------
  // ImageData class
  // ---------------------------------------------------------------------------
   class  ImageData : public ibc::image::DisplayBuffer
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ImageData
    // -------------------------------------------------------------------------
    ImageData()
    {
      addImageConverter(&mRGB_to_RGB);
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

      // Sanity check here
      if (mPixbuf->get_width() != mImageFormatPtr->mWidth ||
          mPixbuf->get_height() != mImageFormatPtr->mHeight ||
          mActiveConverter == NULL)
        return false; // Should throw exception?

      mActiveConverter->convert(getImageBufferPixelPtr(), mPixbuf->get_pixels());
      clearIsImageModifiedFlag();
      return true;
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
    //
    ibc::image::converter::RGB_to_RGB   mRGB_to_RGB;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // parameterModified
    // -------------------------------------------------------------------------
    virtual void  parameterModified()
    {
      mPixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB , false, 8, // does not have alpha and 8bit
                                    mImageFormatPtr->mWidth, mImageFormatPtr->mHeight);

      ibc::image::ImageType   imageType(ibc::image::ImageType::PIXEL_TYPE_RGB,
                                        ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                        ibc::image::ImageType::DATA_TYPE_8BIT);
      ibc::image::ImageFormat imageFormat(imageType, mImageFormatPtr->mWidth, mImageFormatPtr->mHeight);
      selectConverter(mImageFormatPtr, &imageFormat);

      markAsImageModified();
      markAllWidgetsAsImageSizeChanged();
    }
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_DATA_H_
