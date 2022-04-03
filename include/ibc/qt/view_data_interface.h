// =============================================================================
//  view_data_interface.h
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
  \file     ibc/qt/view_data_interface.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/04/28
  \brief    Header file for the MVC view and model (view) inteface deinition
*/

#ifndef IBC_QT_VIEW_DATA_INTERFACE_H_
#define IBC_QT_VIEW_DATA_INTERFACE_H_

// Includes --------------------------------------------------------------------
#include <cstring>

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace qt
 {
  // ---------------------------------------------------------------------------
  // ViewDataInterface interface class
  // ---------------------------------------------------------------------------
  class  ViewDataInterface
  {
  public:
    //  member functions
    virtual void    queueRedrawWidget()   = 0;
    virtual void    markAsImageSizeChanged()   = 0;
  };
 };
};

#endif  // #ifdef IBC_QT_VIEW_DATA_INTERFACE_H_
