// =============================================================================
//  image_converter_interface.h
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
