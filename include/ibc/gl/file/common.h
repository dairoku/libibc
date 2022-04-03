// =============================================================================
//  common.h
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
  \file     ibc/gl/file/common.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/04/30
  \brief    Header file for common routines for handling files
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
    // skipLines
    // -------------------------------------------------------------------------
    static bool skipLines(const char *inStr, size_t inLen, size_t inLines, size_t *outOffset)
    {
      *outOffset = 0;
      while (inLines != 0)
      {
        size_t  len;
        if (getLineLength(inStr, inLen, &len) == false)
          return false;
        inStr += len;
        inLen -= len;
        (*outOffset) += len;
        inLines--;
      }
      return true;
    }
    // -------------------------------------------------------------------------
    // getLineLength
    // -------------------------------------------------------------------------
    static bool getLineLength(const char *inStr, size_t inLen, size_t *outLen)
    {
      *outLen = 0;
      while (inLen != 0)
      {
        (*outLen)++;
        if (*inStr == CHAR_LF_CODE)
          return true;
        if (*inStr == CHAR_CR_CODE)
        {
          if (inLen == 1)
            return true;
          if (inStr[1] == CHAR_LF_CODE)
            (*outLen)++;
          return true;
        }
        inStr++;
        inLen--;
      }
      return false;
    }
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
          if (r.len == 0) // If we found separators in series, we just skip those
          {
            r.from++;
          }
          else
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
