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
    : Glib::ObjectBase("ImageViewBase"),
      mHAdjustment(*this, "hadjustment"),
      mVAdjustment(*this, "vadjustment"),
      mHScrollPolicy(*this, "hscroll-policy", Gtk::SCROLL_NATURAL),
      mVScrollPolicy(*this, "vscroll-policy", Gtk::SCROLL_NATURAL)
{
  set_has_window(true);
  property_hadjustment().signal_changed().connect(sigc::mem_fun(*this, &ibc::gtkmm::ImageViewBase::hAdjustmentChanged));
  property_vadjustment().signal_changed().connect(sigc::mem_fun(*this, &ibc::gtkmm::ImageViewBase::vAdjustmentChanged));

  mWidth = 0;
  mHeight = 0;
  mOrgWidth = 0;
  mOrgHeight = 0;
  mOffsetX    = 0;
  mOffsetY    = 0;
  mOffsetXMax = 0;
  mOffsetYMax = 0;
  mOffsetXOrg = 0;
  mOffsetYOrg = 0;
  mWindowX    = 0;
  mWindowY    = 0;
  mWindowWidth  = 0;
  mWindowHeight = 0;
  mZoom          = 1.0;
  mMouseLPressed = false;
  mAdjusmentsModified = false;

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

  if(!mWindow)
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

    mWindow = Gdk::Window::create(get_parent_window(), &attributes, Gdk::WA_X | Gdk::WA_Y);
    set_window(mWindow);

    //make the widget receive expose events
    mWindow->set_user_data(Gtk::Widget::gobj());
  }
}

void ibc::gtkmm::ImageViewBase::on_unrealize()
{
  mWindow.reset();

  //Call base class:
  Gtk::Widget::on_unrealize();
}

bool ibc::gtkmm::ImageViewBase::on_button_press_event(GdkEventButton* button_event)
{
  mMouseLPressed = true;
  m_mouse_x = button_event->x;
  m_mouse_y = button_event->y;
  mOffsetXOrg= mOffsetX;
  mOffsetYOrg = mOffsetY;
}

bool ibc::gtkmm::ImageViewBase::on_motion_notify_event(GdkEventMotion* motion_event)
{
  if (mMouseLPressed == false)
    return false;

  if (mWidth > mWindowWidth)
  {
    double d = mOffsetXOrg + (m_mouse_x - motion_event->x);
    if (d > mOffsetXMax)
      d = mOffsetXMax;
    if (d < 0)
      d = 0;
    if (d != mOffsetX)
    {
      mOffsetX = d;
      const auto v = property_hadjustment().get_value();
      v->freeze_notify();
      v->set_value(mOffsetX);
      mAdjusmentsModified = true;
      v->thaw_notify();
    }
  }
  if (mHeight > mWindowHeight)
  {
    double d = mOffsetYOrg + (m_mouse_y - motion_event->y);
    if (d > mOffsetYMax)
      d = mOffsetYMax;
    if (d < 0)
      d = 0;
    if (d != mOffsetY)
    {
      mOffsetY = d;
      const auto v = property_vadjustment().get_value();
      v->freeze_notify();
      v->set_value(mOffsetY);
      mAdjusmentsModified = true;
      v->thaw_notify();
    }
  }
  return true;
}

