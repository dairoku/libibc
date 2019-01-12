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

#ifndef IBC_IMAGE_DISPLAY_BUFFER_H_
#define IBC_IMAGE_DISPLAY_BUFFER_H_

// Includes --------------------------------------------------------------------
#include <Windows.h>
#include <stdio.h>
#include "ibc/image/image_exception.h"
#include "ibc/image/image_buffer.hpp"
#include "ibc/image/display_interface.hpp"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace image
 {
  // ---------------------------------------------------------------------------
  // DisplayBuffer class
  // ---------------------------------------------------------------------------
   template <typename ImageBufferType> class  DisplayBuffer :
          public ImageBuffer<ImageBufferType>, virtual public DisplayInterface
  {
  public:
    // Enum --------------------------------------------------------------------
    enum DisplayMapMode
    {
      DISPLAY_MAP_NOT_SPECIFIED	      = 0,
      DISPLAY_MAP_ANY,
      DISPLAY_MAP_NONE,
      DISPLAY_MAP_DIRECT,

      DISPLAY_MAP_PARTIAL             = 1024,

      DISPLAY_MAP_LUT_1D              = 2048,

      DISPLAY_MAP_LUT_3D              = 4096
    };

    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // DisplayImageBuffer
    // -------------------------------------------------------------------------
    DisplayBuffer(DisplayMapMode inMapMode = DISPLAY_MAP_NOT_SPECIFIED)
      : ImageBuffer()
    {
      mMapMode = inMapMode;
      mIsBufferUpdateNeeded = false;
    }
    // -------------------------------------------------------------------------
    // ~DisplayBuffer
    // -------------------------------------------------------------------------
    virtual ~DisplayBuffer()
    {
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // updateDisplayBuffer
    // -------------------------------------------------------------------------
    virtual void  updateDisplayBuffer()
    {
      if (mUseParentBuffer == true)
        return;

      if (mDisplayBuffer == NULL)
        return;

      switch (mMapMode)
      {
        case DISPLAY_MAP_DIRECT:
        default:  // DISPLAY_MAP_DIRECT
          displayMapDirect();
          break;
      }

      clearIsImageModifiedFlag();
      clearIsBufferUpdateNeededFlag();
    }
    // -------------------------------------------------------------------------
    // setDisplayBufferPtr
    // -------------------------------------------------------------------------
    virtual void  setDisplayBufferPtr(void *inImagePtr, Image::ImageFormat inFormat)
    {  
      mDisplayBuffer.setImageBufferPtr(inImagePtr, inFormat);
    }
    // -------------------------------------------------------------------------
    // allocateDisplayBuffer
    // -------------------------------------------------------------------------
    virtual void allocateDisplayBuffer(Image::ImageFormat inFormat)
    {  
      mDisplayBuffer.allocateDisplayBuffer(inFormat);
    }
    // -------------------------------------------------------------------------
    // getDisplayBufferPtr
    // -------------------------------------------------------------------------
    virtual const unsigned char *getDisplayBufferPtr()
    {  
      return mDisplayBuffer.getImageBufferPtr();
    }
    // -------------------------------------------------------------------------
    // getDisplayBufferFormat
    // -------------------------------------------------------------------------
    virtual ImageFormat  getDisplayBufferFormat()
    {
      return mDisplayBuffer.getImageFormat();
    }
    // -------------------------------------------------------------------------
    // getDisplayMapMode
    // -------------------------------------------------------------------------
    virtual DisplayMapMode  getDisplayMapMode()
    {
      return mMapMode;
    }
    // -------------------------------------------------------------------------
    // setDisplayMapMode
    // -------------------------------------------------------------------------
    virtual bool  setDisplayMapMode(DisplayMapMode inMapMode)
    {
      return false;
    }
    // -------------------------------------------------------------------------
    // setAsBufferUpdateNeeded
    // -------------------------------------------------------------------------
    virtual void  setAsBufferUpdateNeeded()
    {
      mIsBufferUpdateNeeded = true;
    }
    // -------------------------------------------------------------------------
    // clearIsBufferUpdateNeededFlag
    // -------------------------------------------------------------------------
    virtual void  clearIsBufferUpdateNeededFlag()
    {
      mIsBufferUpdateNeeded = false;
    }
    // -------------------------------------------------------------------------
    // isBufferUpdateNeeded
    // -------------------------------------------------------------------------
    virtual bool  isBufferUpdateNeeded()
    {
      return mIsBufferUpdateNeeded;
    }

  protected:
    // Member variables --------------------------------------------------------
    DisplayMapMode  mMapMode;

    ImageBuffer<unsigned char>  mDisplayBuffer;
    bool  mIsBufferUpdateNeeded;

    // Member functions --------------------------------------------------------
    // ---------------------------------------------------------------------
    // displayMapDirect
    // ---------------------------------------------------------------------
    void  displayMapDirect()
    {
      ImageBufferType  *srcPtr = getImageBufferPtr();
      unsigned char  *dstPtr = getDisplayBufferPtr();

      for (int y = 0; y < mDisplayWidth; y++)
        for (int x = 0; x < mDisplayWidth * mOnePixelCount; x++, dstPtr++, srcPtr++)
          *dstPtr = (unsigned char)(*srcPtr);
    }
  };
 };
};

#endif  // #ifdef IBC_IMAGE_DISPLAY_BUFFER_H_
