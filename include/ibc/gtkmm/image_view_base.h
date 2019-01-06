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

    void hadjustment_changed();
    void vadjustment_changed();

    virtual void configure_hadjustment();
    virtual void configure_vadjustment();
    virtual void adjustment_value_changed();

    Glib::Property<Glib::RefPtr<Gtk::Adjustment>> hadjustment_;
    Glib::Property<Glib::RefPtr<Gtk::Adjustment>> vadjustment_;
    Glib::Property<Gtk::ScrollablePolicy> hscroll_policy_;
    Glib::Property<Gtk::ScrollablePolicy> vscroll_policy_;
    sigc::connection hadjustment_connection_;
    sigc::connection vadjustment_connection_;

    double m_window_x, m_window_y, m_window_width, m_window_height;
    double m_width, m_height;
    double m_org_width, m_org_height;
    double m_mouse_x, m_mouse_y;
    double m_offset_x, m_offset_y;
    double m_offset_x_max, m_offset_y_max;
    double m_offset_x_org, m_offset_y_org;
    double  m_zoom;
    bool  m_mouse_l_pressed;
    bool  m_adjusments_modified;

    Glib::RefPtr<Gdk::Window> m_window;


    Glib::RefPtr<Gdk::Pixbuf>  m_pixbuf;
    Glib::RefPtr<Pango::Layout> m_layout;
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_VIEW_BASE_H_

