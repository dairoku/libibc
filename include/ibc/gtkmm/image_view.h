// =============================================================================
//  image_view.h
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
  \file     ibc/gtkmm/image_view.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/06
  \brief    Header file for ImageView widget
*/

#ifndef IBC_GTKMM_IMAGE_VIEW_H_
#define IBC_GTKMM_IMAGE_VIEW_H_

// Includes --------------------------------------------------------------------
#include <math.h>
#include <cstring>
#include <gtkmm.h>
#include "ibc/gtkmm/image_view_base.h"
#include "ibc/gtkmm/image_data.h"
#include "ibc/gtkmm/view_data_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gtkmm
 {
  // ---------------------------------------------------------------------------
  // ImageView class
  // ---------------------------------------------------------------------------
  class ImageView : virtual public ImageViewBase, virtual public ViewDataInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ImageView
    // -------------------------------------------------------------------------
    ImageView() :
      Glib::ObjectBase("ImageView")
    {
      mImageDataPtr = NULL;
      mIsImageSizeChanged = false;
    }
    // -------------------------------------------------------------------------
    // ~ImageView
    // -------------------------------------------------------------------------
    virtual ~ImageView()
    {
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // setImageDataPtr
    // -------------------------------------------------------------------------
    void  setImageDataPtr(ibc::gtkmm::ImageData *inImageDataPtr)
    {
      mImageDataPtr = inImageDataPtr;
      mImageDataPtr->addWidget(this);
      markAsImageSizeChanged();
    }
    // -------------------------------------------------------------------------
    // queueRedrawWidget
    // -------------------------------------------------------------------------
    virtual void  queueRedrawWidget()
    {
      queue_draw();
    }
    // -------------------------------------------------------------------------
    // markAsImageSizeChanged
    // -------------------------------------------------------------------------
    virtual void  markAsImageSizeChanged()
    {
      mIsImageSizeChanged = true;
      queue_draw();
    }
    // -------------------------------------------------------------------------
    // isImageSizeChanged
    // -------------------------------------------------------------------------
    bool  isImageSizeChanged() const
    {
      return mIsImageSizeChanged;
    }

  protected:
    // Member variables --------------------------------------------------------
    ImageData *mImageDataPtr;
    bool      mIsImageSizeChanged;

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // updateSizeUsingImageData
    // -------------------------------------------------------------------------
    bool  updateSizeUsingImageData()
    {
      if (mImageDataPtr == NULL)
        return false;
      if (mImageDataPtr->checkImageBufferPtr() == false)  // dare to use this instead of checkImageData()
        return false;
     
      mOrgWidth   = mImageDataPtr->getWidth();
      mOrgHeight  = mImageDataPtr->getHeight();
      mWidth       = mOrgWidth;
      mHeight      = mOrgHeight;
    
      configureHAdjustment();
      configureVAdjustment();
      return true;
    }
    // -------------------------------------------------------------------------
    // on_draw
    // -------------------------------------------------------------------------
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
    {
      if (mImageDataPtr == NULL)
        return false;
      if (mImageDataPtr->checkImageData() == false)
        return false;
      if (mIsImageSizeChanged)
      {
        updateSizeUsingImageData();
        mIsImageSizeChanged = false;
      }
    
      double x = 0, y = 0;
    
      mImageDataPtr->updatePixbuf();
    
      if (mWidth <= mWindowWidth)
        x = (mWindowWidth  - mWidth)  / 2;
      else
        x = -1 * mOffsetX;
      if (mHeight <= mWindowHeight)
        y = (mWindowHeight - mHeight) / 2;
      else
        y = -1 * mOffsetY;
    
      if (mZoom >= 1)
      {
        cr->set_identity_matrix();
        cr->translate(x, y);
        cr->scale(mZoom, mZoom);
        Gdk::Cairo::set_source_pixbuf(cr, mImageDataPtr->mPixbuf, 0, 0);
        Cairo::SurfacePattern pattern(cr->get_source()->cobj());
        pattern.set_filter(Cairo::Filter::FILTER_NEAREST);
      }
      else
      {
        Gdk::Cairo::set_source_pixbuf(cr, mImageDataPtr->mPixbuf->scale_simple(mWidth, mHeight, Gdk::INTERP_NEAREST), x, y);
      }
    
      cr->paint();
    
      return true;
    }
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_VIEW_H_
