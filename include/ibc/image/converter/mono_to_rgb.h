// =============================================================================
//  mono_to_rgb.h
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
  \file     ibc/image/converter/mono_to_rgb.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/22
  \brief    Header file for handling the image buffer

  This file defines the image buffer class for the IBC Library
*/

#ifndef IBC_IMAGE_CONVERTER_MONO_TO_RGB_H_
#define IBC_IMAGE_CONVERTER_MONO_TO_RGB_H_

// Includes ------------------------------------------------------ --------------
#include <cstring>
#include "ibc/image/image_converter_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc::image::converter // <- nested namespace (C++17)
{
  // ---------------------------------------------------------------------------
  // RGB_to_RGB class
  // ---------------------------------------------------------------------------
  class  Mono_to_RGB : public virtual ImageConverterInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // RGB_to_RGB
    // -------------------------------------------------------------------------
    Mono_to_RGB()
    {
    }
    // -------------------------------------------------------------------------
    // ~RGB_to_RGB
    // -------------------------------------------------------------------------
    virtual ~Mono_to_RGB()
    {
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // isSupported
    // -------------------------------------------------------------------------
    virtual bool    isSupported(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat) const
    {
      if (inSrcFormat->mType.checkType( ibc::image::ImageType::PIXEL_TYPE_MONO,
                                        ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                        ibc::image::ImageType::DATA_TYPE_8BIT))
        return true;
    }
    // -------------------------------------------------------------------------
    // init
    // -------------------------------------------------------------------------
    virtual void    init(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat)
    {
      mWidth = inSrcFormat->mWidth;
      mHeight = inSrcFormat->mHeight;
    }
    // -------------------------------------------------------------------------
    // convert
    // -------------------------------------------------------------------------
    virtual void    convert(const void *inImage, void *outImage)
    {
      const unsigned char *srcPtr = (unsigned char *)inImage;
      unsigned char *dstPtr = (unsigned char *)outImage;

      for (int i = 0; i < mHeight; i++)
        for (int j = 0; j < mWidth; j++)
        {
          *dstPtr = *srcPtr;
          dstPtr++;
          *dstPtr = *srcPtr;
          dstPtr++;
          *dstPtr = *srcPtr;
          dstPtr++;
          srcPtr++;
        }
    }
    // -------------------------------------------------------------------------
    // dispose
    // -------------------------------------------------------------------------
    virtual void    dispose()
    {
      // Do nothing at this moment...
    }
  protected:
    int  mWidth, mHeight;
  };
};

#endif  // #ifdef IBC_IMAGE_CONVERTER_MONO_TO_RGB_H_
