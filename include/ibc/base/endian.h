// =============================================================================
//  endian.h
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
    static uint16_t  swap(uint16_t inValue)
    {
      uint16_t  result;
      result  = inValue << 8;
      result |= inValue >> 8;
      return result;
    }
    // -------------------------------------------------------------------------
    // swap
    // -------------------------------------------------------------------------
    static uint32_t  swap(uint32_t inValue)
    {
      uint32_t  result;
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
    static uint64_t  swap(uint64_t inValue)
    {
      uint64_t  result;
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
       uint32_t value_uint32_t;
      } u;
      u.value_float = inValue;
      u.value_uint32_t = swap(u.value_uint32_t);
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
       uint64_t value_uint64_t;
      } u;
      u.value_double = inValue;
      u.value_uint64_t = swap(u.value_uint64_t);
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
