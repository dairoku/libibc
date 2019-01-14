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
  \brief    Format file for handling the image buffer

  This file defines the image buffer class for the IBC Library
*/

#ifndef IBC_IMAGE_IMAGE_BUFFER_H_
#define IBC_IMAGE_IMAGE_BUFFER_H_

// Includes --------------------------------------------------------------------
#include <csring>
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
      mAllocatedImageBufferPtr   = NULL;
      mExternalImageBufferPtr    = NULL;

      mIsImageModified        = false;
    }
    // -------------------------------------------------------------------------
    // ~ImageBuffer
    // -------------------------------------------------------------------------
    virtual ~ImageBuffer()
    {
      if (mAllocatedImageBufferPtr != NULL)
        delete mAllocatedImageBufferPtr;
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // updateImageBufferPtr
    // -------------------------------------------------------------------------
    void  updateImageBufferPtr(void *inImagePtr)
    {
      if (mAllocatedImageBufferPtr != NULL)
      {
        delete mAllocatedImageBufferPtr;
        mAllocatedImageBufferPtr = NULL;
      }

      mExternalImageBufferPtr = inImagePtr;
      imageBufferModified();
    }
    // -------------------------------------------------------------------------
    // setImageBufferPtr
    // -------------------------------------------------------------------------
    void  setImageBufferPtr(void *inImagePtr, const Image::ImageFormat &inFormat)
    {
      if (mAllocatedImageBufferPtr != NULL)
      {
        delete mAllocatedImageBufferPtr;
        mAllocatedImageBufferPtr = NULL;
      }

      mImageFormat = inFormat;
      mExternalImageBufferPtr = inImagePtr;

      parameterModified();
      imageBufferModified();
    }
    // -------------------------------------------------------------------------
    // allocateImageBuffer
    // -------------------------------------------------------------------------
    void  allocateImageBuffer(const Image::ImageFormat &inFormat)
    {
      if (mAllocatedImageBufferPtr != NULL)
      {
        if (mImageFormat.mBufferSize == inFormat.mBufferSize)
        {
          mImageFormat = inFormat;
          return;
        }

        delete mAllocatedImageBufferPtr;
        mAllocatedImageBufferPtr = NULL;
      }

      mImageFormat = inFormat;
      mExternalImageBufferPtr = NULL;

      mAllocatedImageBufferPtr = new unsigned char[mImageFormat.mBufferSize];
      if (mAllocatedImageBufferPtr == NULL)
      {
        if (mThrowsEx == false)
          throw ImageException(Exception::MEMORY_ERROR,
            "mAllocatedImageBufferPtr == NULL", BLEU_EXCEPTION_LOCATION_MACRO, 0);
      }

      parameterModified();
    }
    // -------------------------------------------------------------------------
    // copyIntoImageBuffer
    // -------------------------------------------------------------------------
    void  copyIntoImageBuffer(const void *inImagePtr, const Image::ImageFormat &inFormat)
    {
      allocateImageBuffer(inFormat);

      ::std::memcpy(mAllocatedImageBufferPtr, inImagePtr, mImageFormat.mBufferSize);

      imageBufferModified();
    }
    // -------------------------------------------------------------------------
    // markAsImageModified
    // -------------------------------------------------------------------------
    void  markAsImageModified()
    {
      mIsImageModified = true;
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
      if (mAllocatedImageBufferPtr == NULL)
        return mExternalImageBufferPtr;
      return mAllocatedImageBufferPtr;
    }
    // -------------------------------------------------------------------------
    // checkImageBuffer
    // -------------------------------------------------------------------------
    bool  checkImageBufferPtr()
    {
      if (mAllocatedImageBufferPtr == NULL && mExternalImageBufferPtr == NULL)
        return false;
      return true;
    }
    // -------------------------------------------------------------------------
    // getImageBufferPlanePtr
    // -------------------------------------------------------------------------
    void  *getImageBufferPlanePtr(unsigned int inPlaneIndex = 0)
    {
      void  *bufferPtr = getImageBufferPtr();
      bufferPtr += mImageFormat.getPlaneOffset(inPlaneIndex);
      return bufferPtr;
    }
    // -------------------------------------------------------------------------
    // getImageBufferLinePtr
    // -------------------------------------------------------------------------
    void  *getImageBufferLinePtr(unsigned int inY = 0, , unsigned int inPlaneIndex = 0)
    {
      void  *bufferPtr = getImageBufferPtr();
      bufferPtr += mImageFormat.getLineOffset(inY, inPlaneIndex);
      return bufferPtr;
    }
    // -------------------------------------------------------------------------
    // getImageBufferPixelPtr
    // -------------------------------------------------------------------------
    void  *getImageBufferPixelPtr(unsigned int inX = 0, , unsigned int inY = 0, , unsigned int inPlaneIndex = 0)
    {
      void  *bufferPtr = getImageBufferPtr();
      bufferPtr += mImageFormat.getPixelOffset(inX, inY, inPlaneIndex);
      return bufferPtr;
    }
    // -------------------------------------------------------------------------
    // getImageFormat
    // -------------------------------------------------------------------------
    ImageFormat  getImageFormat()
    {
      return mImageFormat;
    }
    // -------------------------------------------------------------------------
    // getPixelAreaSize
    // -------------------------------------------------------------------------
    size_t  getImagePixelAreaSize()
    {
      return mImageFormat.mPixelAreaSize;
    }
    // -------------------------------------------------------------------------
    // getWidth
    // -------------------------------------------------------------------------
    int  getWidth()
    {
      return mImageFormat.mWidth;
    }
    // -------------------------------------------------------------------------
    // getHeight
    // -------------------------------------------------------------------------
    int  getHeight()
    {
      return mImageFormat.mHeight;
    }

  protected:
    // Member variables --------------------------------------------------------
    ImageFormat     mImageFormat;
    void *mAllocatedImageBufferPtr;
    void *mExternalImageBufferPtr;

    // Member functions --------------------------------------------------------
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

