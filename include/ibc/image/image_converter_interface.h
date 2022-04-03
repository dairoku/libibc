// =============================================================================
//  image_converter_interface.h
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
  \file     ibc/image/image_converter_interface.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/19
  \brief    Header file for defining the image converter interface
*/

#ifndef IBC_IMAGE_IMAGE_CONVERTER_INTERFACE_H_
#define IBC_IMAGE_IMAGE_CONVERTER_INTERFACE_H_

// Includes --------------------------------------------------------------------
#include "ibc/image/image.h"
#include "ibc/image/color_map.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace image
 {
  // ---------------------------------------------------------------------------
  // ImageConverterInterface interface class
  // ---------------------------------------------------------------------------
  class  ImageConverterInterface
  {
  public:
    //  member functions
    virtual bool    isSupported(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat) const  = 0;
    virtual void    init(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat)   = 0;
    virtual void    convert(const void *inImage, void *outImage) = 0;
    virtual void    dispose() = 0;
    virtual bool  isColorMapSupported() = 0;
    virtual void  setColorMapIndex(ColorMap::ColorMapIndex inIndex, int inMultiNum = 1) = 0;
    virtual ColorMap::ColorMapIndex getColorMapIndex() const = 0;
    virtual int getColorMapMultiNum() const = 0;
    virtual void  setGain(double inGain) = 0;
    virtual double  getGain() const = 0;
    virtual void  setChGains(const std::vector<double> &inGain) = 0;
    virtual std::vector<double> getChGaings() const = 0;
    virtual void  setOffset(double inOffset) = 0;
    virtual double  getOffset() const = 0;
    virtual void  setChOffsets(const std::vector<double> &inGain) = 0;
    virtual std::vector<double> getChOffsets() const = 0;
    virtual void  setGamma(double inGamma) = 0;
    virtual double  getGamma() const = 0;

    // ToDo
    // isIndex
    // setColorMap (IndexOnly)
    // setGain (white, r,g,b)
    // setToneCurve (ToneCurve -> ColorMap)
    // setOffset (? faster than tonecurve for pure offset)
    // setGamma?
    // setLUT?
    // set3DLUT?
  };
 };
};

#endif  // #ifdef IBC_IMAGE_IMAGE_CONVERTER_INTERFACE_H_
