// =============================================================================
//  endian.h
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
  \file     ibc/base/endian.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/01/26
  \brief    Header file for handling endian
*/

#ifndef IBC_ENDIAN_H_
#define IBC_ENDIAN_H_

// Includes --------------------------------------------------------------------
#include "ibc/base/types.h"

// Macros ----------------------------------------------------------------------
#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
  #if __BYTE_ORDER == __LITTLE_ENDIAN
    #define __LITTLE_ENDIAN__
  #else
    #define __BIG_ENDIAN__
  #endif
#endif
//
#ifdef __LITTLE_ENDIAN__
 #define CONV_FROM_LITTLE_ENDIAN(x)   x
 #define CONV_FROM_BIG_ENDIAN(x)      ibc::Endian::swap(x)
 //
 #define CONV_TO_LITTLE_ENDIAN(x)     x
 #define CONV_To_BIG_ENDIAN(x)        ibc::Endian::swap(x)
#else
 #define CONV_FROM_LITTLE_ENDIAN(x)   ibc::Endian::swap(x)
 #define CONV_FROM_BIG_ENDIAN(x)      x
 //
 #define CONV_TO_LITTLE_ENDIAN(x)     ibc::Endian::swap(x)
 #define CONV_TO_BIG_ENDIAN(x)        x
#endif

// Namespace -------------------------------------------------------------------
namespace ibc
{
  class  Endian
  {
  public:
    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // swap
    // -------------------------------------------------------------------------
    // [TODO] should use _byteswap_ushort() with VC++ compiler
    //
    static uint16  swap(uint16 inValue)
    {
      uint16  result;
      result  = inValue << 8;
      result |= inValue >> 8;
      return result;
    }
    // -------------------------------------------------------------------------
    // swap
    // -------------------------------------------------------------------------
    static uint32  swap(uint32 inValue)
    {
      uint32  result;
      result  = inValue << 24;
      result |= (inValue&0x0000FF00) << 8;
      result |= (inValue&0x00FF0000) >> 8;
      result |= inValue >> 24;
      return result;
    }
    // -------------------------------------------------------------------------
    // swap
    // -------------------------------------------------------------------------
    // TODO : might need to use swap32 twice instead
    static uint64  swap(uint64 inValue)
    {
      uint64  result;
      result  = inValue << 56;
      result |= (inValue&0x000000000000FF00) << 40;
      result |= (inValue&0x0000000000FF0000) << 24;
      result |= (inValue&0x00000000FF000000) <<  8;
      result |= (inValue&0x000000FF00000000) >>  8;
      result |= (inValue&0x0000FF0000000000) >> 24;
      result |= (inValue&0x00FF000000000000) >> 40;
      result |= inValue >> 56;
      return result;
    }
    // -------------------------------------------------------------------------
    // swap
    // -------------------------------------------------------------------------
    // Note that the following code assumes that sizeof(float) == 4
    //
    static float  swap(float inValue)
    {
      union
      {
       float  value_float;
       uint32 value_uint32;
      } u;
      u.value_float = inValue;
      u.value_uint32 = swap(u.value_uint32);
      return u.value_float;
    }
    // -------------------------------------------------------------------------
    // swap
    // -------------------------------------------------------------------------
    // Note that the following code assumes that sizeof(double) == 8
    //
    static double  swap(double inValue)
    {
      union
      {
       double  value_double;
       uint64 value_uint64;
      } u;
      u.value_double = inValue;
      u.value_uint64 = swap(u.value_uint64);
      return u.value_double;
    }
    // -------------------------------------------------------------------------
    // swapBytes
    // -------------------------------------------------------------------------
    //
    static void swapBytes(void *inBytes, size_t inSize)
    {
      unsigned char *dataPtr = (unsigned char *)inBytes;
      unsigned char t;
      for (size_t i = 0; i < inSize / 2; i++)
      {
        t = dataPtr[i];
        dataPtr[i] = dataPtr[inSize - i - 1];
        dataPtr[inSize - i - 1] = t;
      }
    }
  };
};

#endif  // IBC_ENDIAN_H_
