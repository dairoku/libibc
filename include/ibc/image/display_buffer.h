// =============================================================================
//  display_buffer.h
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
  \file     ibc/image/display_buffer.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/20
  \brief    Format file for handling the display buffer
*/

#ifndef IBC_IMAGE_DISPLAY_BUFFER_H_
#define IBC_IMAGE_DISPLAY_BUFFER_H_

// Includes --------------------------------------------------------------------
#include <cstring>
#include <vector>
#include "ibc/image/image_buffer.h"
#include "ibc/image/image_converter_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace image
 {
  // ---------------------------------------------------------------------------
  // DisplayBuffer class
  // ---------------------------------------------------------------------------
  class  DisplayBuffer : public ImageBuffer
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // DisplayBuffer
    // -------------------------------------------------------------------------
    DisplayBuffer()
    {
      mActiveConverter = NULL;
    }
    // -------------------------------------------------------------------------
    // ~ImageBuffer
    // -------------------------------------------------------------------------
    virtual ~DisplayBuffer()
    {
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // addImageConverter
    // -------------------------------------------------------------------------
    void  addImageConverter(ImageConverterInterface *inConverter)
    {
      mConverterList.push_back(inConverter);
    }
    // -------------------------------------------------------------------------
    // removeImageConverter
    // -------------------------------------------------------------------------
    void  removeImageConverter(ImageConverterInterface *inConverter)
    {
      auto it = std::find(mConverterList.begin(), mConverterList.end(), inConverter);
      if (it == mConverterList.end())
        return;
      mConverterList.erase(it);
    }

    // Member variables --------------------------------------------------------
    ImageConverterInterface *mActiveConverter;

  protected:
    // Member variables --------------------------------------------------------
    std::vector<ImageConverterInterface *>  mConverterList;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // selectConverter
    // -------------------------------------------------------------------------
    bool  selectConverter(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat)
    {
      if (mActiveConverter != NULL)
        mActiveConverter->dispose();
      mActiveConverter = findSupportedConverter(inSrcFormat, inDstFormat);
      if (mActiveConverter == NULL)
        return false;
      mActiveConverter->init(inSrcFormat, inDstFormat);
      return true;
    }

  private:
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // findConverter
    // -------------------------------------------------------------------------
    ImageConverterInterface *findSupportedConverter(const ImageFormat *inSrcFormat, const ImageFormat *inDstFormat)
    {
      for (auto it = mConverterList.begin(); it != mConverterList.end(); it++)
        if ((*it)->isSupported(inSrcFormat, inDstFormat))
          return (*it);
      return NULL;
    }
  };
 };
};

#endif  // #ifdef IBC_IMAGE_DISPLAY_BUFFER_H_

