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
  \brief    Header file for handling the image buffer

  This file defines the image buffer class for the IBC Library
*/

#ifndef IBC_IMAGE_IMAGE_CONVERTER_INTERFACE_H_
#define IBC_IMAGE_IMAGE_CONVERTER_INTERFACE_H_

// Includes --------------------------------------------------------------------
#include "ibc/image/image.h"

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
