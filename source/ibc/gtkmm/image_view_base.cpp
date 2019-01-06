// =============================================================================
//  image_view_base.cpp
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
  \file     ibc/image/image_view_base.cpp
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/06
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
*/

// Includes --------------------------------------------------------------------
#include <math.h>
#include <cstring>
#include "ibc/gtkmm/image_view_base.h"

// =============================================================================
// ImageViewBase class
// =============================================================================
ibc::gtkmm::ImageViewBase::ImageViewBase()
    : Glib::ObjectBase("my_draw_area"),
      hadjustment_(*this, "hadjustment"),
      vadjustment_(*this, "vadjustment"),
      hscroll_policy_(*this, "hscroll-policy", Gtk::SCROLL_NATURAL),
      vscroll_policy_(*this, "vscroll-policy", Gtk::SCROLL_NATURAL)
{
  set_has_window(true);
  property_hadjustment().signal_changed().connect(sigc::mem_fun(*this, &ibc::gtkmm::ImageViewBase::hadjustment_changed));
  property_vadjustment().signal_changed().connect(sigc::mem_fun(*this, &ibc::gtkmm::ImageViewBase::vadjustment_changed));

  m_width = 0;
  m_height = 0;
  m_org_width = 0;
  m_org_height = 0;
  m_offset_x    = 0;
  m_offset_y    = 0;
  m_window_x    = 0;
  m_window_y    = 0;
  m_window_width  = 0;
  m_window_height = 0;
  m_zoom          = 1.0;
  m_mouse_l_pressed = false;
  m_adjusments_modified = false;

  add_events(Gdk::SCROLL_MASK |
             Gdk::BUTTON_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
             Gdk::POINTER_MOTION_MASK);
}

ibc::gtkmm::ImageViewBase::~ImageViewBase()
{
}

void ibc::gtkmm::ImageViewBase::on_realize()
{
  // Do not call base class Gtk::Widget::on_realize().
  // It's intended only for widgets that set_has_window(false).
  set_realized();

  if(!m_window)
  {
    GdkWindowAttr attributes;

    std::memset(&attributes, 0, sizeof(decltype(attributes)));
    const Gtk::Allocation allocation(get_allocation());
    attributes.x = allocation.get_x();
    attributes.y = allocation.get_y();
    attributes.width = allocation.get_width();
    attributes.height = allocation.get_height();
    attributes.event_mask = get_events() | Gdk::EXPOSURE_MASK;
    attributes.window_type = GDK_WINDOW_CHILD;
    attributes.wclass = GDK_INPUT_OUTPUT;

    m_window = Gdk::Window::create(get_parent_window(), &attributes, Gdk::WA_X | Gdk::WA_Y);
    set_window(m_window);

    //make the widget receive expose events
    m_window->set_user_data(Gtk::Widget::gobj());
  }
}

void ibc::gtkmm::ImageViewBase::on_unrealize()
{
  m_window.reset();

  //Call base class:
  Gtk::Widget::on_unrealize();
}

bool ibc::gtkmm::ImageViewBase::on_button_press_event(GdkEventButton* button_event)
{
  m_mouse_l_pressed = true;
  m_mouse_x = button_event->x;
  m_mouse_y = button_event->y;
  m_offset_x_org= m_offset_x;
  m_offset_y_org = m_offset_y;
}

bool ibc::gtkmm::ImageViewBase::on_motion_notify_event(GdkEventMotion* motion_event)
{
  if (m_mouse_l_pressed == false)
    return false;

  if (m_width > m_window_width)
  {
    double d = m_offset_x_org + (m_mouse_x - motion_event->x);
    if (d > m_offset_x_max)
      d = m_offset_x_max;
    if (d < 0)
      d = 0;
    if (d != m_offset_x)
    {
      m_offset_x = d;
      const auto v = property_hadjustment().get_value();
      v->freeze_notify();
      v->set_value(m_offset_x);
      m_adjusments_modified = true;
      v->thaw_notify();
    }
  }
  if (m_height > m_window_height)
  {
    double d = m_offset_y_org + (m_mouse_y - motion_event->y);
    if (d > m_offset_y_max)
      d = m_offset_y_max;
    if (d < 0)
      d = 0;
    if (d != m_offset_y)
    {
      m_offset_y = d;
      const auto v = property_vadjustment().get_value();
      v->freeze_notify();
      v->set_value(m_offset_y);
      m_adjusments_modified = true;
      v->thaw_notify();
    }
  }
  return true;
}

bool ibc::gtkmm::ImageViewBase::on_button_release_event(GdkEventButton* release_event)
{
  m_mouse_l_pressed = false;
  return true;
}

