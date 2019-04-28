// =============================================================================
//  raw_buffer.h
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
  \file     ibc/image/raw_buffer.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/25
  \brief    Header file for handling the image buffer

  This file defines the bitmap image buffer class for the IBC Library
*/

#ifndef IBC_IMAGE_RAW_BUFFER_H_
#define IBC_IMAGE_RAW_BUFFER_H_

// Includes --------------------------------------------------------------------
#include <Windows.h>
#include <stdio.h>
#include "ibc/image/image_exception.h"
#include "ibc/image/display_buffer.h"
#include "ibc/image/raw.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace image
 {
  // ---------------------------------------------------------------------------
  // RawBuffer class
  // ---------------------------------------------------------------------------
  template <typename ImageBufferType> class  RawBuffer : public DisplayBuffer<ImageBufferType>
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // BitmapBuffer
    // -------------------------------------------------------------------------
    RawBuffer()
      : DisplayBuffer()
    {
      mBitmap  = NULL;
    }
    // -------------------------------------------------------------------------
    // ~BitmapBuffer
    // -------------------------------------------------------------------------
    virtual ~RawBuffer()
    {
      if (mBitmap != NULL)
        delete mBitmap;
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // getColorIndexNum
    // -------------------------------------------------------------------------
    int  getColorIndexNum()
    {
        return 0;   // TODO : implement this
    }
    // -------------------------------------------------------------------------
    // getBitmapImageBufPtr
    // -------------------------------------------------------------------------
    const unsigned char  *getBitmapImageBufPtr()
    {
      if (updateBitmapInfoPtr() == false)
        return NULL;

      unsigned char  *imageBufferPtr = allocateDisplayBuffer();
      if (imageBufferPtr == NULL)
        return NULL;

      if (mBitmap->setBitmapBits(imageBufferPtr, getDisplayBufferSize()) == false)
        return NULL;
      
      return imageBufferPtr;
    }
    // -------------------------------------------------------------------------
    // saveToRawFile
    // -------------------------------------------------------------------------
    void  saveToRawFile(const char *inFileName)
    {
      mRaw->saveToFile(inFileName);
    }
    // ---------------------------------------------------------------------
    // loadFromRawFile
    // ---------------------------------------------------------------------
    void  loadFromBitmapFile(const char *inFileName)
    {
    }
    // ---------------------------------------------------------------------
    // allocateBitmap
    // ---------------------------------------------------------------------
    bool  allocateBitmap()
    {
      if (mBitmap != NULL)
        return true;

      int  bitCount = obtainBitmapBitCount(mFormat);
      if (bitCount == 0)
      {
        if (mThrowsEx == false)
          return false;
        else
          throw ImageException(Exception::INVALID_OPERATION_ERROR,
                "unsupported format", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }

      mBitmap = Bitmap::createBitmap(mWidth, mHeight, bitCount, mThrowsEx);
      if (mBitmap == NULL)
        return false;

      return true;
    }
  
    // Static Functions ----------------------------------------------------
    // ---------------------------------------------------------------------
    // obtainBitmapHeight
    // ---------------------------------------------------------------------
    static int  obtainBitmapHeight(int inHeight, bool inIsBottomUp)
    {
      if (inIsBottomUp == false)
        return -1 * inHeight;

      return inHeight;
    }
    // ---------------------------------------------------------------------
    // obtainBitmapBitCount
    // ---------------------------------------------------------------------
    static int  obtainBitmapBitCount(BufferFormat inFormat)
    {
      switch (inFormat)
      {
        case BUFFER_FORMAT_MONO:
          return 8;
        //case BUFFER_FORMAT_RGB:
        case BUFFER_FORMAT_BGR:
          return 24;
        //case BUFFER_FORMAT_RGBA:
        //case BUFFER_FORMAT_BGRA:
        //  return 4;
      }

      return 0;
    }

  protected:
    // Member variables ----------------------------------------------------
    Bitmap        *mBitmap;

    // Member functions ----------------------------------------------------
    // ---------------------------------------------------------------------
    // parameterModified
    // ---------------------------------------------------------------------
    //virtual void  parameterModified()
    //{
    //  ImageBuffer::parameterModified();
    //  updateBitmapInfoPtr();
    //}
    // ---------------------------------------------------------------------
    // updateBitmapInfoPtr
    // ---------------------------------------------------------------------
    const bool  updateBitmapInfoPtr()
    {
      //unsigned char  *displayBuffer = getDisplayBufferPtr();
      //if (displayBuffer == NULL)
      //  return false;

      if (mBitmap == NULL)
      {
        if (allocateBitmap() == false)
          return false;
        return true;
      }

      int  bitCount = obtainBitmapBitCount(mFormat);
      int height = obtainBitmapHeight(mHeight, mIsBottomUp);

      return mBitmap->setBitmapInfo(mWidth, height, bitCount);
    }

  };
 };
};

#endif  // #ifdef IBC_IMAGE_RAW_BUFFER_H_

