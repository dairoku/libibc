// =============================================================================
//  image_exception.h
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
  \file     ibc/image/image_exception.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/10
  \brief    Header file for handling the exception

  This file defines exception classes for the IBC Library
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
