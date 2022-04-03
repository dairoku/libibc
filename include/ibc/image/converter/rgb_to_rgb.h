// =============================================================================
//  rgb_to_rgb.h
//
//  Written in 2019 by Dairoku Sekiguchi (sekiguchi at acm dot org)
//
//  To the extent possible under law, the author(s) have dedicated all copyright
//  and related and neighboring rights to this software to the public domain worldwide.
//  This software is distributed without any warranty.
//
//  You should have received a copy of the CC0 Public Domain Dedication along with
//  this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// =============================================================================
/*!
  \file     ibc/image/converter/rgb_to_rgb.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/19
  \brief    Header file for converting the rgb to rgb (bit depth difference)
*/

#ifndef IBC_IMAGE_CONVERTER_RGB_TO_RGB_H_
#define IBC_IMAGE_CONVERTER_RGB_TO_RGB_H_

// Includes --------------------------------------------------------------------
#include <cstring>
#include "ibc/image/image_converter_interface.h"

// Namespace -------------------------------------------------------------------
//namespace ibc::image::converter // <- nested namespace (C++17)
namespace ibc { namespace image { namespace converter
{
  // ---------------------------------------------------------------------------
  // RGB_to_RGB class
  // ---------------------------------------------------------------------------
  class  RGB_to_RGB : public virtual ImageConverterInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // RGB_to_RGB
    // -------------------------------------------------------------------------
    RGB_to_RGB()
    {
      mConvertFunc = NULL;
      mGain = 1.0;
      mOffset = 0.0;
      mGamma = 1.0;
      mIsParameterModified = false;
    }
    // -------------------------------------------------------------------------
    // ~RGB_to_RGB
    // -------------------------------------------------------------------------
    virtual ~RGB_to_RGB()
    {
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // isSupported
    // -------------------------------------------------------------------------
    virtual bool    isSupported(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat) const
    {
      if (findConvertFunction(inSrcFormat, inDstFormat) == NULL)
        return false;
      return true;
    }
    // -------------------------------------------------------------------------
    // init
    // -------------------------------------------------------------------------
    virtual void    init(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat)
    {
      mPixelAreaSize = inSrcFormat->mPixelAreaSize;
      mConvertFunc = findConvertFunction(inSrcFormat, inDstFormat);
    }
    // -------------------------------------------------------------------------
    // convert
    // -------------------------------------------------------------------------
    virtual void    convert(const void *inImage, void *outImage)
    {
      if (mConvertFunc == NULL)
        return;

      mConvertFunc(this, inImage, outImage);
    }
    // -------------------------------------------------------------------------
    // dispose
    // -------------------------------------------------------------------------
    virtual void    dispose()
    {
      // Do nothing at this moment...
    }
    // -------------------------------------------------------------------------
    // isColorMapSupported
    // -------------------------------------------------------------------------
    virtual bool  isColorMapSupported()
    {
      return false;
    }
    // -------------------------------------------------------------------------
    // setColorMapIndex
    // -------------------------------------------------------------------------
    virtual void  setColorMapIndex(ColorMap::ColorMapIndex inIndex, int inMultiNum = 1)
    {
      // Do nothing (This class does not have the color map function)
      UNUSED(inIndex);
      UNUSED(inMultiNum);
    }
    // -------------------------------------------------------------------------
    // getColorMapIndex
    // -------------------------------------------------------------------------
    virtual ColorMap::ColorMapIndex getColorMapIndex() const
    {
      // Do nothing (This class does not have the color map function)
      return ColorMap::CMIndex_NOT_SPECIFIED;
    }
    // -------------------------------------------------------------------------
    // getColorMapMultiNum
    // -------------------------------------------------------------------------
    virtual int getColorMapMultiNum() const
    {
      return 1;
    }
    // -------------------------------------------------------------------------
    // setGain
    // -------------------------------------------------------------------------
    virtual void  setGain(double inGain)
    {
      mGain = inGain;
      mIsParameterModified = true;
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
    virtual void  setChGains(const std::vector<double> &inGains)
    {
      mGain = inGains[0];
      mIsParameterModified = true;
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
      mIsParameterModified = true;
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
    virtual void  setChOffsets(const std::vector<double> &inOffsets)
    {
      mOffset = inOffsets[0];
      mIsParameterModified = true;
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
      mIsParameterModified = true;
    }
    // -------------------------------------------------------------------------
    // getGamma
    // -------------------------------------------------------------------------
    virtual double  getGamma() const
    {
      return mGamma;
    }

  protected:
    size_t  mPixelAreaSize;
    double  mGain, mOffset, mGamma;
    bool  mIsParameterModified;
    void  (*mConvertFunc)(RGB_to_RGB *, const void *, void *);

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // findConvertFunction
    // -------------------------------------------------------------------------
    static void  (*findConvertFunction(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat))(RGB_to_RGB *, const void *, void *)
    {
      UNUSED(inDstFormat);
      //
      if (inSrcFormat->mType.checkType( ibc::image::ImageType::PIXEL_TYPE_RGB,
                                        ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                        ibc::image::ImageType::DATA_TYPE_8BIT))
        return convertRGB8;
      return NULL;
    }
    // -------------------------------------------------------------------------
    // convertRGB8
    // -------------------------------------------------------------------------
    static void  convertRGB8(RGB_to_RGB *inObj, const void *inImage, void *outImage)
    {
      std::memcpy(outImage, inImage, inObj->mPixelAreaSize);
    }
  };
};};};

#endif  // #ifdef IBC_IMAGE_CONVERTER_RGB_TO_RGB_H_
