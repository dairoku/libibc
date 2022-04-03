// =============================================================================
//  image_exception.h
//
//  Written in 2018 by Dairoku Sekiguchi (sekiguchi at acm dot org)
//
//  To the extent possible under law, the author(s) have dedicated all copyright
//  and related and neighboring rights to this software to the public domain worldwide.
//  This software is distributed without any warranty.
//
//  You should have received a copy of the CC0 Public Domain Dedication along with
//  this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// =============================================================================
/*!
  \file     ibc/image/image_exception.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/10
  \brief    Header file for handling the image related exception
*/

#ifndef IBC_IMAGE_EXCEPTION_H_
#define IBC_IMAGE_EXCEPTION_H_

// Includes --------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "ibc/base/exception.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
  // ---------------------------------------------------------------------------
  // BaseException class
  // ---------------------------------------------------------------------------
  class  ImageException : public Exception
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    ImageException(int inExceptionCode, const char *inDescription,
                  const char *inLocation, int inOSErrorCode = 0)
      : Exception("ImageException", inExceptionCode, inDescription,
                  inLocation, inOSErrorCode)
    {
    }
    ImageException(const ImageException &inEx)
      : Exception(inEx)
    {
    }
    virtual ~ImageException() {};

    // Constatns ---------------------------------------------------------------
    enum ExceptionCode
    {
      FILE_FORMAT_ERROR = Exception::SUB_CLASS_ERROR
    };
  };
};

#endif  // IBC_IMAGE_EXCEPTION_H_
