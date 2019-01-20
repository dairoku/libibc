// =============================================================================
//  display_buffer.h
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
  \file     ibc/image/display_buffer.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/20
  \brief    Format file for handling the image buffer

  This file defines the image buffer class for the IBC Library
*/

#ifndef IBC_IMAGE_DISPLAY_BUFFER_H_
#define IBC_IMAGE_DISPLAY_BUFFER_H_

// Includes --------------------------------------------------------------------
#include <cstring>
#include <vector>
#include "ibc/image/image_buffer.h"
#include "ibc/image/image_converter_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace image
 {
  // ---------------------------------------------------------------------------
  // DisplayBuffer class
  // ---------------------------------------------------------------------------
  class  DisplayBuffer : public ImageBuffer
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // DisplayBuffer
    // -------------------------------------------------------------------------
    DisplayBuffer()
    {
      mActiveConverter = NULL;
    }
    // -------------------------------------------------------------------------
    // ~ImageBuffer
    // -------------------------------------------------------------------------
    virtual ~DisplayBuffer()
    {
      if (mActiveConverter != NULL)
        mActiveConverter->dispose();
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // addImageConverter
    // -------------------------------------------------------------------------
    void  addImageConverter(ImageConverterInterface *inConverter)
    {
      mConverterList.push_back(inConverter);
    }
    // -------------------------------------------------------------------------
    // removeImageConverter
    // -------------------------------------------------------------------------
    void  removeImageConverter(ImageConverterInterface *inConverter)
    {
      auto it = std::find(mConverterList.begin(), mConverterList.end(), inConverter);
      if (it == mConverterList.end())
        return;
      mConverterList.erase(it);
    }

    // Member variables --------------------------------------------------------
    ImageConverterInterface *mActiveConverter;

  protected:
    // Member variables --------------------------------------------------------
    std::vector<ImageConverterInterface *>  mConverterList;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // selectConverter
    // -------------------------------------------------------------------------
    bool  selectConverter(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat)
    {
      if (mActiveConverter != NULL)
        mActiveConverter->dispose();
      mActiveConverter = findSupportedConverter(inSrcFormat, inDstFormat);
      if (mActiveConverter != NULL)
        mActiveConverter->init(inSrcFormat, inDstFormat);
    }

  private:
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // findConverter
    // -------------------------------------------------------------------------
    ImageConverterInterface *findSupportedConverter(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat)
    {
      for (auto it = mConverterList.begin(); it != mConverterList.end(); it++)
        if ((*it)->isSupported(inSrcFormat, inDstFormat))
          return (*it);
      return NULL;
    }
  };
 };
};

#endif  // #ifdef IBC_IMAGE_DISPLAY_BUFFER_H_

