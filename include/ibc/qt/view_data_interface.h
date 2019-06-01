// =============================================================================
//  view_data_interface.h
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
