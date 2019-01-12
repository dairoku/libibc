// =============================================================================
//  image.h
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
  \file     ibc/image/image.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/11
  \brief    Header file for handling the image

  This file defines the image buffer class for the IBC Library
*/

#ifndef IBC_IMAGE_IMAGE_H_
#define IBC_IMAGE_IMAGE_H_

// Includes --------------------------------------------------------------------
#include "ibc/base/types.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace image
 {
  // ---------------------------------------------------------------------------
  // ImageType class
  // ---------------------------------------------------------------------------
  class  ImageType
  {
  public:
    // Enum --------------------------------------------------------------------
    enum  PixelType
    {
      PIXEL_TYPE_NOT_SPECIFIED  = 0,
      PIXEL_TYPE_RAW            = 1024,
      PIXEL_TYPE_MONO           = 2048,
      PIXEL_TYPE_BAYER          = 3072,
      PIXEL_TYPE_RGB            = 4096,
      PIXEL_TYPE_RGBA,
      PIXEL_TYPE_BGR,
      PIXEL_TYPE_BGRA,
      PIXEL_TYPE_CMY            = 5120,
      PIXEL_TYPE_CMYK,
      PIXEL_TYPE_HSL            = 6144,
      PIXEL_TYPE_HSB,
      PIXEL_TYPE_LUV,
      PIXEL_TYPE_LAB,
      PIXEL_TYPE_YUV410         = 7168, // YUV9
      PIXEL_TYPE_YUV411,
      PIXEL_TYPE_YUV420,                // YUV12
      PIXEL_TYPE_YUV422,
      PIXEL_TYPE_YUV444,
      PIXEL_TYPE_FOURCC         = 8192,
      PIXEL_TYPE_MULTI_CH       = 9216,
      PIXEL_TYPE_JPEG           = 10240,
      PIXEL_TYPE_ANY            = 32765
    };

     enum  BufferType
    {
      BUFFER_TYPE_NOT_SPECIFIED = 0,
      BUFFER_TYPE_PIXEL_ALIGNED,
      BUFFER_TYPE_PIXEL_PACKED,
      BUFFER_TYPE_PLANAR_ALIGNED,
      BUFFER_TYPE_PLANAR_PACKED,
      BUFFER_TYPE_COMPRESSION   = 10240,
      BUFFER_TYPE_ANY           = 32765
    };

    enum  DataType
    {
      DATA_TYPE_NOT_SPECIFIED = 0,
      DATA_TYPE_1BIT          = 1,
      DATA_TYPE_4BIT          = 4,
      DATA_TYPE_8BIT          = 8,
      DATA_TYPE_10BIT         = 10,
      DATA_TYPE_12BIT         = 12,
      DATA_TYPE_14BIT         = 14,
      DATA_TYPE_16BIT         = 16,
      DATA_TYPE_24BIT         = 24,
      DATA_TYPE_32BIT         = 32,
      DATA_TYPE_40BIT         = 40,
      DATA_TYPE_48BIT         = 48,
      DATA_TYPE_54BIT         = 56,
      DATA_TYPE_64BIT         = 64,
      DATA_TYPE_FLOAT         = 512,
      DATA_TYPE_DOUBLE        = 1024
      DATA_TYPE_ANY           = 32765
    };

    enum  BayerType
    {
      BAYER_TYPE_NOT_SPECIFIED  = 0,
      BAYER_TYPE_GBRG,
      BAYER_TYPE_GRBG,
      BAYER_TYPE_BGGR,
      BAYER_TYPE_RGGB,
      BAYER_TYPE_ANY           = 32765
    };
  
    enum  ChannelType
    {
      CH_TYPE_NOT_SPECIFIED   = 0,
      CH_TYPE_LUMINANCE       = 1024,
      CH_TYPE_RED,
      CH_TYPE_GREEN,
      CH_TYPE_BLUE,
      CH_TYPE_WHITE,
      CH_TYPE_IR,
      CH_TYPE_ALPHA,
      CH_TYPE_CYAN,
      CH_TYPE_MAGENTA,
      CH_TYPE_YELLOW,
      CH_TYPE_KEY_PLATE,
      CH_TYPE_HUE,
      CH_TYPE_SATURATION,
      CH_TYPE_VALUE,
      CH_TYPE_L_STAR,
      CH_TYPE_U_STAR,
      CH_TYPE_V_STAR,
      CH_TYPE_A_STAR,
      CH_TYPE_B_STAR,
      CH_TYPE_Y,
      CH_TYPE_U,            // Cb
      CH_TYPE_V,            // Cr
      CH_TYPE_R_0           = 2048,
      CH_TYPE_R_1,
      CH_TYPE_G_0           = 2064,
      CH_TYPE_G_1,
      CH_TYPE_B_0           = 2080,
      CH_TYPE_B_1,
      CH_TYPE_W_0           = 2096,
      CH_TYPE_W_1,
      CH_TYPE_IR_0          = 2112,
      CH_TYPE_IR_1,
      CH_TYPE_Y_0           = 3072,
      CH_TYPE_Y_1,
      CH_TYPE_Y_3,
      CH_TYPE_Y_4,
      CH_TYPE_U_0           = 3088,
      CH_TYPE_U_1,
      CH_TYPE_V_0           = 3104,
      CH_TYPE_V_1,
      CH_TYPE_MULTI_0       = 4096,
      CH_TYPE_ANY           = 32765
    };

    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ImageType
    // -------------------------------------------------------------------------
    ImageType()
    {
      mPixelType              = PIXEL_TYPE_NOT_SPECIFIED;
      mBufferType             = BUFFER_TYPE_NOT_SPECIFIED;
      mDataType               = DATA_TYPE_NOT_SPECIFIED;
      mBayerType              = BAYER_TYPE_NOT_SPECIFIED;
      mFourCC                 = 0;
      mComponentsPerPixel     = 0;
    }
    // -------------------------------------------------------------------------
    // ImageType
    // -------------------------------------------------------------------------
    ImageType(PixelType inPixelType, BufferType inBufferType, DataType inDataType,
                BayerType inBayerType = BAYER_TYPE_NOT_SPECIFIED,
                uint32 inFourCC = 0,
                int inComponentsPerPixel = 0)
    {
      mPixelType              = inPixelType;
      mBufferType             = inBufferType;
      mDataType               = inDataType;
      mBayerType              = inBayerType;
      mFourCC                 = inFourCC;
      if (mComponentsPerPixel == 0)
        mComponentsPerPixel = coponentsPerPixel(inPixelType);
      else
        mComponentsPerPixel = inComponentsPerPixel;
    }
    // -------------------------------------------------------------------------
    // ~ImageType
    // -------------------------------------------------------------------------
    virtual ~ImageType()
    {
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // isValid
    // -------------------------------------------------------------------------
    bool  isValid()
    {
      if (mPixelType == PIXEL_TYPE_NOT_SPECIFIED)
        return false;
      if (mBufferType == BUFFER_TYPE_NOT_SPECIFIED)
        return false;
      if (mDataType == DATA_TYPE_NOT_SPECIFIED)
        return false;
      if (mBayerType == BAYER_TYPE_NOT_SPECIFIED)
        return false;
      if (mComponentsPerPixel < 0)
        return false;
      return true;
    }
    // Member variables --------------------------------------------------------
    PixelType       mPixelType;
    BufferType      mBufferType;
    DataType        mDataType;
    BayerType       mBayerType;

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // retrieveNativeMonoFormat
    // -------------------------------------------------------------------------
    static ImageType  retrieveNativeMonoImageType()
    {
      return ImageType(PIXEL_TYPE_MONO, BUFFER_TYPE_PIXEL_ALIGNED, DATA_TYPE_8BIT);
    }
    // -------------------------------------------------------------------------
    // retrieveNativeColorImageType
    // -------------------------------------------------------------------------
    static ImageType  retrieveNativeColorImageType()
    {
      return ImageType(PIXEL_TYPE_BGR, BUFFER_TYPE_PIXEL_ALIGNED, DATA_TYPE_8BIT);
    }
    // -------------------------------------------------------------------------
    // retrieveNativeColorAlphaImageType
    // -------------------------------------------------------------------------
    static ImageType  retrieveNativeColorAlphaImageType()
    {
      return ImageType(PIXEL_TYPE_BGRA, BUFFER_TYPE_PIXEL_ALIGNED, DATA_TYPE_8BIT);
    }
    // -------------------------------------------------------------------------
    // coponentsPerPixel
    // -------------------------------------------------------------------------
    static int  coponentsPerPixel(PixelType inType)
    {
      switch (inType)
      {
        case PIXEL_TYPE_RAW:
        case PIXEL_TYPE_MONO:
        case PIXEL_TYPE_BAYER:
          return 1;
        case PIXEL_TYPE_RGB:
        case PIXEL_TYPE_BGR:
        case PIXEL_TYPE_CMY:
        case PIXEL_TYPE_HSL:
        case PIXEL_TYPE_HSB:
        case PIXEL_TYPE_LUV:
        case PIXEL_TYPE_LAB:
        case PIXEL_TYPE_YUV444:
          return 3;
        case PIXEL_TYPE_RGBA:
        case PIXEL_TYPE_BGRA:
        case PIXEL_TYPE_CMYK:
          return 4;
      }
      return 0;
    }
    // -------------------------------------------------------------------------
    // sizeOfData
    // -------------------------------------------------------------------------
    static size_t  sizeOfData(DataType inType)
    {
      switch (inType)
      {
        case DATA_TYPE_8BIT:
          return 1;
        case DATA_TYPE_16BIT:
          return 2;
        case DATA_TYPE_24BIT:
          return 3;
        case DATA_TYPE_32BIT:
          return 4;
        case DATA_TYPE_40BIT:
          return 5;
        case DATA_TYPE_48BIT:
          return 6;
        case DATA_TYPE_54BIT:
          return 7;
        case DATA_TYPE_64BIT:
          return 8;
        case DATA_TYPE_FLOAT:
          return 4;
        case DATA_TYPE_DOUBLE:
          return 8;
      }
      return 0;
    }
    // -------------------------------------------------------------------------
    // isPlanar
    // -------------------------------------------------------------------------
    static bool isPlanar(BufferType inBufferType)
    {
      if (inBufferType == BUFFER_TYPE_PLANAR_ALIGNED ||
          inBufferType == BUFFER_TYPE_PLANAR_PACKED)
        return true;

      return false;
    }
  }

  // ---------------------------------------------------------------------------
  // ImageFormat class
  // ---------------------------------------------------------------------------
  class  ImageFormat
  {
  public:
    // -------------------------------------------------------------------------
    // ImageFormat
    // -------------------------------------------------------------------------
    ImageFormat(ImageType inType,
                uint32 inWidth, uint32 inHeight,
                bool inIsBottomUp = false,
                size_t inBufferSize = 0;
                size_t inHeaderOffset = 0,
                size_t inPixelStep = 0,  size_t inWidthStep = 0,
                size_t inPlaneStep = 0)
    {
      mType           = inType;
      mWidth          = inWidth;
      mHeight         = inHeight;
      mIsBottomUp     = inIsBottomUp;
      mBufferSize     = inBufferSize;
      mHeaderOffset   = inHeaderOffset;
      mPixelStep      = inPixelStep;
      mWidthStep      = inWidthStep;
      mPlaneStep      = inPlaneStep;
      if (mBufferSize == 0)
        mBufferSize = calculateBufferSize(this);
    }
    // -------------------------------------------------------------------------
    // ~ImageFormat
    // -------------------------------------------------------------------------
    virtual ~ImageFormat()
    {
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // calculateBufferSize
    // -------------------------------------------------------------------------
    static size_t calculateBufferSize(ImageFormat inHeader)
    {
      size_t  size;

      if (mPlaneStep != 0)
      {
        size  = ImageType::coponentsPerPixel(inHeader.mFormat.mPixelType);
        size *= mPlaneStep;
        size += mHeaderOffset;
        return size;
      }
      if (mWidthStep != 0)
      {
        size  =  inHeader.mSize.mHeight * mWidthStep;
        if (ImageType::isPlanar(inHeader.mFormat.mBufferType))
          size  *= ImageType::coponentsPerPixel(inHeader.mFormat.mPixelType);
        size += mHeaderOffset;
        return size;
      }
      if (mPixelStep != 0)
      {
        size  =  inHeader.mSize.mWidth * inHeader.mSize.mPixelStep;
        size  *= inHeader.mSize.mHeight;
        if (ImageType::isPlanar(inHeader.mFormat.mBufferType))
          size  *= ImageType::coponentsPerPixel(inHeader.mFormat.mPixelType);
        size += mHeaderOffset;
        return size;
      }
      size =  ImageType::sizeOfData(inHeader.mFormat.mDataType);
      size *= ImageType::coponentsPerPixel(mFormat.mPixelType);
      size *= inHeader.mSize.mWidth;
      size *= inHeader.mSize.mHeight;
      return size;
    }

    // Member variables --------------------------------------------------------
    ImageType       mType;
    uint32          mFourCC;
    unsigned int    mComponentsPerPixel;
    uint32          mWidth;
    uint32          mHeight;
    bool            mIsBottomUp;
    size_t          mBufferSize;
    size_t          mHeaderOffset;
    size_t          mPixelStep;
    size_t          mWidthStep;
    size_t          mPlaneStep;
  }
 };
};

#endif  // #ifdef IBC_IMAGE_IMAGE_H_

