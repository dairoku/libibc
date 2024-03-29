// =============================================================================
//  image_buffer.h
//
//  Written in 2018 by Dairoku Sekiguchi (sekiguchi at acm dot org)
//
//  To the extent possible under law, the author(s) have dedicated all copyright
//  and related and neighboring rights to this software to the public domain worldwide.
//  This software is distributed without any warranty.
//
//  You should have received a copy of the CC0 Public Domain Dedication along with
//  this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// =============================================================================
/*!
  \file     ibc/image/image_buffer.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/10
  \brief    Format file for handling the image buffer
*/

#ifndef IBC_IMAGE_IMAGE_BUFFER_H_
#define IBC_IMAGE_IMAGE_BUFFER_H_

// Includes --------------------------------------------------------------------
#include <cstring>
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
      mImageFormatPtr           = NULL;
      mAllocatedImageBufferPtr  = NULL;
      mExternalImageBufferPtr   = NULL;

      mIsImageModified          = false;
    }
    // -------------------------------------------------------------------------
    // ~ImageBuffer
    // -------------------------------------------------------------------------
    virtual ~ImageBuffer()
    {
      if (mImageFormatPtr != NULL)
        delete mImageFormatPtr;
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

      mExternalImageBufferPtr = (unsigned char *)inImagePtr;
      imageBufferModified();
    }
    // -------------------------------------------------------------------------
    // setImageBufferPtr
    // -------------------------------------------------------------------------
    void  setImageBufferPtr(void *inImagePtr, const ImageFormat &inFormat)
    {
      if (mAllocatedImageBufferPtr != NULL)
      {
        delete mAllocatedImageBufferPtr;
        mAllocatedImageBufferPtr = NULL;
      }

      if (mImageFormatPtr != NULL)
        delete mImageFormatPtr;
      mImageFormatPtr = new ImageFormat(inFormat);
      if (mImageFormatPtr == NULL)
      {
        throw ImageException(Exception::MEMORY_ERROR,
          "mImageFormatPtr == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }

      mExternalImageBufferPtr = (unsigned char *)inImagePtr;

      parameterModified();
      imageBufferModified();
    }
    // -------------------------------------------------------------------------
    // allocateImageBuffer
    // -------------------------------------------------------------------------
    void  allocateImageBuffer(const ImageFormat &inFormat)
    {
      if (mAllocatedImageBufferPtr != NULL && mImageFormatPtr != NULL)
      {
        // Need sanity check (should check if mBufferSize is not zero)
        if (mImageFormatPtr->mBufferSize == inFormat.mBufferSize)
        {
          delete mImageFormatPtr;
          mImageFormatPtr = new ImageFormat(inFormat);
          if (mImageFormatPtr == NULL)
          {
            throw ImageException(Exception::MEMORY_ERROR,
              "mImageFormatPtr == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
          }
          return;
        }

        delete mAllocatedImageBufferPtr;
        mAllocatedImageBufferPtr = NULL;
      }

      if (mImageFormatPtr != NULL)
        delete mImageFormatPtr;
      mImageFormatPtr = new ImageFormat(inFormat);
      if (mImageFormatPtr == NULL)
      {
        throw ImageException(Exception::MEMORY_ERROR,
          "mImageFormatPtr == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }

      mExternalImageBufferPtr = NULL;

      mAllocatedImageBufferPtr = new unsigned char[mImageFormatPtr->mBufferSize];
      if (mAllocatedImageBufferPtr == NULL)
      {
        throw ImageException(Exception::MEMORY_ERROR,
          "mAllocatedImageBufferPtr == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }

      parameterModified();
    }
    // -------------------------------------------------------------------------
    // copyIntoImageBuffer
    // -------------------------------------------------------------------------
    void  copyIntoImageBuffer(const void *inImagePtr, const ImageFormat &inFormat)
    {
      allocateImageBuffer(inFormat);

      std::memcpy(mAllocatedImageBufferPtr, inImagePtr, mImageFormatPtr->mBufferSize);

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
    bool  isImageModified() const
    {
      return mIsImageModified;
    }
    // -------------------------------------------------------------------------
    // getImageBufferPtr
    // -------------------------------------------------------------------------
    size_t  getImageBufferSize() const
    {
      if (mImageFormatPtr == NULL)
      {
        return 0;
      }
      return mImageFormatPtr->mBufferSize;
    }
    // -------------------------------------------------------------------------
    // getImageBufferPtr
    // -------------------------------------------------------------------------
    void  *getImageBufferPtr() const
    {
      if (mAllocatedImageBufferPtr == NULL)
        return mExternalImageBufferPtr;
      return mAllocatedImageBufferPtr;
    }
    // -------------------------------------------------------------------------
    // checkImageBufferPtr
    // -------------------------------------------------------------------------
    bool  checkImageBufferPtr() const
    {
      if (  (mAllocatedImageBufferPtr == NULL && mExternalImageBufferPtr == NULL) ||
            mImageFormatPtr == NULL)
        return false;
      return true;
    }
    // -------------------------------------------------------------------------
    // getImageBufferPlanePtr
    // -------------------------------------------------------------------------
    void  *getImageBufferPlanePtr(unsigned int inPlaneIndex = 0) const
    {
      if (checkImageBufferPtr() == false)
      {
        throw ImageException(Exception::NULL_POINTER_ACCESS_ERROR,
          "checkImageBufferPtr() == false", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }
      unsigned char *bufferPtr = (unsigned char *)getImageBufferPtr();
      bufferPtr += mImageFormatPtr->getPlaneOffset(inPlaneIndex);
      return bufferPtr;
    }
    // -------------------------------------------------------------------------
    // getImageBufferLinePtr
    // -------------------------------------------------------------------------
    void  *getImageBufferLinePtr(unsigned int inY = 0, unsigned int inPlaneIndex = 0) const
    {
      if (checkImageBufferPtr() == false)
      {
        throw ImageException(Exception::NULL_POINTER_ACCESS_ERROR,
          "checkImageBufferPtr() == false", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }
      unsigned char  *bufferPtr = (unsigned char *)getImageBufferPtr();
      bufferPtr += mImageFormatPtr->getLineOffset(inY, inPlaneIndex);
      return bufferPtr;
    }
    // -------------------------------------------------------------------------
    // getImageBufferPixelPtr
    // -------------------------------------------------------------------------
    void  *getImageBufferPixelPtr(unsigned int inX = 0, unsigned int inY = 0, unsigned int inPlaneIndex = 0) const
    {
      if (checkImageBufferPtr() == false)
      {
        throw ImageException(Exception::NULL_POINTER_ACCESS_ERROR,
          "checkImageBufferPtr() == false", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }
      unsigned char  *bufferPtr = (unsigned char *)getImageBufferPtr();
      bufferPtr += mImageFormatPtr->getPixelOffset(inX, inY, inPlaneIndex);
      return bufferPtr;
    }
    // -------------------------------------------------------------------------
    // getImageFormat
    // -------------------------------------------------------------------------
    ImageFormat  getImageFormat() const
    {
      if (mImageFormatPtr == NULL)
      {
        throw ImageException(Exception::NULL_POINTER_ACCESS_ERROR,
          "mImageFormatPtr == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }
      return *mImageFormatPtr;
    }
    // -------------------------------------------------------------------------
    // getImageType
    // -------------------------------------------------------------------------
    ImageType  getImageType() const
    {
      if (mImageFormatPtr == NULL)
      {
        throw ImageException(Exception::NULL_POINTER_ACCESS_ERROR,
          "mImageFormatPtr == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }
      return mImageFormatPtr->mType;
    }
    // -------------------------------------------------------------------------
    // getPixelAreaSize
    // -------------------------------------------------------------------------
    size_t  getImagePixelAreaSize() const
    {
      if (mImageFormatPtr == NULL)
      {
        throw ImageException(Exception::NULL_POINTER_ACCESS_ERROR,
          "mImageFormatPtr == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }
      return mImageFormatPtr->mPixelAreaSize;
    }
    // -------------------------------------------------------------------------
    // getWidth
    // -------------------------------------------------------------------------
    int  getWidth() const
    {
      if (mImageFormatPtr == NULL)
      {
        throw ImageException(Exception::NULL_POINTER_ACCESS_ERROR,
          "mImageFormatPtr == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }
      return mImageFormatPtr->mWidth;
    }
    // -------------------------------------------------------------------------
    // getHeight
    // -------------------------------------------------------------------------
    int  getHeight() const
    {
      if (mImageFormatPtr == NULL)
      {
        throw ImageException(Exception::NULL_POINTER_ACCESS_ERROR,
          "mImageFormatPtr == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }
      return mImageFormatPtr->mHeight;
    }
    // -------------------------------------------------------------------------
    // getPixelAreaSize
    // -------------------------------------------------------------------------
    size_t  getPixelAreaSize() const
    {
      if (mImageFormatPtr == NULL)
      {
        throw ImageException(Exception::NULL_POINTER_ACCESS_ERROR,
          "mImageFormatPtr == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }
      return mImageFormatPtr->mPixelAreaSize;
    }

  protected:
    // Member variables --------------------------------------------------------
    ImageFormat     *mImageFormatPtr;
    unsigned char *mAllocatedImageBufferPtr;
    unsigned char *mExternalImageBufferPtr;

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
    bool  mIsImageModified;
  };
 };
};

#endif  // #ifdef IBC_IMAGE_IMAGE_BUFFER_H_

