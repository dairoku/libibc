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
//#include <arpa/inet.h>  // <- for byte swapping
#include "ibc/image/image.h"
#include "ibc/image/image_converter_interface.h"
#include "ibc/image/image_exception.h"

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
    // Mono_to_RGB
    // -------------------------------------------------------------------------
    Mono_to_RGB()
    {
      mSrcFormat      = NULL;
      mDstFormat      = NULL;
      mConvertFunc    = NULL;
      mColorMapIndex  = ColorMap::CMIndex_NOT_SPECIFIED;
      mColorMapPtr    = NULL;
      mColorMapMultiNum = 1;
      mGain = 1.0;
      mOffset = 0.0;
      mGamma = 1.0;
      mIsColorMapModified = false;
    }
    // -------------------------------------------------------------------------
    // ~Mono_to_RGB
    // -------------------------------------------------------------------------
    virtual ~Mono_to_RGB()
    {
      if (mSrcFormat != NULL)
        delete mSrcFormat;
      if (mDstFormat != NULL)
        delete mDstFormat;
      if (mColorMapPtr != NULL)
        delete mColorMapPtr;
      mSrcFormat = NULL;
      mDstFormat = NULL;
      mColorMapPtr = NULL;
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // isSupported
    // -------------------------------------------------------------------------
    virtual bool    isSupported(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat) const
    {
      if (findConvertFunction(inSrcFormat, inDstFormat, mColorMapIndex) == NULL)
        return false;
      return true;
    }
    // -------------------------------------------------------------------------
    // init
    // -------------------------------------------------------------------------
    virtual void    init(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat)
    {
      if (mSrcFormat != NULL)
        delete mSrcFormat;
      if (mDstFormat != NULL)
        delete mDstFormat;
      mSrcFormat = new ImageFormat(*inSrcFormat);
      mDstFormat = new ImageFormat(*inDstFormat);
      if (mSrcFormat == NULL || mDstFormat == NULL)
      {
        throw ImageException(Exception::MEMORY_ERROR,
          "mSrcFormat == NULL || mDstFormat == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }

      mWidth = inSrcFormat->mWidth;
      mHeight = inSrcFormat->mHeight;
      mConvertFunc = findConvertFunction(mSrcFormat, mDstFormat, mColorMapIndex);
    }
    // -------------------------------------------------------------------------
    // convert
    // -------------------------------------------------------------------------
    virtual void    convert(const void *inImage, void *outImage)
    {
      if (mConvertFunc == NULL)
        return;
      if (mSrcFormat == NULL || mDstFormat == NULL)
      {
        throw ImageException(Exception::INVALID_OPERATION_ERROR,
          "mSrcFormat == NULL || mDstFormat == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }

      if (mIsColorMapModified)
        mConvertFunc = findConvertFunction(mSrcFormat, mDstFormat, mColorMapIndex);

      mConvertFunc(this, inImage, outImage);
    }
    // -------------------------------------------------------------------------
    // dispose
    // -------------------------------------------------------------------------
    virtual void    dispose()
    {
      if (mSrcFormat != NULL)
        delete mSrcFormat;
      if (mDstFormat != NULL)
        delete mDstFormat;
      if (mColorMapPtr != NULL)
        delete mColorMapPtr;
      mSrcFormat = NULL;
      mDstFormat = NULL;
      mColorMapPtr = NULL;
    }
    // -------------------------------------------------------------------------
    // isColorMapSupported
    // -------------------------------------------------------------------------
    virtual bool  isColorMapSupported()
    {
      return true;
    }
    // -------------------------------------------------------------------------
    // setColorMapIndex
    // -------------------------------------------------------------------------
    virtual void  setColorMapIndex(ColorMap::ColorMapIndex inIndex, int inMultiNum = 1)
    {
      mColorMapIndex = inIndex;
      mColorMapMultiNum = inMultiNum;
      if (mColorMapMultiNum < 1)
        mColorMapMultiNum = 1;
      mIsColorMapModified = true;
    }
    // -------------------------------------------------------------------------
    // getColorMapIndex
    // -------------------------------------------------------------------------
    virtual ColorMap::ColorMapIndex getColorMapIndex() const
    {
      return mColorMapIndex;
    }
    // -------------------------------------------------------------------------
    // getColorMapMultiNum
    // -------------------------------------------------------------------------
    virtual int getColorMapMultiNum() const
    {
      return mColorMapMultiNum;
    }
    // -------------------------------------------------------------------------
    // setGain
    // -------------------------------------------------------------------------
    virtual void  setGain(double inGain)
    {
      mGain = inGain;
      mIsColorMapModified = true;
    }
    // -------------------------------------------------------------------------
    // getGain
    // -------------------------------------------------------------------------
    virtual double  getGain() const
    {
      return mGain;
    }
    // -------------------------------------------------------------------------
    // setChGains
    // -------------------------------------------------------------------------
    virtual void  setChGains(const std::vector<double> &inGain)
    {
      //
      mIsColorMapModified = true;
    }
    // -------------------------------------------------------------------------
    // getChGains
    // -------------------------------------------------------------------------
    virtual std::vector<double> getChGaings() const
    {
      std::vector<double> offsets = {mOffset};
      return offsets;
    }
    // -------------------------------------------------------------------------
    // setOffset
    // -------------------------------------------------------------------------
    virtual void  setOffset(double inOffset)
    {
      mOffset = inOffset;
      mIsColorMapModified = true;
    }
    // -------------------------------------------------------------------------
    // getOffset
    // -------------------------------------------------------------------------
    virtual double  getOffset() const
    {
      return mOffset;
    }
    // -------------------------------------------------------------------------
    // setChOffsets
    // -------------------------------------------------------------------------
    virtual void  setChOffsets(const std::vector<double> &inGain)
    {
      //
      mIsColorMapModified = true;
    }
    // -------------------------------------------------------------------------
    // getChOffsets
    // -------------------------------------------------------------------------
    virtual std::vector<double> getChOffsets() const
    {
      std::vector<double> offsets = {mOffset};
      return offsets;
    }
    // -------------------------------------------------------------------------
    // setGamma
    // -------------------------------------------------------------------------
    virtual void  setGamma(double inGamma)
    {
      mGamma = inGamma;
      mIsColorMapModified = true;
    }
    // -------------------------------------------------------------------------
    // getGamma
    // -------------------------------------------------------------------------
    virtual double  getGamma() const
    {
      return mGamma;
    }

  protected:
    ImageFormat *mSrcFormat, *mDstFormat;
    int  mWidth, mHeight;
    ColorMap::ColorMapIndex mColorMapIndex;
    int mColorMapMultiNum;
    unsigned char *mColorMapPtr;
    bool  mIsColorMapModified;
    double  mGain, mOffset, mGamma;
    void  (*mConvertFunc)(Mono_to_RGB *, const void *, void *);

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // updateImageBufferPtr
    // -------------------------------------------------------------------------
    bool  updateColorMap(bool inForceUpdate = false)
    {
      if (inForceUpdate == false && mIsColorMapModified == false)
        return false;

      if (mColorMapPtr != NULL)
      {
        delete mColorMapPtr;
        mColorMapPtr = NULL;
      }

      if (mColorMapIndex == ColorMap::CMIndex_NOT_SPECIFIED)
        return false;

      int colorNum = 0;
      if (mSrcFormat->mType.mDataType == ImageType::DATA_TYPE_8BIT)
      {
        colorNum = 256;
        mColorMapPtr = new unsigned char[colorNum*3];
      }
      if (mSrcFormat->mType.mDataType == ImageType::DATA_TYPE_16BIT)
      {
        colorNum = 65536;
        mColorMapPtr = new unsigned char[colorNum*3];
      }
      if (mColorMapPtr == NULL)
      {
        throw ImageException(Exception::MEMORY_ERROR,
          "mColorMapPtr == NULL", IBC_EXCEPTION_LOCATION_MACRO, 0);
      }

      if (mColorMapIndex == ColorMap::CMIndex_GrayScale && mGamma != 1.0)
        ColorMap::getMonoMap(colorNum, mColorMapPtr, mGamma, mGain, (int )mOffset);
      else
        ColorMap::getColorMap(mColorMapIndex, colorNum, mColorMapPtr, mColorMapMultiNum, mGain, (int )mOffset);
      mIsColorMapModified = false;
      return true;
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // findConvertFunction
    // -------------------------------------------------------------------------
    static void  (*findConvertFunction(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat, ColorMap::ColorMapIndex inIndex))(Mono_to_RGB *, const void *, void *)
    {
      if (inSrcFormat->mType.checkType( ImageType::PIXEL_TYPE_MONO,
                                        ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                        ImageType::DATA_TYPE_8BIT))
      {
        if (inIndex == ColorMap::CMIndex_NOT_SPECIFIED)
          return convertMono8;
        return convertMono8_ColorMap;
      }
      if (inSrcFormat->mType.checkType( ImageType::PIXEL_TYPE_MONO,
                                        ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                        ImageType::DATA_TYPE_16BIT))
      {
        if (inIndex == ColorMap::CMIndex_NOT_SPECIFIED)
        {
          if (inSrcFormat->mType.mEndian == ImageType::ENDIAN_LITTLE)
            return convertMono16;
          return convertMono16_BigEndian;
        }
        if (inSrcFormat->mType.mEndian == ImageType::ENDIAN_LITTLE)
          return convertMono16_ColorMap;
        return convertMono16_ColorMap_BigEndian;
      }
      return NULL;
    }
    // Convert functions
    // -------------------------------------------------------------------------
    // convertMono8
    // -------------------------------------------------------------------------
    static void  convertMono8(Mono_to_RGB *inObj, const void *inImage, void *outImage)
    {
      size_t  srcPixStep = inObj->mSrcFormat->mPixelStep;

      for (int i = 0; i < inObj->mHeight; i++)
      {
        const unsigned char *srcPtr =
          (const unsigned char *)inObj->mSrcFormat->getLinePtr(inImage, i);
        unsigned char *dstPtr =
          (unsigned char *)inObj->mDstFormat->getLinePtr(outImage, i);

        for (int j = 0; j < inObj->mWidth; j++)
        {
          unsigned char v = *srcPtr;
          srcPtr+=srcPixStep;
          //
          *dstPtr = v;
          dstPtr++;
          *dstPtr = v;
          dstPtr++;
          *dstPtr = v;
          dstPtr++;
        }
      }
    }
    // -------------------------------------------------------------------------
    // convertMono8_ColorMap
    // -------------------------------------------------------------------------
    static void  convertMono8_ColorMap(Mono_to_RGB *inObj, const void *inImage, void *outImage)
    {
      size_t  srcPixStep = inObj->mSrcFormat->mPixelStep;

      inObj->updateColorMap();

      for (int i = 0; i < inObj->mHeight; i++)
      {
        const unsigned char *srcPtr =
          (const unsigned char *)inObj->mSrcFormat->getLinePtr(inImage, i);
        unsigned char *dstPtr =
          (unsigned char *)inObj->mDstFormat->getLinePtr(outImage, i);

        for (int j = 0; j < inObj->mWidth; j++)
        {
          unsigned char v = *srcPtr;
          srcPtr+=srcPixStep;
          //
          unsigned char *mapPtr = &(inObj->mColorMapPtr[v * 3]);
          *dstPtr = *mapPtr;
          dstPtr++;
          mapPtr++;
          *dstPtr = *mapPtr;
          dstPtr++;
          mapPtr++;
          *dstPtr = *mapPtr;
          dstPtr++;
          mapPtr++;
        }
      }
    }
    // -------------------------------------------------------------------------
    // convertMono16 (_LittleEndian)
    // -------------------------------------------------------------------------
    static void  convertMono16(Mono_to_RGB *inObj, const void *inImage, void *outImage)
    {
      size_t  srcPixStep = inObj->mSrcFormat->mPixelStep;

      for (int i = 0; i < inObj->mHeight; i++)
      {
        const unsigned char *srcPtr =
          (const unsigned char *)inObj->mSrcFormat->getLinePtr(inImage, i);
        unsigned char *dstPtr =
          (unsigned char *)inObj->mDstFormat->getLinePtr(outImage, i);

        srcPtr++; // in the case of the little endian and to get the MSB byte)
        for (int j = 0; j < inObj->mWidth; j++)
        {
          unsigned char v = *srcPtr;
          srcPtr+=srcPixStep;
          //
          *dstPtr = v;
          dstPtr++;
          *dstPtr = v;
          dstPtr++;
          *dstPtr = v;
          dstPtr++;
        }
      }
    }
    // -------------------------------------------------------------------------
    // convertMono16_BigEndian
    // -------------------------------------------------------------------------
    static void  convertMono16_BigEndian(Mono_to_RGB *inObj, const void *inImage, void *outImage)
    {
      size_t  srcPixStep = inObj->mSrcFormat->mPixelStep;

      for (int i = 0; i < inObj->mHeight; i++)
      {
        const unsigned char *srcPtr =
          (const unsigned char *)inObj->mSrcFormat->getLinePtr(inImage, i);
        unsigned char *dstPtr =
          (unsigned char *)inObj->mDstFormat->getLinePtr(outImage, i);

        for (int j = 0; j < inObj->mWidth; j++)
        {
          unsigned char v = *srcPtr;
          srcPtr+=srcPixStep;
          //
          *dstPtr = v;
          dstPtr++;
          *dstPtr = v;
          dstPtr++;
          *dstPtr = v;
          dstPtr++;
        }
      }
    }
    // -------------------------------------------------------------------------
    // convertMono16_ColorMap (_LittleEndian)
    // -------------------------------------------------------------------------
    static void  convertMono16_ColorMap(Mono_to_RGB *inObj, const void *inImage, void *outImage)
    {
      size_t  srcPixStep = inObj->mSrcFormat->mPixelStep;

      inObj->updateColorMap();

      for (int i = 0; i < inObj->mHeight; i++)
      {
        const unsigned char *srcPtr =
          (const unsigned char *)inObj->mSrcFormat->getLinePtr(inImage, i);
        unsigned char *dstPtr =
          (unsigned char *)inObj->mDstFormat->getLinePtr(outImage, i);

        for (int j = 0; j < inObj->mWidth; j++)
        {
          unsigned short v = CONV_FROM_LITTLE_ENDIAN(*((const unsigned short *)srcPtr));
          srcPtr+=srcPixStep;
          //
          unsigned char *mapPtr = &(inObj->mColorMapPtr[v * 3]);
          *dstPtr = *mapPtr;
          dstPtr++;
          mapPtr++;
          *dstPtr = *mapPtr;
          dstPtr++;
          mapPtr++;
          *dstPtr = *mapPtr;
          dstPtr++;
          mapPtr++;
        }
      }
    }
    // -------------------------------------------------------------------------
    // convertMono16_ColorMap_BigEndian
    // -------------------------------------------------------------------------
    static void  convertMono16_ColorMap_BigEndian(Mono_to_RGB *inObj, const void *inImage, void *outImage)
    {
      size_t  srcPixStep = inObj->mSrcFormat->mPixelStep;

      inObj->updateColorMap();

      for (int i = 0; i < inObj->mHeight; i++)
      {
        const unsigned char *srcPtr =
          (const unsigned char *)inObj->mSrcFormat->getLinePtr(inImage, i);
        unsigned char *dstPtr =
          (unsigned char *)inObj->mDstFormat->getLinePtr(outImage, i);

        for (int j = 0; j < inObj->mWidth; j++)
        {
          unsigned short v = CONV_FROM_BIG_ENDIAN(*((const unsigned short *)srcPtr));
          srcPtr+=srcPixStep;
          //
          unsigned char *mapPtr = &(inObj->mColorMapPtr[v * 3]);
          *dstPtr = *mapPtr;
          dstPtr++;
          mapPtr++;
          *dstPtr = *mapPtr;
          dstPtr++;
          mapPtr++;
          *dstPtr = *mapPtr;
          dstPtr++;
          mapPtr++;
        }
      }
    }
  };
};

#endif  // #ifdef IBC_IMAGE_CONVERTER_MONO_TO_RGB_H_
