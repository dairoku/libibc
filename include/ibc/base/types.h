// =============================================================================
//  types.h
//
//  MIT License
//
//  Copyright (c) 2018-2019 Dairoku Sekiguchi
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
  \file     ibc/base/types.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/11
  \brief    Header file for basic types
*/

#ifndef IBC_TYPES_H_
#define IBC_TYPES_H_

// Macros ----------------------------------------------------------------------
#define IBC_MACRO_TOSTRING_i(x)  #x
#define IBC_MACRO_JOIN_i(x, y)   x##y
//
#define IBC_MACRO_TOSTRING(x)           IBC_MACRO_TOSTRING_i(x)
#define IBC_MACRO_JOIN(x, y)            IBC_MACRO_JOIN_i(x,y)


// Types -----------------------------------------------------------------------

#ifndef int8
typedef char                  int8;
#endif
#ifndef uint8
typedef unsigned char         uint8;
#endif
#ifndef int16
typedef short                 int16;
#endif
#ifndef uint16
typedef unsigned short        uint16;
#endif
#ifndef int32
typedef int                   int32;
#endif
#ifndef uint32
typedef unsigned int          uint32;
#endif
#ifndef int64
 #ifdef _WIN32
  typedef __int64             int64;
 #else
  typedef long long           int64;
 #endif
#endif
#ifndef uint64
 #ifdef _WIN32
  typedef unsigned __int64    uint64;
 #else
  typedef unsigned long long  uint64;
 #endif
#endif

#define UNUSED(x)   (void)(x)

#endif  // IBC_TYPES_H_
