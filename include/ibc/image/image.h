// =============================================================================
//  image.h
//
//  MIT License
//
//  Copyright (c) 2018-2019 Dairoku Sekiguchi
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
#include <string.h>
#include "ibc/base/types.h"
#include "ibc/base/endian.h"

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
      DATA_TYPE_DOUBLE        = 1024,
      DATA_TYPE_ANY           = 32765
    };
  
    enum  EndianType
    {
      ENDIAN_TYPE_NOT_SPECIFIED = 0,
      ENDIAN_LITTLE,
      ENDIAN_BIG,
      ENDIAN_TYPE_HOST          = 32764,
      ENDIAN_TYPE_ANY           = 32765
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

    // Member variables (public) -----------------------------------------------
    PixelType       mPixelType;
    BufferType      mBufferType;
    DataType        mDataType;
    EndianType      mEndian;
    BayerType       mBayerType;
    uint32          mFourCC;
    int             mComponentsPerPixel;

    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ImageType
    // -------------------------------------------------------------------------
    ImageType()
    {
      mPixelType              = PIXEL_TYPE_NOT_SPECIFIED;
      mBufferType             = BUFFER_TYPE_NOT_SPECIFIED;
      mDataType               = DATA_TYPE_NOT_SPECIFIED;
      mEndian                 = ENDIAN_TYPE_NOT_SPECIFIED;
      mBayerType              = BAYER_TYPE_NOT_SPECIFIED;
      mFourCC                 = 0;
      mComponentsPerPixel     = 0;
    }
    // -------------------------------------------------------------------------
    // ImageType
    // -------------------------------------------------------------------------
    ImageType(PixelType inPixelType, BufferType inBufferType, DataType inDataType,
                BayerType inBayerType = BAYER_TYPE_NOT_SPECIFIED,
                EndianType inEndian = ENDIAN_TYPE_HOST,
                uint32 inFourCC = 0,
                int inComponentsPerPixel = 0)
    {
      mPixelType              = inPixelType;
      mBufferType             = inBufferType;
      mDataType               = inDataType;
      mBayerType              = inBayerType;
      mFourCC                 = inFourCC;
      if (inEndian == ENDIAN_TYPE_HOST)
        mEndian               = getHostEndian();
      else
        mEndian               = inEndian;
      if (inComponentsPerPixel == 0)
        mComponentsPerPixel   = coponentsPerPixel(inPixelType);
      else
        mComponentsPerPixel   = inComponentsPerPixel;
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
    bool  isValid() const
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
    // -------------------------------------------------------------------------
    // isPlanar
    // -------------------------------------------------------------------------
    bool isPlanar() const
    {
      return isPlanar(mBufferType);
    }
    // -------------------------------------------------------------------------
    // isPacked
    // -------------------------------------------------------------------------
    bool isPacked() const
    {
      return isPacked(mBufferType);
    }
    // -------------------------------------------------------------------------
    // sizeOfData
    // -------------------------------------------------------------------------
    size_t  sizeOfData() const
    {
      return sizeOfData(mDataType);
    }
    // -------------------------------------------------------------------------
    // checkType
    // -------------------------------------------------------------------------
    bool checkType(PixelType inPixelType, BufferType inBufferType, DataType inDataType) const
    {
      return checkType(*this, inPixelType, inBufferType, inDataType);
    }

    // Static Functions --------------------------------------------------------
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
    // -------------------------------------------------------------------------
    // isPacked
    // -------------------------------------------------------------------------
    static bool isPacked(BufferType inBufferType)
    {
      if (inBufferType == BUFFER_TYPE_PIXEL_PACKED ||
          inBufferType == BUFFER_TYPE_PLANAR_PACKED)
        return true;
      return false;
    }
    // -------------------------------------------------------------------------
    // checkType
    // -------------------------------------------------------------------------
    static bool checkType(const ImageType &inType, PixelType inPixelType, BufferType inBufferType, DataType inDataType)
    {
      if (inPixelType != PIXEL_TYPE_ANY && inType.mPixelType != inPixelType)
        return false;
      if (inBufferType != BUFFER_TYPE_ANY && inType.mBufferType != inBufferType)
        return false;
      if (inDataType != DATA_TYPE_ANY && inType.mDataType != inDataType)
        return false;
      return true;
    }
    // -------------------------------------------------------------------------
    // getHostEndian
    // -------------------------------------------------------------------------
    static EndianType getHostEndian()
    {
#ifdef __LITTLE_ENDIAN__
        return ENDIAN_LITTLE;
#else
        return ENDIAN_BIG;
#endif
    }
    // -------------------------------------------------------------------------
    // stringToPixelType
    // -------------------------------------------------------------------------
    static PixelType stringToPixelType(const char *inString, PixelType inDefault = PIXEL_TYPE_NOT_SPECIFIED)
    {
      const PixelTypeTable  *tablePtr = getPixelTypeTable();
      while (tablePtr->type != PIXEL_TYPE_NOT_SPECIFIED)
      {
#ifndef WIN32
        if (::strncasecmp(inString, tablePtr->str, ::strlen(tablePtr->str)) == 0)
#else
        if (::stricmp(inString, tablePtr->str) == 0)
#endif
          return tablePtr->type;
        tablePtr++;
      }
      return inDefault;
    }
    // -------------------------------------------------------------------------
    // stringToDataType
    // -------------------------------------------------------------------------
    static DataType stringToDataType(const char *inString, DataType inDefault = DATA_TYPE_NOT_SPECIFIED)
    {
      const DataTypeTable  *tablePtr = getDataTypeTable();
      while (tablePtr->type != DATA_TYPE_NOT_SPECIFIED)
      {
#ifndef WIN32
        if (::strncasecmp(inString, tablePtr->str, ::strlen(tablePtr->str)) == 0)
#else
        if (::stricmp(inString, tablePtr->str) == 0)
#endif
          return tablePtr->type;
        tablePtr++;
      }
      return inDefault;
    }

  private:
    // Typedefs  ---------------------------------------------------------------
    typedef struct
    {
      PixelType type;
      const char  *str;
    } PixelTypeTable;
    typedef struct
    {
      DataType type;
      const char  *str;
    } DataTypeTable;

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // getPixelTypeTable
    // -------------------------------------------------------------------------
    static const PixelTypeTable *getPixelTypeTable()
    {
      const static PixelTypeTable  table[] =
      {
        {PIXEL_TYPE_RAW, "RAW"},
        {PIXEL_TYPE_MONO, "MONO"},
        {PIXEL_TYPE_BAYER, "BAYER"},
        {PIXEL_TYPE_RGB, "RGB"},
        {PIXEL_TYPE_RGBA, "RGBA"},
        {PIXEL_TYPE_BGR, "BGR"},
        {PIXEL_TYPE_BGRA, "BGRA"},
        {PIXEL_TYPE_CMY, "CMY"},
        {PIXEL_TYPE_CMYK, "CMYK"},
        {PIXEL_TYPE_HSL, "HSL"},
        {PIXEL_TYPE_HSB, "HSB"},
        {PIXEL_TYPE_LUV, "LUV"},
        {PIXEL_TYPE_LAB, "LAB"},
        {PIXEL_TYPE_YUV410, "YUV410"},
        {PIXEL_TYPE_YUV411, "YUV411"},
        {PIXEL_TYPE_YUV420, "YUV420"},
        {PIXEL_TYPE_YUV422, "YUV422"},
        {PIXEL_TYPE_FOURCC, "FORCC"},
        {PIXEL_TYPE_MULTI_CH, "MULTI_CH"},
        {PIXEL_TYPE_JPEG, "JPEG"},
        {PIXEL_TYPE_ANY, "ANY"},
        {PIXEL_TYPE_NOT_SPECIFIED, ""},
      };
      return table;
    }
    // -------------------------------------------------------------------------
    // getDataTypeTable
    // -------------------------------------------------------------------------
    static const DataTypeTable *getDataTypeTable()
    {
      const static DataTypeTable  table[] =
      {
        {DATA_TYPE_1BIT, "1BIT"},
        {DATA_TYPE_4BIT, "4BIT"},
        {DATA_TYPE_8BIT, "8BIT"},
        {DATA_TYPE_10BIT, "10BIT"},
        {DATA_TYPE_12BIT, "12BIT"},
        {DATA_TYPE_14BIT, "14BIT"},
        {DATA_TYPE_16BIT, "16BIT"},
        {DATA_TYPE_24BIT, "24BIT"},
        {DATA_TYPE_32BIT, "32BIT"},
        {DATA_TYPE_40BIT, "40BIT"},
        {DATA_TYPE_48BIT, "48BIT"},
        {DATA_TYPE_54BIT, "54BIT"},
        {DATA_TYPE_64BIT, "64BIT"},
        {DATA_TYPE_FLOAT, "FLOAT"},
        {DATA_TYPE_DOUBLE, "DOUBLE"},
        {DATA_TYPE_ANY, "ANY"},
        {DATA_TYPE_NOT_SPECIFIED, ""}
      };
      return table;
    }
  };

  // ---------------------------------------------------------------------------
  // ImageFormat class
  // ---------------------------------------------------------------------------
  class  ImageFormat
  {
  public:
    // Member variables --------------------------------------------------------
    ImageType       mType;
    int             mWidth;
    int             mHeight;
    bool            mIsBottomUp;
    size_t          mBufferSize;
    size_t          mHeaderOffset;
    size_t          mPixelStep;
    size_t          mWidthStep;
    size_t          mPlaneStep;
    size_t          mPixelAreaSize;

    // -------------------------------------------------------------------------
    // ImageFormat
    // -------------------------------------------------------------------------
    ImageFormat(const ImageType &inType,
                uint32 inWidth, uint32 inHeight,
                bool inIsBottomUp = false,
                size_t inBufferSize = 0,
                size_t inHeaderOffset = 0,
                size_t inPixelStep = 0,
                size_t inWidthStep = 0,
                size_t inPlaneStep = 0)
    {
      mType           = inType;
      mWidth          = inWidth;
      mHeight         = inHeight;
      mIsBottomUp     = inIsBottomUp;
      mHeaderOffset   = inHeaderOffset;
      if (inPixelStep != 0)
        mPixelStep = inPixelStep; // TODO: Add a sanity check here...
      else
      {
        if (mType.isPacked())
          mPixelStep = 0;
        else
          mPixelStep = mType.sizeOfData();
      }
      if (inWidthStep != 0)
        mWidthStep = inWidthStep; // TODO: Add a sanity check here...
      else
      {
        mWidthStep = mPixelStep * mWidth;
        if (mType.isPlanar() == false)
          mWidthStep *= mType.mComponentsPerPixel;
      }
      if (inPlaneStep != 0)
        mPlaneStep = inPlaneStep;  // TODO: Add a sanity check here...
      else
        mPlaneStep = mWidthStep * mHeight;
      //
      if (mType.isPlanar())
        mPixelAreaSize = mPlaneStep * mType.mComponentsPerPixel;
      else
        mPixelAreaSize = mPlaneStep;
      //
      if (inBufferSize != 0)
        mBufferSize = inBufferSize;
      else
        mBufferSize = mHeaderOffset + mPixelAreaSize;
    }
    // -------------------------------------------------------------------------
    // ~ImageFormat
    // -------------------------------------------------------------------------
    virtual ~ImageFormat()
    {
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // getPlaneOffset
    // -------------------------------------------------------------------------
    size_t getPlaneOffset(unsigned int inPlaneIndex = 0) const
    {
      calculatePlaneOffset(*this, inPlaneIndex);
    }
    // -------------------------------------------------------------------------
    // getPlanePtr
    // -------------------------------------------------------------------------
    void *getPlanePtr(void *inBufferPtr, unsigned int inPlaneIndex = 0) const
    {
      return ((unsigned char *)inBufferPtr) + calculatePlaneOffset(*this, inPlaneIndex);
    }
    // -------------------------------------------------------------------------
    // getPlanePtr
    // -------------------------------------------------------------------------
    const void *getPlanePtr(const void *inBufferPtr, unsigned int inPlaneIndex = 0) const
    {
      return ((const unsigned char *)inBufferPtr) + calculatePlaneOffset(*this, inPlaneIndex);
    }
    // -------------------------------------------------------------------------
    // getLineOffset
    // -------------------------------------------------------------------------
    size_t getLineOffset(unsigned int inY = 0, unsigned int inPlaneIndex = 0) const
    {
      calculateLineOffset(*this, inY, inPlaneIndex);
    }
    // -------------------------------------------------------------------------
    // getLinePtr
    // -------------------------------------------------------------------------
    void *getLinePtr(void *inBufferPtr, unsigned int inY, unsigned int inPlaneIndex = 0) const
    {
      return ((unsigned char *)inBufferPtr) + calculateLineOffset(*this, inY, inPlaneIndex);
    }
    // -------------------------------------------------------------------------
    // getLinePtr
    // -------------------------------------------------------------------------
    const void *getLinePtr(const void *inBufferPtr, unsigned int inY, unsigned int inPlaneIndex = 0) const
    {
      return ((const unsigned char *)inBufferPtr) + calculateLineOffset(*this, inY, inPlaneIndex);
    }
    // -------------------------------------------------------------------------
    // getPixelOffset
    // -------------------------------------------------------------------------
    size_t getPixelOffset(unsigned int inX = 0, unsigned int inY = 0, unsigned int inPlaneIndex = 0) const
    {
      return calculatePixelOffset(*this, inX, inY, inPlaneIndex);
    }
    // -------------------------------------------------------------------------
    // getPixelPtr
    // -------------------------------------------------------------------------
    void *getPixelPtr(void *inBufferPtr, unsigned int inX, unsigned int inY, unsigned int inPlaneIndex = 0) const
    {
      return ((unsigned char *)inBufferPtr) + calculatePixelOffset(*this, inX, inY, inPlaneIndex);
    }
    // -------------------------------------------------------------------------
    // getPixelPtr
    // -------------------------------------------------------------------------
    const void *getPixelPtr(const void *inBufferPtr, unsigned int inX, unsigned int inY, unsigned int inPlaneIndex = 0) const
    {
      return ((const unsigned char *)inBufferPtr) + calculatePixelOffset(*this, inX, inY, inPlaneIndex);
    }
    // -------------------------------------------------------------------------
    // getPixelPtr
    // -------------------------------------------------------------------------
    void *getPixelPtr(void *inBufferPtr) const
    {
      return getPixelPtr(*this, inBufferPtr);
    }
    // -------------------------------------------------------------------------
    // getPixelOffset
    // -------------------------------------------------------------------------
    const void *getPixelPtr(const void *inBufferPtr) const
    {
      return getPixelPtr(*this, inBufferPtr);
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // getPixelPtr
    // -------------------------------------------------------------------------
    static void *getPixelPtr(const ImageFormat &inFormat, void *inBufferPtr)
    {
      unsigned char *pixelPtr = (unsigned char *)inBufferPtr;
      pixelPtr += inFormat.mHeaderOffset;
      return pixelPtr;
    }
    // -------------------------------------------------------------------------
    // getPixelPtr
    // -------------------------------------------------------------------------
    static const void *getPixelPtr(const ImageFormat &inFormat, const void *inBufferPtr)
    {
      unsigned char *pixelPtr = (unsigned char *)inBufferPtr;
      pixelPtr += inFormat.mHeaderOffset;
      return pixelPtr;
    }
    // -------------------------------------------------------------------------
    // calculatePlaneOffset
    // -------------------------------------------------------------------------
    static size_t calculatePlaneOffset(const ImageFormat &inFormat, unsigned int inPlaneIndex = 0)
    {
      if (inFormat.mType.isPlanar() == false)
        return inFormat.mHeaderOffset;
      if (inPlaneIndex >= inFormat.mType.mComponentsPerPixel)
        inPlaneIndex = inFormat.mType.mComponentsPerPixel - 1;
      return inFormat.mPlaneStep * inPlaneIndex + inFormat.mHeaderOffset;
    }
    // -------------------------------------------------------------------------
    // calculateLineOffset
    // -------------------------------------------------------------------------
    static size_t calculateLineOffset(const ImageFormat &inFormat, unsigned int inY = 0,
                                      unsigned int inPlaneIndex = 0)
    {
      size_t  offset;

      if (inY >= inFormat.mHeight)
        inY = inFormat.mHeight - 1;
      offset = calculatePlaneOffset(inFormat, inPlaneIndex);
      if (inY == 0)
        return offset;
      offset +=  inFormat.mWidthStep * inY;
      return offset;
    }
    // -------------------------------------------------------------------------
    // calculatePixelOffset
    // -------------------------------------------------------------------------
    static size_t calculatePixelOffset(const ImageFormat &inFormat, unsigned int inX  = 0,
                                       unsigned int inY = 0, unsigned int inPlaneIndex = 0)
    {
      size_t  offset;

      if (inY >= inFormat.mWidth)
        inY = inFormat.mWidth - 1;
      offset = calculateLineOffset(inFormat, inY, inPlaneIndex);
      offset +=  inFormat.mPixelStep * inX;
      return offset;
    }
  };
 };
};

#endif  // #ifdef IBC_IMAGE_IMAGE_H_

