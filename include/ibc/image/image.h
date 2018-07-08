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
  // ImageFormat class
  // ---------------------------------------------------------------------------
  class  ImageFormat
  {
  public:
    // Enum --------------------------------------------------------------------
    enum  ImageType
    {
      IMAGE_TYPE_NOT_SPECIFIED  = 0,
      IMAGE_TYPE_RAW            = 1024,
      IMAGE_TYPE_MONO           = 2048,
      IMAGE_TYPE_BAYER          = 3072,
      IMAGE_TYPE_RGB            = 4096,
      IMAGE_TYPE_RGBA,
      IMAGE_TYPE_BGR,
      IMAGE_TYPE_BGRA,
      IMAGE_TYPE_CMY            = 5120,
      IMAGE_TYPE_CMYK,
      IMAGE_TYPE_HSL            = 6144,
      IMAGE_TYPE_HSB,
      IMAGE_TYPE_LUV,
      IMAGE_TYPE_LAB,
      IMAGE_TYPE_YUV410         = 7168, // YUV9
      IMAGE_TYPE_YUV411,
      IMAGE_TYPE_YUV420,                // YUV12
      IMAGE_TYPE_YUV422,
      IMAGE_TYPE_YUV444,
      IMAGE_TYPE_FOURCC         = 8192,
      IMAGE_TYPE_MULTI_CH       = 9216,
      IMAGE_TYPE_JPEG           = 10240,
      IMAGE_TYPE_NAITIVE        = 16384,
      IMAGE_TYPE_NAITIVE_MONO,
      IMAGE_TYPE_NAITIVE_COLOR,
      IMAGE_TYPE_NAITIVE_COLOR_ALPHA,
      IMAGE_TYPE_ANY            = 32765
    };

     enum  BufferFormat
    {
      BUFFER_FORMAT_NOT_SPECIFIED = 0,
      BUFFER_FORMAT_PIXEL_ALIGNED,
      BUFFER_FORMAT_PIXEL_PACKED,
      BUFFER_FORMAT_PLANAR_ALIGNED,
      BUFFER_FORMAT_PLANAR_PACKED,
      BUFFER_FORMAT_COMPRESSION   = 10240,
      BUFFER_FORMAT_ANY           = 32765
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
      DATA_TYPE_48BIT         = 48,
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
      CH_TYPE_NOT_SPECIFIED = 0,
      CH_TYPE_LUMINANCE     = 1024,
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
      CH_TYPE_U,          // Cb
      CH_TYPE_V,          // Cr
      CH_TYPE_R_0         = 2048,
      CH_TYPE_R_1,
      CH_TYPE_G_0         = 2064,
      CH_TYPE_G_1,
      CH_TYPE_B_0         = 2080,
      CH_TYPE_B_1,
      CH_TYPE_W_0         = 2096,
      CH_TYPE_W_1,
      CH_TYPE_IR_0        = 2112,
      CH_TYPE_IR_1,
      CH_TYPE_Y_0         = 3072,
      CH_TYPE_Y_1,
      CH_TYPE_Y_3,
      CH_TYPE_Y_4,
      CH_TYPE_U_0         = 3088,
      CH_TYPE_U_1,
      CH_TYPE_V_0         = 3104,
      CH_TYPE_V_1,
      CH_TYPE_MULTI_0     = 4096,
      CH_TYPE_ANY         = 32765
    };

    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ImageBuffer
    // -------------------------------------------------------------------------
    ImageFormat()
    {
      mImageType              = IMAGE_TYPE_NOT_SPECIFIED;
      mBufferFormat           = BUFFER_FORMAT_NOT_SPECIFIED;
      mDataType               = DATA_TYPE_NOT_SPECIFIED;
      mBayerType              = BAYER_TYPE_NOT_SPECIFIED;
      mFourCC                 = 0;
      mIsBottomUp             = false;
      mComponentsPerPixel     = 0;
    }
    // -------------------------------------------------------------------------
    // ImageBuffer
    // -------------------------------------------------------------------------
    ImageFormat(ImageType inImageType, BufferFormat inBufferFormat, DataType inDataType,
                BayerType inBayerType = BAYER_TYPE_NOT_SPECIFIED,
                uint32 inFourCC = 0, bool inIsBottomUp = false,
                int inComponentsPerPixel = -1)
    {
      mImageType              = inImageType;
      mBufferFormat           = inBufferFormat;
      mDataType               = inDataType;
      mBayerType              = inBayerType;
      mFourCC                 = inFourCC;
      mIsBottomUp             = inIsBottomUp;
      if (mComponentsPerPixel < 0)
        mComponentsPerPixel = retrieveCoponentsPerPixel(inImageType);
      else
        mComponentsPerPixel = inComponentsPerPixel;
    }
    // -------------------------------------------------------------------------
    // ~ImageFormat
    // -------------------------------------------------------------------------
    virtual ~ImageFormat()
    {
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // isValid
    // -------------------------------------------------------------------------
    bool  isValid()
    {
      if (mImageType == IMAGE_TYPE_NOT_SPECIFIED)
        return false;
      if (mBufferFormat == BUFFER_FORMAT_NOT_SPECIFIED)
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
    ImageType       mImageType;
    BufferFormat    mBufferFormat;
    DataType        mDataType;
    BayerType       mBayerType;
    uint32          mFourCC;
    bool            mIsBottomUp;
    int             mComponentsPerPixel;

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // retrieveNativeImageType
    // -------------------------------------------------------------------------
    static ImageFormat  &retrieveNativeImageFormat(ImageType inType = IMAGE_TYPE_NAITIVE)
    {
      switch (inType)
      {
        case IMAGE_TYPE_NAITIVE_MONO:
          return retrieveNativeMonoImageFormat;
        case IMAGE_TYPE_NAITIVE:
        case IMAGE_TYPE_NAITIVE_COLOR:
          return retrieveNativeColorImageFormat();
        case IMAGE_TYPE_NAITIVE_COLOR_ALPHA:
          return retrieveNativeColorAlphaImageFormat();
      }
      return retrieveNativeColorImageFormat();
    }
    // -------------------------------------------------------------------------
    // retrieveNativeMonoFormat
    // -------------------------------------------------------------------------
    static ImageFormat  &retrieveNativeMonoImageFormat()
    {
      return ImageFormat(IMAGE_TYPE_MONO, BUFFER_FORMAT_PIXEL_ALIGNED, DATA_TYPE_8BIT);
    }
    // -------------------------------------------------------------------------
    // retrieveNativeColorImageFormat
    // -------------------------------------------------------------------------
    static ImageFormat  &retrieveNativeColorImageFormat()
    {
      return ImageFormat(IMAGE_TYPE_BGR, BUFFER_FORMAT_PIXEL_ALIGNED, DATA_TYPE_8BIT);
    }
    // -------------------------------------------------------------------------
    // retrieveNativeColorAlphaImageFormat
    // -------------------------------------------------------------------------
    static ImageFormat  &retrieveNativeColorAlphaImageFormat()
    {
      return ImageFormat(IMAGE_TYPE_BGRA, BUFFER_FORMAT_PIXEL_ALIGNED, DATA_TYPE_8BIT);
    }
    // -------------------------------------------------------------------------
    // obtainOnePixelCount
    // -------------------------------------------------------------------------
    static int  retrieveCoponentsPerPixel(ImageType inType)
    {
      switch (inType)
      {
        case IMAGE_TYPE_RAW:
        case IMAGE_TYPE_MONO:
        case IMAGE_TYPE_BAYER:
          return 1;
        case IMAGE_TYPE_RGB:
        case IMAGE_TYPE_BGR:
        case IMAGE_TYPE_CMY:
        case IMAGE_TYPE_HSL:
        case IMAGE_TYPE_HSB:
        case IMAGE_TYPE_LUV:
        case IMAGE_TYPE_LAB:
        case IMAGE_TYPE_YUV444:
          return 3;
        case IMAGE_TYPE_RGBA:
        case IMAGE_TYPE_BGRA:
        case IMAGE_TYPE_CMYK:
          return 4;
      }
      return 0;
    }
  }

  // ---------------------------------------------------------------------------
  // ImageSize class
  // ---------------------------------------------------------------------------
  class  ImageSize
  {
  public:
    // -------------------------------------------------------------------------
    // ImageBuffer
    // -------------------------------------------------------------------------
    ImageSize()
    {
      mWidth                  = 0;
      mHeight                 = 0;
      mWidthStep              = 0;
      mPlaneStep              = 0;
    }
    // -------------------------------------------------------------------------
    // ImageBuffer
    // -------------------------------------------------------------------------
    ImageSize(uint32 inWidth, uint32 inHeight,
                size_t inWidthStep, size_t inPlaneStep)
    {
      mWidth                  = inWidth;
      mHeight                 = inHeight;
      mWidthStep              = inWidthStep;
      mPlaneStep              = inPlaneStep;
    }
    // -------------------------------------------------------------------------
    // ~ImageFormat
    // -------------------------------------------------------------------------
    virtual ~ImageSize()
    {
    }

    // Member variables --------------------------------------------------------
    uint32          mWidth;
    uint32          mHeight;
    size_t          mWidthStep;
    size_t          mPlaneStep;
//  uint32          mImageBufferPixelCount;
  }

 };
};

#endif  // #ifdef IBC_IMAGE_IMAGE_H_