bool ibc::gtkmm::ImageViewBase::on_button_release_event(GdkEventButton* release_event)
{
  mMouseLPressed = false;
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
  double prev_zoom = mZoom;

  if (event->direction == 0)
    v = 0.02;
  else
    v = -0.02;
  v = log10(mZoom) + v;
  mZoom = pow(10, v);
  if (fabs(mZoom - 1.0) <= 0.01)
    mZoom = 1.0;
  if (mZoom <= 0.01)
    mZoom = 0.01;

  double prev_width = mWidth;
  double prev_height = mHeight;
  v = mOrgWidth * mZoom;
  mWidth = v;
  v = mOrgHeight * mZoom;
  mHeight = v;

  if (mWidth <= mWindowWidth)
    mOffsetX = 0;
  else
  {
    if (prev_width <= mWindowWidth)
      v = -1 * (mWindowWidth - prev_width) / 2.0;
    else
      v = 0;
    v = v + (mOffsetX + event->x) / prev_zoom;
    v = v * mZoom - event->x;
    mOffsetX = v;
    mOffsetXMax = mWidth - mWindowWidth;
    if (mOffsetX > mOffsetXMax)
      mOffsetX = mOffsetXMax;
    if (mOffsetX < 0)
      mOffsetX = 0;
  }

  if (mHeight <= mWindowHeight)
    mOffsetY = 0;
  else
  {
    if (prev_height <= mWindowHeight)
      v = -1 * (mWindowHeight - prev_height) / 2.0;
    else
      v = 0;
    v = (mOffsetY + event->y) / prev_zoom;
    v = v * mZoom - event->y;
    mOffsetY = v;
    mOffsetYMax = mHeight - mWindowHeight;
    if (mOffsetY > mOffsetYMax)
      mOffsetY = mOffsetYMax;
    if (mOffsetY < 0)
      mOffsetY = 0;
  }

  configureHAdjustment();
  configureVAdjustment();
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
  mWindowX = allocation.get_x();
  mWindowY = allocation.get_y();
  mWindowWidth = allocation.get_width();
  mWindowHeight = allocation.get_height();

  if(mWindow)
  {
    mWindow->move_resize(mWindowX, mWindowY, mWindowWidth, mWindowHeight);
    configureHAdjustment();
    configureVAdjustment();
  }
}

void ibc::gtkmm::ImageViewBase::hAdjustmentChanged()
{
  const auto v = property_hadjustment().get_value();
  if (!v)
    return;
  mHAdjustmentConnection.disconnect();
  mHAdjustmentConnection = v->signal_value_changed().connect(sigc::mem_fun(*this, &ibc::gtkmm::ImageViewBase::adjustmentValueChanged));
  configureHAdjustment();
}

void ibc::gtkmm::ImageViewBase::vAdjustmentChanged()
{
  const auto v = property_vadjustment().get_value();
  if (!v)
    return;
  mVAdjustmentConnection.disconnect();
  mVAdjustmentConnection = v->signal_value_changed().connect(sigc::mem_fun(*this, &ibc::gtkmm::ImageViewBase::adjustmentValueChanged));
  configureVAdjustment();
}

void ibc::gtkmm::ImageViewBase::configureHAdjustment()
{
  const auto v = property_hadjustment().get_value();
  if (!v || mWindowWidth == 0)
    return;
  v->freeze_notify();
  if (mWidth <= mWindowWidth)
  {
    mOffsetX = 0;
    v->set_value(0);
    v->set_upper(0);
    v->set_step_increment(0);
    v->set_page_size(0);
  }
  else
  {
    mOffsetXMax = mWidth - mWindowWidth;
    if (mOffsetX > mOffsetXMax)
      mOffsetX = mOffsetXMax;
    v->set_upper(mOffsetXMax);
    v->set_value(mOffsetX);
    v->set_step_increment(1);
    v->set_page_size(10);
    mAdjusmentsModified = true;
  }
  v->thaw_notify();
}

void ibc::gtkmm::ImageViewBase::configureVAdjustment()
{
  const auto v = property_vadjustment().get_value();
  if (!v || mWindowHeight == 0)
    return;
  v->freeze_notify();
  if (mHeight <= mWindowHeight)
  {
    mOffsetY = 0;
    v->set_value(0);
    v->set_upper(0);
    v->set_step_increment(0);
    v->set_page_size(0);
  }
  else
  {
    mOffsetYMax = mHeight - mWindowHeight;
    if (mOffsetY > mOffsetYMax)
      mOffsetY = mOffsetYMax;
    v->set_upper(mOffsetYMax);
    v->set_value(mOffsetY);
    v->set_step_increment(1);
    v->set_page_size(10);
    mAdjusmentsModified = true;
  }
  v->thaw_notify();
}

void ibc::gtkmm::ImageViewBase::adjustmentValueChanged()
{
  if (mWidth > mWindowWidth && mAdjusmentsModified == false)
  {
    const auto v = property_hadjustment().get_value();
    mOffsetX = v->get_value();
  }
  if (mHeight > mWindowHeight && mAdjusmentsModified == false)
  {
    const auto v = property_vadjustment().get_value();
    mOffsetY = v->get_value();
  }
  mAdjusmentsModified = false;
  queue_draw();
}
