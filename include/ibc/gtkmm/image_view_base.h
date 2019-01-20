// =============================================================================
//  image_view_base.h
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
  \file     ibc/gtkmm/image_view_base.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/06
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
*/

#ifndef IBC_GTKMM_IMAGE_VIEW_BASE_H_
#define IBC_GTKMM_IMAGE_VIEW_BASE_H_

// Includes --------------------------------------------------------------------
#include <gtkmm.h>

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gtkmm
 {
  // ---------------------------------------------------------------------------
  // ImageView class
  // ---------------------------------------------------------------------------
  class ImageViewBase : virtual public Gtk::Scrollable, virtual public Gtk::Widget  // Note: Order of Scrollable -> Widget is very important
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    ImageViewBase();
    virtual ~ImageViewBase();

  protected:
    virtual void  on_realize();
    virtual void  on_unrealize();

    virtual bool  on_button_press_event(GdkEventButton* button_event);
    virtual bool  on_motion_notify_event(GdkEventMotion* motion_event);
    virtual bool  on_button_release_event(GdkEventButton* release_event);
    virtual bool  on_scroll_event(GdkEventScroll *event);
    virtual void  on_size_allocate(Gtk::Allocation& allocation);

    void hAdjustmentChanged();
    void vAdjustmentChanged();

    virtual void configureHAdjustment();
    virtual void configureVAdjustment();
    virtual void adjustmentValueChanged();

    Glib::Property<Glib::RefPtr<Gtk::Adjustment>> mHAdjustment;
    Glib::Property<Glib::RefPtr<Gtk::Adjustment>> mVAdjustment;
    Glib::Property<Gtk::ScrollablePolicy> mHScrollPolicy;
    Glib::Property<Gtk::ScrollablePolicy> mVScrollPolicy;
    sigc::connection mHAdjustmentConnection;
    sigc::connection mVAdjustmentConnection;

    double mWindowX, mWindowY, mWindowWidth, mWindowHeight;
    double mWidth, mHeight;
    double mOrgWidth, mOrgHeight;
    double m_mouse_x, m_mouse_y;
    double mOffsetX, mOffsetY;
    double mOffsetXMax, mOffsetYMax;
    double mOffsetXOrg, mOffsetYOrg;
    double mZoom;
    bool  mMouseLPressed;
    bool  mAdjusmentsModified;

    Glib::RefPtr<Gdk::Window> mWindow;
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_VIEW_BASE_H_

