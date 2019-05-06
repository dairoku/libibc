// =============================================================================
//  common.h
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
  \file     ibc/gl/file/common.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/04/30
  \brief    Header file for the PLY file

  This file defines the class for handling the PLY polygon file format.
*/

#ifndef IBC_GL_FILE_COMMON_H_
#define IBC_GL_FILE_COMMON_H_

// Includes --------------------------------------------------------------------
#include <vector>
#include <string>
#include "ibc/base/types.h"

// Namespace -------------------------------------------------------------------
//namespace ibc::gl::file // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace file
{
  // ---------------------------------------------------------------------------
  // Common class
  // ---------------------------------------------------------------------------
  class Common
  {
  public:
    // Constants ---------------------------------------------------------------
    static const char  CHAR_TAB_CODE    = 0x9;
    static const char  CHAR_LF_CODE     = 0xA;
    static const char  CHAR_CR_CODE     = 0xD;
    static const char  CHAR_SPACE_CODE  = 0x20;
    //
    static const char  CHAR_EOL_CODE  = CHAR_LF_CODE;

    // -------------------------------------------------------------------------
    // Typedefs
    // -------------------------------------------------------------------------
    typedef struct
    {
      size_t  from;
      size_t  len;
    } Range;

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // findLines
    // -------------------------------------------------------------------------
    static size_t findLines(const char *inStr, size_t inLen, std::vector<Range> *outLines)
    {
      const char  separators[2] = {CHAR_LF_CODE, CHAR_CR_CODE};
      return charTokenizer(inStr, inLen, outLines, separators, 2);
    }
    // -------------------------------------------------------------------------
    // findWords
    // -------------------------------------------------------------------------
    static size_t findWords(const char *inStr, size_t inLen, std::vector<Range> *outWords)
    {
      const char  separators[2] = {CHAR_SPACE_CODE, CHAR_TAB_CODE};
      return charTokenizer(inStr, inLen, outWords, separators, 2);
    }
    // -------------------------------------------------------------------------
    // charTokenizer
    // -------------------------------------------------------------------------
    static size_t charTokenizer(const char *inStr, size_t inLen, std::vector<Range> *outRanges,
                          const char *inSeparators, int inSeparatorNum)
    {
      size_t  initialSize, total;
      Range r;

      initialSize = outRanges->size();
      total = 0;
      r.from = 0;
      r.len = 0;
      while (*inStr != 0 && total != inLen)
      {
        bool foundSepartor = false;
        for (int i = 0; i < inSeparatorNum; i++)
          if (*inStr == inSeparators[i])
          {
            foundSepartor = true;
            break;
          }
        //
        if (foundSepartor == false)
        {
          r.len++;
        }
        else
        {
          if (r.len != 0)
          {
            outRanges->push_back(r);
            r.from += r.len + 1;
            r.len = 0;
          }
        }
        inStr++;
        total++;
      }
      if (r.len != 0)
        outRanges->push_back(r);
      return outRanges->size() - initialSize;
    }
  };
};};};

#endif  // #ifdef IBC_GL_FILE_PLY_H_
