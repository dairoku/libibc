// =============================================================================
//  image_buffer.h
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
  \file     ibc/image/image_buffer.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/10
  \brief    Header file for handling the image buffer

  This file defines the image buffer class for the IBC Library
*/

#ifndef IBC_IMAGE_IMAGE_BUFFER_H_
#define IBC_IMAGE_IMAGE_BUFFER_H_

// Includes --------------------------------------------------------------------
#include <Windows.h>
#include <stdio.h>
#include "ibc/image/image.h"
#include "ibc/image/image_exception.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace image
 {
  // ---------------------------------------------------------------------------
  // ImageBuffer class
  // ---------------------------------------------------------------------------
  class  ImageBuffer
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ImageBuffer
    // -------------------------------------------------------------------------
    ImageBuffer()
    {
      mAllocatedImageBuffer   = NULL;
      mExternalImageBuffer    = NULL;

      mIsImageModified        = false;
    }
    // -------------------------------------------------------------------------
    // ~ImageBuffer
    // -------------------------------------------------------------------------
    virtual ~ImageBuffer()
    {
      if (mAllocatedImageBuffer != NULL)
        delete mAllocatedImageBuffer;
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // updateImageBufferPtr
    // -------------------------------------------------------------------------
    void  updateImageBufferPtr(void *inImagePtr)
    {
      if (mAllocatedImageBuffer != NULL)
      {
        delete mAllocatedImageBuffer;
        mAllocatedImageBuffer = NULL;
      }

      mExternalImageBuffer = inImagePtr;
      imageBufferModified();
    }
    // -------------------------------------------------------------------------
    // setImageBufferPtr
    // -------------------------------------------------------------------------
    void  setImageBufferPtr(void *inImagePtr, Image::ImageHeader inHeader)
    {
      if (mAllocatedImageBuffer != NULL)
      {
        delete mAllocatedImageBuffer;
        mAllocatedImageBuffer = NULL;
      }

      mHeader = inHeader;
      mExternalImageBuffer = inImagePtr;

      parameterModified();
      imageBufferModified();
    }
    // -------------------------------------------------------------------------
    // allocateImageBuffer
    // -------------------------------------------------------------------------
    void  allocateImageBuffer(Image::ImageHeader inHeader)
    {
      if (mAllocatedImageBuffer != NULL)
      {
        if (mImageHeader.getBufferSize() == inHeader.getBufferSize())
        {
          mImageHeader = inHeader;
          return;
        }

        delete mAllocatedImageBuffer;
        mAllocatedImageBuffer = NULL;
      }

      mImageHeader = inHeader;
      mExternalImageBuffer = NULL;

      mAllocatedImageBuffer = new unsigned char[mImageHeader.getBufferSize()];
      if (mAllocatedImageBuffer == NULL)
      {
        if (mThrowsEx == false)
          throw ImageException(Exception::MEMORY_ERROR,
            "mAllocatedImageBuffer == NULL", BLEU_EXCEPTION_LOCATION_MACRO, 0);
      }

      parameterModified();
    }
    // -------------------------------------------------------------------------
    // setMonoImageBufferPtr
    // -------------------------------------------------------------------------
    void  setMonoImageBufferPtr(int inWidth, int inHeight, void *inImagePtr, bool inIsBottomUp = false)
    {
      ImageHeader header(ImageFormat::retrieveNativeMonoImageFormat(),
                         inWidth, inHeight,
                         inIsBottomUp);
      setImageBufferPtr(inImagePtr, header);
    }
    // -------------------------------------------------------------------------
    // setColorImageBufferPtr
    // -------------------------------------------------------------------------
    void  setColorImageBufferPtr(int inWidth, int inHeight, void *inImagePtr, bool inIsBottomUp = false)
    {
      ImageHeader header(ImageFormat::retrieveNativeColorImageFormat(),
                         inWidth, inHeight,
                         inIsBottomUp);
      setImageBufferPtr(inImagePtr, header);
    }
    // -------------------------------------------------------------------------
    // allocateMonoImageBuffer
    // -------------------------------------------------------------------------
    void  allocateMonoImageBuffer(int inWidth, int inHeight, bool inIsBottomUp = false)
    {
      ImageHeader header(ImageFormat::retrieveNativeMonoImageFormat(),
                         inWidth, inHeight,
                         inIsBottomUp);
      allocateImageBuffer(header);
    }
    // -------------------------------------------------------------------------
    // allocateColorImageBuffer
    // -------------------------------------------------------------------------
    void  allocateColorImageBuffer(int inWidth, int inHeight, bool inIsBottomUp = false)
    {
      ImageHeader header(ImageFormat::retrieveNativeColorImageFormat(),
                         inWidth, inHeight,
                         inIsBottomUp);
      allocateImageBuffer(header);
    }
    // -------------------------------------------------------------------------
    // copyIntoImageBuffer
    // -------------------------------------------------------------------------
    void  copyIntoImageBuffer(const void *inImagePtr, Image::ImageHeader inHeader)
    {
      allocateImageBuffer(inHeader);

      ::CopyMemory(mAllocatedImageBuffer, inImagePtr, mImageHeader.mBufferSize);

      parameterModified();
      imageBufferModified();
    }
    // -------------------------------------------------------------------------
    // flipImageBuffer
    // -------------------------------------------------------------------------
    bool  flipImageBuffer()
    {
    }
    // -------------------------------------------------------------------------
    // markAsImageModified
    // -------------------------------------------------------------------------
    void  markAsImageModified()
    {
      mIsImageModified = true;
    }
    // -------------------------------------------------------------------------
    // getImageHeader
    // -------------------------------------------------------------------------
    ImageFormat  getImageHeader()
    {
      return mHeader;
    }
    // -------------------------------------------------------------------------
    // getImageFormat
    // -------------------------------------------------------------------------
    ImageFormat  getImageFormat()
    {
      return mHeader.mFormat;
    }
    // -------------------------------------------------------------------------
    // isImageModified
    // -------------------------------------------------------------------------
    bool  isImageModified()
    {
      return mIsImageModified;
    }
    // -------------------------------------------------------------------------
    // getImageBufferPtr
    // -------------------------------------------------------------------------
    void  *getImageBufferPtr()
    {
      if (mAllocatedImageBuffer == NULL)
        return mExternalImageBuffer;
      return mAllocatedImageBuffer;
    }
    // -------------------------------------------------------------------------
    // getImageBufferLinePtr
    // -------------------------------------------------------------------------
    void  *getImageBufferLinePtr(int inY)
    {
      void  *bufferPtr = getImageBufferPtr();

      bufferPtr += getWidth() * getOnePixelCount() * inY;

      return bufferPtr;
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // obtainNativeColorFormat
    // -------------------------------------------------------------------------
    static BufferFormat  obtainNativeColorFormat()
    {
      return BUFFER_FORMAT_BGR;
    }
    // -------------------------------------------------------------------------
    // obtainNativeColorAlphaFormat
    // -------------------------------------------------------------------------
    static BufferFormat  obtainNativeColorAlphaFormat()
    {
      return BUFFER_FORMAT_BGRA;
    }
    // -------------------------------------------------------------------------
    // checkBufferFormat
    // -------------------------------------------------------------------------
    static BufferFormat  checkBufferFormat(BufferFormat inFormat)
    {
      if (inFormat == BUFFER_FORMAT_NAITIVE_COLOR)
        return obtainNativeColorFormat();
      if (inFormat == BUFFER_FORMAT_NAITIVE_COLOR_ALPHA)
        return obtainNativeColorAlphaFormat();
      return inFormat;
    }
    // -------------------------------------------------------------------------
    // obtainOnePixelCount
    // -------------------------------------------------------------------------
    static int  obtainOnePixelCount(BufferFormat inFormat)
    {
      switch (inFormat)
      {
        case BUFFER_FORMAT_MONO:
          return 1;
        case BUFFER_FORMAT_RGB:
        case BUFFER_FORMAT_BGR:
          return 3;
        case BUFFER_FORMAT_RGBA:
        case BUFFER_FORMAT_BGRA:
          return 4;
      }
      return 0;
    }

  protected:
    // Member variables --------------------------------------------------------
    bool            mIsBitmapBitsDirectMapMode;
    ImageBufferType *mAllocatedImageBuffer;
    ImageBufferType *mExternalImageBuffer;

    ImageFormat     mFormat;
    ImageSize       mSize;
    //BufferFormat    mFormat;
    //int             mWidth;
    //int             mHeight;
    //bool            mIsBottomUp;
    //int             mOnePixelCount;
    //int             mImageBufferPixelCount;
    //size_t          mImageBufferSize;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // clearIsImageModifiedFlag
    // -------------------------------------------------------------------------
    void  clearIsImageModifiedFlag()
    {
      mIsImageModified = false;
    }
    // -------------------------------------------------------------------------
    // parameterModified
    // -------------------------------------------------------------------------
    virtual void  parameterModified()
    {
      markAsImageModified();
    }
    // -------------------------------------------------------------------------
    // imageBufferModified
    // -------------------------------------------------------------------------
    virtual void  imageBufferModified()
    {
      markAsImageModified();
    }

  private:
    // Member variables --------------------------------------------------------
    volatile bool    mIsImageModified;
  };
 };
};

#endif  // #ifdef IBC_IMAGE_IMAGE_BUFFER_H_