bool ibc::gtkmm::ImageViewBase::on_scroll_event(GdkEventScroll *event)
{
  //std::cout << "wheel event\n"
  //          << "time = " << event->time << std::endl
  //          << "x = " << event->x << std::endl
  //          << "y = " << event->y << std::endl
  //          << "state = " << event->state << std::endl
  //          << "direction = " << event->direction << std::endl
  //          << "delta_x = " << event->delta_x << std::endl
  //          << "delta_y = " << event->delta_y << std::endl;

  double v;
  double prev_zoom = m_zoom;

  if (event->direction == 0)
    v = 0.02;
  else
    v = -0.02;
  v = log10(m_zoom) + v;
  m_zoom = pow(10, v);
  if (fabs(m_zoom - 1.0) <= 0.01)
    m_zoom = 1.0;
  if (m_zoom <= 0.01)
    m_zoom = 0.01;

  double prev_width = m_width;
  double prev_height = m_height;
  v = m_org_width * m_zoom;
  m_width = v;
  v = m_org_height * m_zoom;
  m_height = v;

  if (m_width <= m_window_width)
    m_offset_x = 0;
  else
  {
    if (prev_width <= m_window_width)
      v = -1 * (m_window_width - prev_width) / 2.0;
    else
      v = 0;
    v = v + (m_offset_x + event->x) / prev_zoom;
    v = v * m_zoom - event->x;
    m_offset_x = v;
    m_offset_x_max = m_width - m_window_width;
    if (m_offset_x > m_offset_x_max)
      m_offset_x = m_offset_x_max;
    if (m_offset_x < 0)
      m_offset_x = 0;
  }

  if (m_height <= m_window_height)
    m_offset_y = 0;
  else
  {
    if (prev_height <= m_window_height)
      v = -1 * (m_window_height - prev_height) / 2.0;
    else
      v = 0;
    v = (m_offset_y + event->y) / prev_zoom;
    v = v * m_zoom - event->y;
    m_offset_y = v;
    m_offset_y_max = m_height - m_window_height;
    if (m_offset_y > m_offset_y_max)
      m_offset_y = m_offset_y_max;
    if (m_offset_y < 0)
      m_offset_y = 0;
  }

  configure_hadjustment();
  configure_vadjustment();
  queue_draw();

  return true;
}

void ibc::gtkmm::ImageViewBase::on_size_allocate(Gtk::Allocation& allocation)
{
  //Do something with the space that we have actually been given:
  //(We will not be given heights or widths less than we have requested, though
  // we might get more)
  
  //Use the offered allocation for this container:
  set_allocation(allocation);
  m_window_x = allocation.get_x();
  m_window_y = allocation.get_y();
  m_window_width = allocation.get_width();
  m_window_height = allocation.get_height();

  if(m_window)
  {
    m_window->move_resize(m_window_x, m_window_y, m_window_width, m_window_height);
    configure_hadjustment();
    configure_vadjustment();
  }
}

void ibc::gtkmm::ImageViewBase::hadjustment_changed()
{
  const auto v = property_hadjustment().get_value();
  if (!v)
    return;
  hadjustment_connection_.disconnect();
  hadjustment_connection_ = v->signal_value_changed().connect(sigc::mem_fun(*this, &ibc::gtkmm::ImageViewBase::adjustment_value_changed));
  configure_hadjustment();
}

void ibc::gtkmm::ImageViewBase::vadjustment_changed()
{
  const auto v = property_vadjustment().get_value();
  if (!v)
    return;
  vadjustment_connection_.disconnect();
  vadjustment_connection_ = v->signal_value_changed().connect(sigc::mem_fun(*this, &ibc::gtkmm::ImageViewBase::adjustment_value_changed));
  configure_vadjustment();
}

void ibc::gtkmm::ImageViewBase::configure_hadjustment()
{
  const auto v = property_hadjustment().get_value();
  if (!v || m_window_width == 0)
    return;
  v->freeze_notify();
  if (m_width <= m_window_width)
  {
    m_offset_x = 0;
    v->set_value(0);
    v->set_upper(0);
    v->set_step_increment(0);
    v->set_page_size(0);
  }
  else
  {
    m_offset_x_max = m_width - m_window_width;
    if (m_offset_x > m_offset_x_max)
      m_offset_x = m_offset_x_max;
    v->set_upper(m_offset_x_max);
    v->set_value(m_offset_x);
    v->set_step_increment(1);
    v->set_page_size(10);
    m_adjusments_modified = true;
  }
  v->thaw_notify();
}

void ibc::gtkmm::ImageViewBase::configure_vadjustment()
{
  const auto v = property_vadjustment().get_value();
  if (!v || m_window_height == 0)
    return;
  v->freeze_notify();
  if (m_height <= m_window_height)
  {
    m_offset_y = 0;
    v->set_value(0);
    v->set_upper(0);
    v->set_step_increment(0);
    v->set_page_size(0);
  }
  else
  {
    m_offset_y_max = m_height - m_window_height;
    if (m_offset_y > m_offset_y_max)
      m_offset_y = m_offset_y_max;
    v->set_upper(m_offset_y_max);
    v->set_value(m_offset_y);
    v->set_step_increment(1);
    v->set_page_size(10);
    m_adjusments_modified = true;
  }
  v->thaw_notify();
}

void ibc::gtkmm::ImageViewBase::adjustment_value_changed()
{
  if (m_width > m_window_width && m_adjusments_modified == false)
  {
    const auto v = property_hadjustment().get_value();
    m_offset_x = v->get_value();
  }
  if (m_height > m_window_height && m_adjusments_modified == false)
  {
    const auto v = property_vadjustment().get_value();
    m_offset_y = v->get_value();
  }
  m_adjusments_modified = false;
  queue_draw();
}
