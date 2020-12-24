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
      PIXEL_TYPE_RAW            = 0x10,
      PIXEL_TYPE_MONO,
      PIXEL_TYPE_BAYER_GBRG,
      PIXEL_TYPE_BAYER_GRBG,
      PIXEL_TYPE_BAYER_BGGR,
      PIXEL_TYPE_BAYER_RGGB,
      // TODO: Add followings
      //PIXEL_TYPE_BAYER_YCGM,
      //PIXEL_TYPE_BAYER_CYMG,
      //PIXEL_TYPE_BAYER_GMYC,
      //PIXEL_TYPE_BAYER_MGCY,
      // RGBW, RGBE, EXR, CYGW, XTRANS...
      //
      PIXEL_TYPE_RGB            = 0x2000,
      PIXEL_TYPE_BGR,
      PIXEL_TYPE_RGBA,
      PIXEL_TYPE_ARGB,
      PIXEL_TYPE_BGRA,
      PIXEL_TYPE_ABGR,
      PIXEL_TYPE_CMY            = 0x3000,
      PIXEL_TYPE_CMYK,
      PIXEL_TYPE_HSL            = 0x4000,
      PIXEL_TYPE_HSV,                       // HSB
      PIXEL_TYPE_HSI,
      PIXEL_TYPE_LUV,
      PIXEL_TYPE_LAB,
      PIXEL_TYPE_LCHAB,
      PIXEL_TYPE_LCHUV,
      PIXEL_TYPE_DIN99,
      PIXEL_TYPE_DIN99D,
      PIXEL_TYPE_DIN99O,
      PIXEL_TYPE_YUV410         = 0x5000,   // YUV9
      PIXEL_TYPE_YUV411,
      PIXEL_TYPE_YUV420,                    // YUV12
      PIXEL_TYPE_YUV422,
      PIXEL_TYPE_YUV444,
      PIXEL_TYPE_MULTI_CH       = 0x6000,
      PIXEL_TYPE_MULTI_CH_MONO,
      PIXEL_TYPE_MULTI_CH_RGB,
      PIXEL_TYPE_MULTI_CH_RGBA,
      PIXEL_TYPE_FOURCC         = 0x7000,
      PIXEL_TYPE_COMPRESSED     = 0x8000,
      PIXEL_TYPE_JPEG,
      PIXEL_TYPE_ANY            = 0xFFFF
    };
  
     enum  BufferType
    {
      BUFFER_TYPE_NOT_SPECIFIED                 = 0,
      BUFFER_TYPE_PIXEL_ALIGNED,
      BUFFER_TYPE_PIXEL_PACKED,
      BUFFER_TYPE_PIXEL_PACKED_CSI_2,
      BUFFER_TYPE_PLANAR_ALIGNED                = 0x1000,
      BUFFER_TYPE_PLANAR_PACKED,
      BUFFER_TYPE_PLANAR_PACKED_CSI_2,
      BUFFER_TYPE_LINE_INTERLEAVE_ALIGNED       = 0x2000,
      BUFFER_TYPE_LINE_INTERLEAVE_PACKED,
      BUFFER_TYPE_LINE_INTERLEAVE_PACKED_CSI_2,
      BUFFER_TYPE_INTRA_LINE_ALIGNED            = 0x3000,
      BUFFER_TYPE_INTRA_LINE_PACKED,
      BUFFER_TYPE_INTRA_LINE_PACKED_CSI_2,
      //
      BUFFER_TYPE_COMPRESSION                   = 0x8000,
      BUFFER_TYPE_ANY                           = 0xFFFF
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
      DATA_TYPE_56BIT         = 56,
      DATA_TYPE_64BIT         = 64,     // 0x40
      DATA_TYPE_FLOAT         = 0x200,  // 512
      DATA_TYPE_DOUBLE        = 0x400,  // 1024
      //
      DATA_TYPE_SIGNED_OFFSET = 0x1000,   // 4096
      DATA_TYPE_4BIT_SIGNED   = 4100,
      DATA_TYPE_8BIT_SIGNED   = 4104,
      DATA_TYPE_10BIT_SIGNED  = 4106,
      DATA_TYPE_12BIT_SIGNED  = 4108,
      DATA_TYPE_14BIT_SIGNED  = 4110,
      DATA_TYPE_16BIT_SIGNED  = 4112,
      DATA_TYPE_24BIT_SIGNED  = 4120,
      DATA_TYPE_32BIT_SIGNED  = 4128,
      DATA_TYPE_40BIT_SIGNED  = 4136,
      DATA_TYPE_48BIT_SIGNED  = 4144,
      DATA_TYPE_56BIT_SIGNED  = 4152,
      DATA_TYPE_64BIT_SIGNED  = 4160,   // 0x1040
      //
      DATA_TYPE_ANY           = 0xFFFF
    };
  
    enum  EndianType
    {
      ENDIAN_TYPE_NOT_SPECIFIED = 0,
      ENDIAN_LITTLE,
      ENDIAN_BIG,
      ENDIAN_TYPE_HOST,
      ENDIAN_TYPE_ANY           = 0xFFFF
    };

    enum  ChannelType       // We are not using this for now...
    {
      CH_TYPE_NOT_SPECIFIED   = 0,
      CH_TYPE_RAW             = 0x1000,
      CH_TYPE_DATA,
      CH_TYPE_LUMINANCE       = 0x2000,
      CH_TYPE_RED             = 0x4000,
      CH_TYPE_GREEN,
      CH_TYPE_BLUE,
      CH_TYPE_ALPHA,
      CH_TYPE_WHITE,
      CH_TYPE_IR,
      CH_TYPE_CYAN            = 0x5000,
      CH_TYPE_MAGENTA,
      CH_TYPE_YELLOW,
      CH_TYPE_KEY_PLATE,
      CH_TYPE_HUE             = 0x6000,
      CH_TYPE_SATURATION,
      CH_TYPE_VALUE,
      CH_TYPE_L_STAR,
      CH_TYPE_U_STAR,
      CH_TYPE_V_STAR,
      CH_TYPE_A_STAR,
      CH_TYPE_B_STAR,
      CH_TYPE_Y               = 0x7000,
      CH_TYPE_U,
      CH_TYPE_V,
      //
      CH_TYPE_MULTI_0         = 0x8000,
      CH_TYPE_MULTI_1,
      CH_TYPE_MULTI_2,
      CH_TYPE_MULTI_3,
      CH_TYPE_MULTI_4,
      CH_TYPE_MULTI_5,
      CH_TYPE_MULTI_6,
      CH_TYPE_MULTI_7,
      CH_TYPE_MULTI_8,
      //
      CH_TYPE_ANY             = 0xFFFF
    };

    // Member variables (public) -----------------------------------------------
    PixelType       mPixelType;
    BufferType      mBufferType;
    DataType        mDataType;
    EndianType      mEndian;
    uint32          mFourCC;
    unsigned int    mComponentsPerPixel;

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
      mFourCC                 = 0;
      mComponentsPerPixel     = 0;
    }
    // -------------------------------------------------------------------------
    // ImageType
    // -------------------------------------------------------------------------
    ImageType(PixelType inPixelType, BufferType inBufferType, DataType inDataType,
                EndianType inEndian = ENDIAN_TYPE_HOST,
                uint32 inFourCC = 0,
                unsigned int inComponentsPerPixel = 0)
    {
      set(inPixelType, inBufferType, inDataType, inEndian,
          inFourCC, inComponentsPerPixel);
    }
    // -------------------------------------------------------------------------
    // ~ImageType
    // -------------------------------------------------------------------------
    virtual ~ImageType()
    {
    }
    // Member functions --------------------------------------------------------
    // cost functions ----------------------------------------------------------
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
      if (mComponentsPerPixel == 0)
        return false;
      return true;
    }
    // -------------------------------------------------------------------------
    // hasMacroPixelStructure
    // -------------------------------------------------------------------------
    bool  hasMacroPixelStructure() const
    {
      return hasMacroPixelStructure(mPixelType, mFourCC);
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
    // isSigned
    // -------------------------------------------------------------------------
    bool  isSigned() const
    {
      return isSigned(mDataType);
    }
    // -------------------------------------------------------------------------
    // isByteAlgned
    // -------------------------------------------------------------------------
    bool  isByteAlgned() const
    {
      return isByteAlgned(mDataType);
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
    // non-cost functions ------------------------------------------------------
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    void  set(PixelType inPixelType, BufferType inBufferType, DataType inDataType,
                EndianType inEndian = ENDIAN_TYPE_HOST,
                uint32 inFourCC = 0,
                unsigned int inComponentsPerPixel = 0)
    {
      setPixelType(inPixelType, inComponentsPerPixel);
      mBufferType             = inBufferType;
      mDataType               = inDataType;
      mFourCC                 = inFourCC;
      setEndianType(inEndian);
    }
    // -------------------------------------------------------------------------
    // setPixelType
    // -------------------------------------------------------------------------
    void  setPixelType(PixelType inPixelType, unsigned int inComponentsPerPixel = 0)
    {
      mPixelType  = inPixelType;
      if (inComponentsPerPixel == 0)
        mComponentsPerPixel   = coponentsPerPixel(inPixelType);
      else
        mComponentsPerPixel   = inComponentsPerPixel;
    }
    // -------------------------------------------------------------------------
    // setDataType
    // -------------------------------------------------------------------------
    void  setDataType(unsigned int inBitWidth, bool inIsSigned = false)
    {
      mDataType = dataTypeFromParams(inBitWidth, inIsSigned);
    }
    // -------------------------------------------------------------------------
    // setEndianType
    // -------------------------------------------------------------------------
    void  setEndianType(EndianType inEndian = ENDIAN_TYPE_HOST)
    {
      if (inEndian == ENDIAN_TYPE_HOST)
        mEndian               = getHostEndian();
      else
        mEndian               = inEndian;
    }
    // -------------------------------------------------------------------------
    // dump (debug function)
    // -------------------------------------------------------------------------
    void  dump(const char *inLeadringStr = "")
    {
      printf("%smPixelType  : 0x%X\n", inLeadringStr, mPixelType);
      printf("%smBufferType : 0x%X\n", inLeadringStr, mBufferType);
      printf("%smDataType   : 0x%X\n", inLeadringStr, mDataType);
      printf("%smEndian     : 0x%X\n", inLeadringStr, mEndian);
      printf("%smFourCC     : 0x%04X\n", inLeadringStr, mFourCC);
      printf("%smComponentsPerPixel : %d\n", inLeadringStr, mComponentsPerPixel);
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // coponentsPerPixel
    // -------------------------------------------------------------------------
    static unsigned int  coponentsPerPixel(PixelType inType)
    {
      switch (inType)
      {
        case PIXEL_TYPE_RAW:
        case PIXEL_TYPE_MONO:
        case PIXEL_TYPE_BAYER_GBRG:
        case PIXEL_TYPE_BAYER_GRBG:
        case PIXEL_TYPE_BAYER_BGGR:
        case PIXEL_TYPE_BAYER_RGGB:
          return 1;
        case PIXEL_TYPE_RGB:
        case PIXEL_TYPE_BGR:
        case PIXEL_TYPE_CMY:
        case PIXEL_TYPE_HSL:
        case PIXEL_TYPE_HSV:
        case PIXEL_TYPE_HSI:
        case PIXEL_TYPE_LUV:
        case PIXEL_TYPE_LAB:
        case PIXEL_TYPE_YUV444:
          return 3;
        case PIXEL_TYPE_RGBA:
        case PIXEL_TYPE_BGRA:
        case PIXEL_TYPE_CMYK:
          return 4;
        default:
          break;
      }
      return 0;
    }
    // -------------------------------------------------------------------------
    // hasMacroPixelStructure
    // -------------------------------------------------------------------------
    static bool  hasMacroPixelStructure(PixelType inType, uint32 inFourCC = 0)
    {
      switch (inType)
      {
        case PIXEL_TYPE_YUV410:
        case PIXEL_TYPE_YUV411:
        case PIXEL_TYPE_YUV420:
        case PIXEL_TYPE_YUV422:
          return true;
        case PIXEL_TYPE_FOURCC:
          // TODO : We need to check inFourCC here
          if (inFourCC == 0)
            return false;
          break;
        default:
          break;
      }
      return false;
    }
    // -------------------------------------------------------------------------
    // isSigned
    // -------------------------------------------------------------------------
    static size_t  isSigned(DataType inType)
    {
      if (inType < 0x1000)
        return false;
      return true;
    }
    // -------------------------------------------------------------------------
    // isByteAlgned
    // -------------------------------------------------------------------------
    static bool  isByteAlgned(DataType inType)
    {
      switch (inType)
      {
        case DATA_TYPE_8BIT:
        case DATA_TYPE_8BIT_SIGNED:
        case DATA_TYPE_16BIT:
        case DATA_TYPE_16BIT_SIGNED:
        case DATA_TYPE_24BIT:
        case DATA_TYPE_24BIT_SIGNED:
        case DATA_TYPE_32BIT:
        case DATA_TYPE_32BIT_SIGNED:
        case DATA_TYPE_40BIT:
        case DATA_TYPE_40BIT_SIGNED:
        case DATA_TYPE_48BIT:
        case DATA_TYPE_48BIT_SIGNED:
        case DATA_TYPE_56BIT:
        case DATA_TYPE_56BIT_SIGNED:
        case DATA_TYPE_64BIT:
        case DATA_TYPE_64BIT_SIGNED:
        case DATA_TYPE_FLOAT:
        case DATA_TYPE_DOUBLE:
          return true;
        default:
          break;
      }
      return false;
    }
    // -------------------------------------------------------------------------
    // sizeOfData
    // -------------------------------------------------------------------------
    static size_t  sizeOfData(DataType inType)
    {
      switch (inType)
      {
        case DATA_TYPE_8BIT:
        case DATA_TYPE_8BIT_SIGNED:
          return 1;
        case DATA_TYPE_16BIT:
        case DATA_TYPE_16BIT_SIGNED:
          return 2;
        case DATA_TYPE_24BIT:
        case DATA_TYPE_24BIT_SIGNED:
          return 3;
        case DATA_TYPE_32BIT:
        case DATA_TYPE_32BIT_SIGNED:
          return 4;
        case DATA_TYPE_40BIT:
        case DATA_TYPE_40BIT_SIGNED:
          return 5;
        case DATA_TYPE_48BIT:
        case DATA_TYPE_48BIT_SIGNED:
          return 6;
        case DATA_TYPE_56BIT:
        case DATA_TYPE_56BIT_SIGNED:
          return 7;
        case DATA_TYPE_64BIT:
        case DATA_TYPE_64BIT_SIGNED:
          return 8;
        default:
          break;
      }
      return 0;
    }
    // -------------------------------------------------------------------------
    // dataTypeFromParams
    // -------------------------------------------------------------------------
    static DataType dataTypeFromParams(unsigned int inBitWidth, bool inIsSigned = false)
    {
      DataType  type;

      switch (inBitWidth)
      {
        case 1:
          return DATA_TYPE_1BIT;
        case 4:
          type = DATA_TYPE_4BIT;
          break;
        case 8:
          type = DATA_TYPE_8BIT;
          break;
        case 10:
          type = DATA_TYPE_10BIT;
          break;
        case 12:
          type = DATA_TYPE_12BIT;
          break;
        case 14:
          type = DATA_TYPE_14BIT;
          break;
        case 16:
          type = DATA_TYPE_16BIT;
          break;
        case 24:
          type = DATA_TYPE_24BIT;
          break;
        case 32:
          type = DATA_TYPE_32BIT;
          break;
        case 40:
          type = DATA_TYPE_40BIT;
          break;
        case 48:
          type = DATA_TYPE_48BIT;
          break;
        case 56:
          type = DATA_TYPE_56BIT;
          break;
        case 64:
          type = DATA_TYPE_64BIT;
          break;
        default:
          return DATA_TYPE_NOT_SPECIFIED;
      }
      if (inIsSigned)
        type = (DataType )((int )type + (int )DATA_TYPE_SIGNED_OFFSET);
      return type;
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
        {PIXEL_TYPE_RAW,        "RAW"},
        {PIXEL_TYPE_MONO,       "MONO"},
        {PIXEL_TYPE_BAYER_GBRG, "BAYER_GBRG"},
        {PIXEL_TYPE_BAYER_GRBG, "BAYER_GRBG"},
        {PIXEL_TYPE_BAYER_BGGR, "BAYER_BGGR"},
        {PIXEL_TYPE_BAYER_RGGB, "BAYER_RGGB"},
        {PIXEL_TYPE_RGB,        "RGB"},
        {PIXEL_TYPE_RGBA,       "RGBA"},
        {PIXEL_TYPE_ARGB,       "ARGB"},
        {PIXEL_TYPE_BGR,        "BGR"},
        {PIXEL_TYPE_BGRA,       "BGRA"},
        {PIXEL_TYPE_ABGR,       "ABGR"},
        {PIXEL_TYPE_CMY,        "CMY"},
        {PIXEL_TYPE_CMYK,       "CMYK"},
        {PIXEL_TYPE_HSL,        "HSL"},
        {PIXEL_TYPE_HSV,        "HSV"},
        {PIXEL_TYPE_HSI,        "HSI"},
        {PIXEL_TYPE_LUV,        "LUV"},
        {PIXEL_TYPE_LAB,        "LAB"},
        {PIXEL_TYPE_LCHAB,      "LCHAB"},
        {PIXEL_TYPE_LCHUV,      "LCHUV"},
        {PIXEL_TYPE_DIN99,      "DIN99"},
        {PIXEL_TYPE_DIN99D,     "DIN99d"},
        {PIXEL_TYPE_DIN99O,     "DIN99o"},
        {PIXEL_TYPE_YUV410,     "YUV410"},
        {PIXEL_TYPE_YUV411,     "YUV411"},
        {PIXEL_TYPE_YUV420,     "YUV420"},
        {PIXEL_TYPE_YUV422,     "YUV422"},
        {PIXEL_TYPE_FOURCC,     "FORCC"},
        {PIXEL_TYPE_MULTI_CH,   "MULTI_CH"},
        {PIXEL_TYPE_JPEG,       "JPEG"},
        {PIXEL_TYPE_ANY,        "ANY"},
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
        {DATA_TYPE_1BIT,    "1BIT"},
        {DATA_TYPE_4BIT,    "4BIT"},
        {DATA_TYPE_8BIT,    "8BIT"},
        {DATA_TYPE_10BIT,   "10BIT"},
        {DATA_TYPE_12BIT,   "12BIT"},
        {DATA_TYPE_14BIT,   "14BIT"},
        {DATA_TYPE_16BIT,   "16BIT"},
        {DATA_TYPE_24BIT,   "24BIT"},
        {DATA_TYPE_32BIT,   "32BIT"},
        {DATA_TYPE_40BIT,   "40BIT"},
        {DATA_TYPE_48BIT,   "48BIT"},
        {DATA_TYPE_56BIT,   "56BIT"},
        {DATA_TYPE_64BIT,   "64BIT"},
        {DATA_TYPE_FLOAT,   "FLOAT"},
        {DATA_TYPE_DOUBLE,  "DOUBLE"},
        //
        {DATA_TYPE_4BIT_SIGNED,   "4BIT_SIGNED"},
        {DATA_TYPE_8BIT_SIGNED,   "8BIT_SIGNED"},
        {DATA_TYPE_10BIT_SIGNED,  "10BIT_SIGNED"},
        {DATA_TYPE_12BIT_SIGNED,  "12BIT_SIGNED"},
        {DATA_TYPE_14BIT_SIGNED,  "14BIT_SIGNED"},
        {DATA_TYPE_16BIT_SIGNED,  "16BIT_SIGNED"},
        {DATA_TYPE_24BIT_SIGNED,  "24BIT_SIGNED"},
        {DATA_TYPE_32BIT_SIGNED,  "32BIT_SIGNED"},
        {DATA_TYPE_40BIT_SIGNED,  "40BIT_SIGNED"},
        {DATA_TYPE_48BIT_SIGNED,  "48BIT_SIGNED"},
        {DATA_TYPE_56BIT_SIGNED,  "56BIT_SIGNED"},
        {DATA_TYPE_64BIT_SIGNED,  "64BIT_SIGNED"},
        //
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
    unsigned int    mWidth;
    unsigned int    mHeight;
    bool            mIsBottomUp;
    size_t          mBufferSize;
    size_t          mHeaderOffset;
    size_t          mPixelStep;
    size_t          mLineStep;
    size_t          mChannelStep;
    size_t          mPixelAreaSize;

    // -------------------------------------------------------------------------
    // ImageFormat
    // -------------------------------------------------------------------------
    ImageFormat()
    {
      mWidth          = 0;
      mHeight         = 0;
      mIsBottomUp     = false;
      mBufferSize     = 0;
      mHeaderOffset   = 0;
      mPixelStep      = 0;
      mLineStep       = 0;
      mChannelStep    = 0;
      mPixelAreaSize  = 0;
    }
    // -------------------------------------------------------------------------
    // ImageFormat
    // -------------------------------------------------------------------------
    ImageFormat(const ImageType &inType,
                unsigned int inWidth, unsigned int inHeight,
                bool inIsBottomUp = false,
                size_t inBufferSize = 0,
                size_t inHeaderOffset = 0,
                size_t inPixelStep = 0,
                size_t inLineStep = 0,
                size_t inChannelStep = 0)
    {
      set(inType,
          inWidth, inHeight,
          inIsBottomUp,
          inBufferSize, inHeaderOffset,
          inPixelStep, inLineStep, inChannelStep);
    }
    // -------------------------------------------------------------------------
    // ~ImageFormat
    // -------------------------------------------------------------------------
    virtual ~ImageFormat()
    {
    }
    // Member functions --------------------------------------------------------
    // cost functions ----------------------------------------------------------
    // -------------------------------------------------------------------------
    // getPlaneOffset
    // -------------------------------------------------------------------------
    size_t getPlaneOffset(unsigned int inPlaneIndex = 0) const
    {
      return calculatePlaneOffset(*this, inPlaneIndex);
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
      return calculateLineOffset(*this, inY, inPlaneIndex);
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
    // non-cost functions ------------------------------------------------------
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    void  set(const ImageType &inType,
                unsigned int inWidth, unsigned int inHeight,
                bool inIsBottomUp = false,
                size_t inBufferSize = 0,
                size_t inHeaderOffset = 0,
                size_t inPixelStep = 0,
                size_t inLineStep = 0,
                size_t inChannelStep = 0)
    {
      mType = inType;
      set(inWidth, inHeight,
          inIsBottomUp,
          inBufferSize, inHeaderOffset,
          inPixelStep, inLineStep, inChannelStep);
    }
    // -------------------------------------------------------------------------
    // set
    // -------------------------------------------------------------------------
    void  set(unsigned int inWidth, unsigned int inHeight,
                bool inIsBottomUp = false,
                size_t inBufferSize = 0,
                size_t inHeaderOffset = 0,
                size_t inPixelStep = 0,
                size_t inLineStep = 0,
                size_t inChannelStep = 0)
    {
      mWidth          = inWidth;
      mHeight         = inHeight;
      mIsBottomUp     = inIsBottomUp;
      mHeaderOffset   = inHeaderOffset;
      if (inPixelStep != 0)
        mPixelStep = inPixelStep; // TODO: Add a sanity check here...
      else
      {
        if (mType.isPacked() || mType.hasMacroPixelStructure())
          mPixelStep = 0;
        else
        {
          mPixelStep = mType.sizeOfData();
          if (mType.isPlanar() == false)
            mPixelStep *= mType.mComponentsPerPixel;
        }
      }
      if (inLineStep != 0)
        mLineStep = inLineStep; // TODO: Add a sanity check here...
      else
        mLineStep = mPixelStep * mWidth;
      if (inChannelStep != 0)
        mChannelStep = inChannelStep;  // TODO: Add a sanity check here...
      else
        mChannelStep = mLineStep * mHeight;  // TODO: this assumption doesn't cover everything...
      //
      if (mType.isPlanar())
        mPixelAreaSize = mChannelStep * mType.mComponentsPerPixel;
      else
        mPixelAreaSize = mChannelStep;
      //
      if (inBufferSize != 0)
        mBufferSize = inBufferSize;
      else
        mBufferSize = mHeaderOffset + mPixelAreaSize;
    }
    // -------------------------------------------------------------------------
    // dump (debug function)
    // -------------------------------------------------------------------------
    void  dump(const char *inLeadringStr = "")
    {
      char  buf[256];
      if (strlen(inLeadringStr) < (sizeof(buf) - 10))
        sprintf(buf, "%s  ", inLeadringStr);
      else
        sprintf(buf, "  ");
      //
      printf("%smType:\n", inLeadringStr);
      mType.dump(buf);
      printf("%smWidth          : %d\n", inLeadringStr, mWidth);
      printf("%smHeight         : %d\n", inLeadringStr, mHeight);
      printf("%smIsBottomUp     : %d\n", inLeadringStr, mIsBottomUp);
      printf("%smBufferSize     : %zu\n", inLeadringStr, mBufferSize);
      printf("%smHeaderOffset   : %zu\n", inLeadringStr, mHeaderOffset);
      printf("%smPixelStep      : %zu\n", inLeadringStr, mPixelStep);
      printf("%smLineStep       : %zu\n", inLeadringStr, mLineStep);
      printf("%smChannelStep    : %zu\n", inLeadringStr, mChannelStep);
      printf("%smPixelAreaSize  : %zu\n", inLeadringStr, mPixelAreaSize);
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
    static size_t calculatePlaneOffset(
                            const ImageFormat &inFormat,
                            unsigned int inPlaneIndex = 0)
    {
      if (inFormat.mType.isPlanar() == false)
        return inFormat.mHeaderOffset;
      if (inPlaneIndex >= inFormat.mType.mComponentsPerPixel)
        inPlaneIndex = inFormat.mType.mComponentsPerPixel - 1;
      // TODO : we need to think about a format that has MacroPixel Structure with a planner buffer
      return inFormat.mChannelStep * inPlaneIndex + inFormat.mHeaderOffset;
    }
    // -------------------------------------------------------------------------
    // calculateLineOffsetFromPlaneOffset
    // -------------------------------------------------------------------------
    static size_t calculateLineOffsetFromPlaneOffset(
                              const ImageFormat &inFormat,
                              size_t inPlaneOffset,
                              unsigned int inY)
    {
      if (inY >= inFormat.mHeight)
        inY = inFormat.mHeight - 1;
      if (inY == 0)
        return inPlaneOffset;
      return inPlaneOffset + inFormat.mLineStep * inY;
    }
    // -------------------------------------------------------------------------
    // calculateLineOffset
    // -------------------------------------------------------------------------
    static size_t calculateLineOffset(
                              const ImageFormat &inFormat,
                              unsigned int inY,
                              unsigned int inPlaneIndex)
    {
      return calculateLineOffsetFromPlaneOffset(
                                  inFormat,
                                  calculatePlaneOffset(inFormat, inPlaneIndex),
                                  inY);
    }
    // -------------------------------------------------------------------------
    // calculatePixelOffsetFromLineOffset
    // -------------------------------------------------------------------------
    static size_t calculatePixelOffsetFromLineOffset(
                              const ImageFormat &inFormat,
                              size_t inLineOffset,
                              unsigned int inX)
    {
      if (inX >= inFormat.mWidth)
        inX = inFormat.mWidth - 1;
      return inLineOffset + inFormat.mPixelStep * inX;
    }
    // -------------------------------------------------------------------------
    // calculatePixelOffsetFromPlaneOffset
    // -------------------------------------------------------------------------
    static size_t calculatePixelOffsetFromPlaneOffset(
                              const ImageFormat &inFormat,
                              size_t inPlaneOffset,
                              unsigned int inX,
                              unsigned int inY)
    {
      return calculatePixelOffsetFromLineOffset(
                                  inFormat,
                                  calculateLineOffsetFromPlaneOffset(inFormat, inPlaneOffset, inY),
                                  inX);
    }
    // -------------------------------------------------------------------------
    // calculatePixelOffset
    // -------------------------------------------------------------------------
    static size_t calculatePixelOffset(
                              const ImageFormat &inFormat,
                              unsigned int inX,
                              unsigned int inY,
                              unsigned int inPlaneIndex = 0)
    {
      return calculatePixelOffsetFromLineOffset(
                                  inFormat,
                                  calculateLineOffset(inFormat, inY, inPlaneIndex),
                                  inX);
    }
  };
 };
};

#endif  // #ifdef IBC_IMAGE_IMAGE_H_

