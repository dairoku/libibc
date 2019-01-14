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
#include <gtkmm.h>
#include "ibc/image/display_buffer.h"
#include "ibc/image/image_buffer.hpp"
#include "ibc/image/display_interface.hpp"

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
    // updatePixbuf
    // -------------------------------------------------------------------------
    virtual bool  updatePixbuf(bool inForceUpdate = false)
    {
      if (inForceUpdate == false && mIsImageModified == false)
        return false;

      if (checkImageBufferPtr() == false || mPixbuf == NULL)
        return false;

      if (mPixbuf->get_width() != mImageFormat.mWidth ||
          mPixbuf->get_height() != mImageFormat.mHeight)
        return false;

      unsigned char *srcPtr = getImageBufferPixelPtr();
      unsigned char *dstPtr = mPixbuf->get_pixels();
      if (mImageFormat.mType.checkType( ibc::image::ImageType::PIXEL_TYPE_RGB,
                                        ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED &&
                                        ibc::image::ImageType::DATA_TYPE_8BIT))
      {
        // ToDo check , width_step, pixel_step
        std::memcpy(dstPtr, srcPtr, mImageFormat.mPixelAreaSize);
        clearIsImageModifiedFlag();
        return true;
      }

      return false;
    }

  protected:
    // Member variables --------------------------------------------------------
    Glib::RefPtr<Gdk::Pixbuf>  mPixbuf;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // parameterModified
    // -------------------------------------------------------------------------
    virtual void  parameterModified()
    {
      mPixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB , false, 8, // does not have alpha and 8bit
                                    mImageFormat.mWidth, mImageFormat.mHeight);
      markAsImageModified();
    }
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_DATA_H_
