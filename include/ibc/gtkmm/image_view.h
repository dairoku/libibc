// =============================================================================
//  image_view.h
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
  \file     ibc/gtkmm/image_view.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/06
  \brief    Header file for ImageView widget

  This file defines the class for the image widget
*/

#ifndef IBC_GTKMM_IMAGE_VIEW_H_
#define IBC_GTKMM_IMAGE_VIEW_H_

// Includes --------------------------------------------------------------------
#include <gtkmm.h>
#include "ibc/gtkmm/image_view_base.h"
#include "ibc/gtkmm/image_data.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gtkmm
 {
  // ---------------------------------------------------------------------------
  // ImageView class
  // ---------------------------------------------------------------------------
  class ImageView : virtual public ImageViewBase
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    ImageView();
    virtual ~ImageView();

  protected:
    void  on_widget_created();
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

    ImageData mImageData;
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_VIEW_H_
