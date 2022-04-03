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
  \file     ibc/qt/image_data.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/04/28
  \brief    Header file for the ImageData class (MVC : model)
*/

#ifndef IBC_QT_IMAGE_DATA_H_
#define IBC_QT_IMAGE_DATA_H_

// Includes --------------------------------------------------------------------
#include <cstring>
#include <vector>
#include <QImage>
#include "ibc/image/display_buffer.h"
#include "ibc/qt/view_data_interface.h"
//
#include "ibc/image/converter/rgb_to_rgb.h"
#include "ibc/image/converter/mono_to_rgb.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace qt
 {
  // ---------------------------------------------------------------------------
  // ImageData class
  // ---------------------------------------------------------------------------
   class  ImageData : public ibc::image::DisplayBuffer
  {
  public:
    // Member variables --------------------------------------------------------
    QImage  *mQImage;

    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ImageData
    // -------------------------------------------------------------------------
    ImageData()
    {
      mQImage = NULL;
      addImageConverter(&mRGB_to_RGB);
      addImageConverter(&mMono_to_RGB);
    }
    // -------------------------------------------------------------------------
    // ~ImageData
    // -------------------------------------------------------------------------
    virtual ~ImageData()
    {
      disposeQImage();
    }

    // Member functions --------------------------------------------------------
     // -------------------------------------------------------------------------
    // checkImageBufferPtr
    // -------------------------------------------------------------------------
    bool  checkImageData() const
    {
      if (checkImageBufferPtr() == false || mQImage == NULL || mImageFormatPtr == NULL)
        return false;
      return true;
    }
   // -------------------------------------------------------------------------
    // updateQImage
    // -------------------------------------------------------------------------
    virtual bool  updateQImage(bool inForceUpdate = false)
    {
      if (inForceUpdate == false && isImageModified() == false)
        return false;

      if (checkImageData() == false)
        return false;

      // Sanity check here
      if ((unsigned int )mQImage->width()  != mImageFormatPtr->mWidth ||
          (unsigned int )mQImage->height() != mImageFormatPtr->mHeight ||
          mActiveConverter == NULL)
        return false; // Should throw exception?

      mActiveConverter->convert(getImageBufferPixelPtr(), mQImage->bits());

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
      disposeQImage();
      mQImage = new QImage(mImageFormatPtr->mWidth, mImageFormatPtr->mHeight,
                           QImage::Format_RGB888);

      ibc::image::ImageType   imageType(ibc::image::ImageType::PIXEL_TYPE_RGB,
                                        ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                        ibc::image::ImageType::DATA_TYPE_8BIT);
      ibc::image::ImageFormat imageFormat(imageType, mImageFormatPtr->mWidth, mImageFormatPtr->mHeight);
      selectConverter(mImageFormatPtr, &imageFormat);

      markAsImageModified();
      markAllWidgetsAsImageSizeChanged();
    }
    // -------------------------------------------------------------------------
    // disposeQImage
    // -------------------------------------------------------------------------
    virtual void  disposeQImage()
    {
      if (mQImage == NULL)
        return;
      delete mQImage;
      mQImage = NULL;
    }
  };
 };
};

#endif  // #ifdef IBC_QT_IMAGE_DATA_H_
