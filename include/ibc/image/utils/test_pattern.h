// =============================================================================
//  test_pattern.h
//
//  Written in 2020 by Dairoku Sekiguchi (sekiguchi at acm dot org)
//
//  To the extent possible under law, the author(s) have dedicated all copyright
//  and related and neighboring rights to this software to the public domain worldwide.
//  This software is distributed without any warranty.
//
//  You should have received a copy of the CC0 Public Domain Dedication along with
//  this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// =============================================================================
/*!
  \file     ibc/image/utils/test_pattern.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2020/12/23
  \brief    Header file for handling the color map
*/

#ifndef IBC_IMAGE_UTILS_TEST_PATTERN_H_
#define IBC_IMAGE_UTILS_TEST_PATTERN_H_

// Includes ------------------------------------------------------ --------------
#include <cstring>
#include "ibc/image/image.h"
#include "ibc/image/color_map.h"
#include "ibc/image/display_buffer.h"

// Namespace ------------------------------------------------------------------
//namespace ibc::image::utils // <- nested namespace (C++17)
namespace ibc { namespace image { namespace utils
{
  // ---------------------------------------------------------------------------
  // TestPattern class
  // ---------------------------------------------------------------------------
  class  TestPattern
  {
  public:
    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // allocateBufferAndFill
    // -------------------------------------------------------------------------
    static void allocateBufferAndFill(
      ibc::image::DisplayBuffer *inBuffer,
      int inPattern,
      ibc::image::ImageType::PixelType inPixelType,
      ibc::image::ImageType::DataType  inDataType,
      int inWidth, int inHeight,
      ibc::image::ColorMap::ColorMapIndex inColorMapIndex = ibc::image::ColorMap::CMIndex_NOT_SPECIFIED,
      int inColorMapMultiNum = 1,
      double inGain = 1.0, double inOffsset = 0)
    {
      // Create ImageData here
      ibc::image::ImageType   imageType(inPixelType,
                                        ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                        inDataType);
      ibc::image::ImageFormat imageFormat(imageType, inWidth, inHeight);

      inBuffer->allocateImageBuffer(imageFormat);
      inBuffer->mActiveConverter->setColorMapIndex(inColorMapIndex, inColorMapMultiNum);
      inBuffer->mActiveConverter->setGain(inGain);
      inBuffer->mActiveConverter->setOffset(inOffsset);

      fillTestPattern(inBuffer, inPattern);
    }
    // -------------------------------------------------------------------------
    // fillTestPattern
    // -------------------------------------------------------------------------
    static void fillTestPattern(ibc::image::ImageBuffer *inBuffer, int inPattern)
    {
      if (inBuffer->getImageType().mPixelType == ibc::image::ImageType::PIXEL_TYPE_MONO &&
          inBuffer->getImageType().mDataType  == ibc::image::ImageType::DATA_TYPE_8BIT)
      {
        unsigned char *bufPtr = (unsigned char *)inBuffer->getImageBufferPtr();
        for (int y = 0; y < inBuffer->getHeight(); y++)
          for (int x = 0; x < inBuffer->getWidth(); x++)
          {
            switch (inPattern)
            {
              case 0:
                *bufPtr = (unsigned char)(x ^ y);
                break;
              case 1:
                *bufPtr = (unsigned char)(x & 0xFF);
                break;
              case 2:
                *bufPtr = (unsigned char)(y & 0xFF);
                break;
              default:
                *bufPtr = (unsigned char)((x+y) & 0xFF);
                break;
            }
            bufPtr++;
          }
        return;
      }
      if (inBuffer->getImageType().mPixelType == ibc::image::ImageType::PIXEL_TYPE_MONO &&
          inBuffer->getImageType().mDataType  == ibc::image::ImageType::DATA_TYPE_16BIT)
      {
        unsigned short *bufPtr = (unsigned short *)inBuffer->getImageBufferPtr();
        for (int y = 0; y < inBuffer->getHeight(); y++)
          for (int x = 0; x < inBuffer->getWidth(); x++)
          {
            switch (inPattern)
            {
              case 0:
                *bufPtr = (unsigned short)(x ^ y);
                break;
              case 1:
                *bufPtr = (unsigned short)(x & 0xFFFF);
                break;
              case 2:
                *bufPtr = (unsigned short)(y & 0xFFFF);
                break;
              default:
                *bufPtr = (unsigned short)((x+y) & 0xFFFF);
                break;
            }
            bufPtr++;
          }
        return;
      }
    }
  };
};};};

#endif  // #ifdef IBC_IMAGE_UTILS_TEST_PATTERN_H_
