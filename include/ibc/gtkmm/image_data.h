// =============================================================================
//  image_data.h
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
  \file     ibc/gtkmm/image_data.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/14
  \brief    Header file for the ImageData class (MVC : model)
*/

#ifndef IBC_GTKMM_IMAGE_DATA_H_
#define IBC_GTKMM_IMAGE_DATA_H_

// Includes --------------------------------------------------------------------
#include <cstring>
#include <vector>
#include <gtkmm.h>
#include "ibc/image/display_buffer.h"
#include "ibc/gtkmm/view_data_interface.h"
//
#include "ibc/image/converter/rgb_to_rgb.h"
#include "ibc/image/converter/mono_to_rgb.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gtkmm
 {
  // ---------------------------------------------------------------------------
  // ImageData class
  // ---------------------------------------------------------------------------
   class  ImageData : public ibc::image::DisplayBuffer
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ImageData
    // -------------------------------------------------------------------------
    ImageData()
    {
      addImageConverter(&mRGB_to_RGB);
      addImageConverter(&mMono_to_RGB);
    }
    // -------------------------------------------------------------------------
    // ~ImageData
    // -------------------------------------------------------------------------
    virtual ~ImageData()
    {
    }

    // Member functions --------------------------------------------------------
     // -------------------------------------------------------------------------
    // checkImageBufferPtr
    // -------------------------------------------------------------------------
    bool  checkImageData() const
    {
      if (checkImageBufferPtr() == false || !mPixbuf || mImageFormatPtr == NULL)
        return false;
      return true;
    }
   // -------------------------------------------------------------------------
    // updatePixbuf
    // -------------------------------------------------------------------------
    virtual bool  updatePixbuf(bool inForceUpdate = false)
    {
      if (inForceUpdate == false && isImageModified() == false)
        return false;

      if (checkImageData() == false)
        return false;

      // Sanity check here
      if (mPixbuf->get_width() != mImageFormatPtr->mWidth ||
          mPixbuf->get_height() != mImageFormatPtr->mHeight ||
          mActiveConverter == NULL)
        return false; // Should throw exception?

      mActiveConverter->convert(getImageBufferPixelPtr(), mPixbuf->get_pixels());
      clearIsImageModifiedFlag();
      return true;
    }
    // -------------------------------------------------------------------------
    // addWidget
    // -------------------------------------------------------------------------
    void  addWidget(ViewDataInterface *inWidget)
    {
      mWidgetList.push_back(inWidget);
    }
     // -------------------------------------------------------------------------
    // removeWidget
    // -------------------------------------------------------------------------
    void  removeWidget(ViewDataInterface *inWidget)
    {
      auto it = std::find(mWidgetList.begin(), mWidgetList.end(), inWidget);
      if (it == mWidgetList.end())
        return;
      mWidgetList.erase(it);
    }
     // -------------------------------------------------------------------------
    // queueRedrawAllWidgets
    // -------------------------------------------------------------------------
    void  queueRedrawAllWidgets()
    {
      for (auto it = mWidgetList.begin(); it != mWidgetList.end(); it++)
        (*it)->queueRedrawWidget();
    }
     // -------------------------------------------------------------------------
    // markAllWidgetsAsImageSizeChanged
    // -------------------------------------------------------------------------
    void  markAllWidgetsAsImageSizeChanged()
    {
      for (auto it = mWidgetList.begin(); it != mWidgetList.end(); it++)
        (*it)->markAsImageSizeChanged();
    }

    // Member variables --------------------------------------------------------
    Glib::RefPtr<Gdk::Pixbuf>  mPixbuf;

  protected:
    // Member variables --------------------------------------------------------
    std::vector<ViewDataInterface *>  mWidgetList;
    //
    ibc::image::converter::RGB_to_RGB   mRGB_to_RGB;
    ibc::image::converter::Mono_to_RGB   mMono_to_RGB;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // parameterModified
    // -------------------------------------------------------------------------
    virtual void  parameterModified()
    {
      mPixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB , false, 8, // does not have alpha and 8bit
                                    mImageFormatPtr->mWidth, mImageFormatPtr->mHeight);

      ibc::image::ImageType   imageType(ibc::image::ImageType::PIXEL_TYPE_RGB,
                                        ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                        ibc::image::ImageType::DATA_TYPE_8BIT);
      ibc::image::ImageFormat imageFormat(imageType, mImageFormatPtr->mWidth, mImageFormatPtr->mHeight);
      selectConverter(mImageFormatPtr, &imageFormat);

      markAsImageModified();
      markAllWidgetsAsImageSizeChanged();
    }
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_DATA_H_
