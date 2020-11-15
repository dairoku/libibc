// =============================================================================
//  ply.h
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
  \file     ibc/gl/file/ply.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/04/30
  \brief    Header file for handling the PLY format file
*/

#ifndef IBC_GL_FILE_PLY_H_
#define IBC_GL_FILE_PLY_H_

// Includes --------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <QFileInfo>
#include "ibc/base/types.h"
#include "ibc/base/endian.h"
#include "ibc/base/log.h"
#include "ibc/gl/data.h"
#include "ibc/gl/file/common.h"

// Macros ----------------------------------------------------------------------
#ifdef IBC_GL_FILE_PLY_TRACE_ENABLE
#define IBC_GL_FILE_PLY_TRACE() { IBC_TRACE(); }
#define IBC_GL_FILE_PLY_TRACE_ONCE() { IBC_TRACE_ONCE(); }
#else
#define IBC_GL_FILE_PLY_TRACE()
#define IBC_GL_FILE_PLY_TRACE_ONCE()
#endif

// Namespace -------------------------------------------------------------------
//namespace ibc::gl::file // <- nested namespace (C++17)
namespace ibc { namespace gl { namespace file
{
  // ---------------------------------------------------------------------------
  // PLYHeader class
  // ---------------------------------------------------------------------------
  class PLYHeader
  {
  public:
    // Enum --------------------------------------------------------------------
    enum  LineType
    {
      LINE_TYPE_NOT_SPECIFIED   = 0,
      LINE_TYPE_START           = 1,
      LINE_TYPE_END,
      LINE_TYPE_FORMAT,
      LINE_TYPE_COMMENT,
      LINE_TYPE_OBJ_INFO,
      LINE_TYPE_ELEMENT,
      LINE_TYPE_PROPERTY,
      LINE_TYPE_PROPERTY_LIST,
      LINE_TYPE_ANY             = 32767
    };
    enum  DataFormat
    {
      DATA_FORMAT_NOT_SPECIFIED         = 0,
      DATA_FORMAT_ASCII                 = 1,
      DATA_FORMAT_BINARY_LITTLE_ENDIAN,
      DATA_FORMAT_BINARY_BIG_ENDIAN,
      DATA_FORMAT_ANY                   = 32767
    };
    enum  DataType
    {
      DATA_TYPE_NOT_SPECIFIED   = 0,
      DATA_TYPE_INT8            = 1,
      DATA_TYPE_UINT8,
      DATA_TYPE_INT16,
      DATA_TYPE_UINT16,
      DATA_TYPE_INT32,
      DATA_TYPE_UINT32,
      DATA_TYPE_FLOAT32,
      DATA_TYPE_FLOAT64,
      //
      DATA_TYPE_CHAR            = 1024,   // Obsolete
      DATA_TYPE_UCHAR,
      DATA_TYPE_SHORT,
      DATA_TYPE_USHORT,
      DATA_TYPE_INT,
      DATA_TYPE_UINT,
      DATA_TYPE_FLOAT,
      DATA_TYPE_DOUBLE,
      //
      DATA_TYPE_ANY             = 32767
    };
    enum ElementType
    {
      ELEMENT_TYPE_NOT_SPECIFIED  = 0,
      ELEMENT_TYPE_VERTEX         = 1,
      ELEMENT_TYPE_FACE,
      ELEMENT_TYPE_EDGE,
      ELEMENT_TYPE_MATERIAL,
      //
      ELEMENT_TYPE_USER           = 16384,
      //
      ELEMENT_TYPE_ANY            = 32767
    };
    enum PropertyType
    {
      PROPERTY_TYPE_NOT_SPECIFIED = 0,
      PROPERTY_X                  = 1,
      PROPERTY_Y,
      PROPERTY_Z,
      PROPERTY_NX,
      PROPERTY_NY,
      PROPERTY_NZ,
      PROPERTY_RED,
      PROPERTY_GREEN,
      PROPERTY_BLUE,
      PROPERTY_ALPHA,
      PROPERTY_INTENSITY,
      PROPERTY_CONFIDENCE,
      //
      PROPERTY_VERTEX_METERIAL_INDEX  = 1024,
      //
      PROPERTY_FACE_VERTEX_INDEX      = 2048,
      //
      PROPERTY_EDGE_VERTEX1           = 3072,
      PROPERTY_EDGE_VERTEX2,
      //
      PROPERTY_TYPE_USER              = 16384,
      //
      PROPERTY_TYPE_ANY               = 32767
    };

    // -------------------------------------------------------------------------
    // Typedefs
    // -------------------------------------------------------------------------
    typedef struct
    {
      ElementType type;
      std::string name;
      size_t      num;
    } Element;
    typedef struct
    {
      size_t        elementIndex;
      PropertyType  type;
      std::string   name;
      DataType      dataType;
      bool          isList;
      DataType      listNumType;
    } Property;

    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // PLYHeader
    // -------------------------------------------------------------------------
    PLYHeader()
    {
      IBC_GL_FILE_PLY_TRACE();
      mDataFormat = DATA_FORMAT_NOT_SPECIFIED;
    }
    // -------------------------------------------------------------------------
    // ~PLYHeader
    // -------------------------------------------------------------------------
    virtual ~PLYHeader()
    {
      IBC_GL_FILE_PLY_TRACE();
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // clear
    // -------------------------------------------------------------------------
    void clear()
    {
      IBC_GL_FILE_PLY_TRACE();
      mDataFormat = DATA_FORMAT_NOT_SPECIFIED;
      mFormatVersionStr.clear();
      mCommentStr.clear();
      mObjInfoStr.clear();
      mElements.clear();
      mProperties.clear();
    }
    // -------------------------------------------------------------------------
    // setFormat
    // -------------------------------------------------------------------------
    void  setFormat(DataFormat inFormat, const char *inVersionStr, size_t inVersionStrLen)
    {
      IBC_GL_FILE_PLY_TRACE();
      mDataFormat = inFormat;
      mFormatVersionStr.assign(inVersionStr, inVersionStrLen);
    }
    // -------------------------------------------------------------------------
    // getFormat
    // -------------------------------------------------------------------------
    DataFormat  getFormat() const
    {
      IBC_GL_FILE_PLY_TRACE();
      return mDataFormat;
    }
    // -------------------------------------------------------------------------
    // getFormatVersionStr
    // -------------------------------------------------------------------------
    const std::string &getFormatVersionStr() const
    {
      IBC_GL_FILE_PLY_TRACE();
      return mFormatVersionStr;
    }
    // -------------------------------------------------------------------------
    // getFormatStr
    // -------------------------------------------------------------------------
    std::string getFormatStr() const
    {
      IBC_GL_FILE_PLY_TRACE();
      std::string str;
      if (mDataFormat == DATA_FORMAT_NOT_SPECIFIED)
        return str;
      str.append(getDataFormatWord(getFormat()));
      str.append(" ");
      str.append(getFormatVersionStr());
      return str;
    }
    // -------------------------------------------------------------------------
    // addComment
    // -------------------------------------------------------------------------
    void  addComment(const char *inStr, size_t inStrLen)
    {
      IBC_GL_FILE_PLY_TRACE();
      if (inStr != NULL && inStrLen != 0)
        mCommentStr.append(inStr, inStrLen);
      mCommentStr.append(1, Common::CHAR_EOL_CODE);
    }
    // -------------------------------------------------------------------------
    // getCommentStr
    // -------------------------------------------------------------------------
    const std::string &getCommentStr() const
    {
      IBC_GL_FILE_PLY_TRACE();
      return mCommentStr;
    }
    // -------------------------------------------------------------------------
    // addObjInfo
    // -------------------------------------------------------------------------
    void  addObjInfo(const char *inStr, size_t inStrLen)
    {
      IBC_GL_FILE_PLY_TRACE();
      if (inStr != NULL && inStrLen != 0)
        mObjInfoStr.append(inStr, inStrLen);
      mObjInfoStr.append(1, Common::CHAR_EOL_CODE);
    }
    // -------------------------------------------------------------------------
    // getObjInfoStr
    // -------------------------------------------------------------------------
    const std::string &getObjInfoStr() const
    {
      IBC_GL_FILE_PLY_TRACE();
      return mObjInfoStr;
    }
    // -------------------------------------------------------------------------
    // addElement
    // -------------------------------------------------------------------------
    size_t  addElement(const char *inNameStr, size_t inNameStrLen, size_t inElementNum)
    {
      IBC_GL_FILE_PLY_TRACE();
      size_t  index = mElements.size();
      mElements.resize(index + 1);
      mElements[index].type = findElementTypetWord(inNameStr, inNameStrLen);
      if (mElements[index].type == ELEMENT_TYPE_USER)
        mElements[index].name = std::string_view(inNameStr, inNameStrLen);
      mElements[index].num  = inElementNum;
      return index;
    }
    // -------------------------------------------------------------------------
    // getElements
    // -------------------------------------------------------------------------
    const std::vector<Element> &getElements() const
    {
      IBC_GL_FILE_PLY_TRACE();
      return mElements;
    }
    // -------------------------------------------------------------------------
    // findElementIndex
    // -------------------------------------------------------------------------
    bool findElementIndex(ElementType inType, size_t *outIndex) const
    {
      IBC_GL_FILE_PLY_TRACE();
      for (size_t i = 0; i < mElements.size(); i++)
      {
        if (mElements[i].type == inType)
        {
          *outIndex = i;
          return true;
        }
      }
      return false;
    }
    // -------------------------------------------------------------------------
    // addProperty
    // -------------------------------------------------------------------------
    size_t  addProperty(size_t inElementIndex, const char *inNameStr, size_t inNameStrLen,
                        DataType inDataType)
    {
      IBC_GL_FILE_PLY_TRACE();
      size_t  index = mProperties.size();
      mProperties.resize(index + 1);
      mProperties[index].elementIndex = inElementIndex;
      mProperties[index].type = findPropertyTypeWord(inNameStr, inNameStrLen);
      if (mProperties[index].type == PROPERTY_TYPE_USER)
        mProperties[index].name       = std::string_view(inNameStr, inNameStrLen);
      mProperties[index].dataType     = inDataType;
      mProperties[index].isList       = false;
      mProperties[index].listNumType  = DATA_TYPE_NOT_SPECIFIED;
      return index;
    }
    // -------------------------------------------------------------------------
    // addPropertyList
    // -------------------------------------------------------------------------
    size_t  addPropertyList(size_t inElementIndex, const char *inNameStr, size_t inNameStrLen,
                            DataType inDataType, DataType inListNumType)
    {
      IBC_GL_FILE_PLY_TRACE();
      size_t  index = mProperties.size();
      mProperties.resize(index + 1);
      mProperties[index].elementIndex = inElementIndex;
      mProperties[index].type = findPropertyTypeWord(inNameStr, inNameStrLen);
      if (mProperties[index].type == PROPERTY_TYPE_USER)
        mProperties[index].name       = std::string_view(inNameStr, inNameStrLen);
      mProperties[index].dataType     = inDataType;
      mProperties[index].isList       = true;
      mProperties[index].listNumType  = inListNumType;
      return index;
    }
    // -------------------------------------------------------------------------
    // getProperties
    // -------------------------------------------------------------------------
    const std::vector<Property> &getProperties() const
    {
      IBC_GL_FILE_PLY_TRACE();
      return mProperties;
    }
    // -------------------------------------------------------------------------
    // getElementProperties
    // -------------------------------------------------------------------------
    size_t getElementProperties(size_t inElementIndex, std::vector<Property> *outProperties) const
    {
      IBC_GL_FILE_PLY_TRACE();
      for (size_t i = 0; i < mProperties.size(); i++)
        if (mProperties[i].elementIndex == inElementIndex)
          outProperties->push_back(mProperties[i]);
      return outProperties->size();
    }
    // -------------------------------------------------------------------------
    // findPropertyIndex
    // -------------------------------------------------------------------------
    bool findPropertyIndex(size_t inElementIndex, PropertyType inType, size_t *outIndex) const
    {
      IBC_GL_FILE_PLY_TRACE();
      for (size_t i = 0; i < mProperties.size(); i++)
        if (mProperties[i].elementIndex == inElementIndex && mProperties[i].type == inType)
        {
          *outIndex = i;
          return true;
        }
      return false;
    }
    // -------------------------------------------------------------------------
    // getElementSingleDataSize
    // -------------------------------------------------------------------------
    size_t getElementSingleDataSize(size_t inElementIndex, bool *outContainsList) const
    {
      IBC_GL_FILE_PLY_TRACE();
      size_t  elementSize = 0;
      *outContainsList = false;
      for (size_t i = 0; i < mProperties.size(); i++)
        if (mProperties[i].elementIndex == inElementIndex)
        {
          if (mProperties[i].isList == false)
            elementSize += sizeofDataType(mProperties[i].dataType);
          else
            *outContainsList = true;
        }
      return elementSize;
    }
    // -------------------------------------------------------------------------
    // getElementTotalSize
    // -------------------------------------------------------------------------
    size_t getElementTotalSize(size_t inElementIndex, const void *inDataPtr, size_t inDataSize) const
    {
      IBC_GL_FILE_PLY_TRACE();
      bool  containsList = false;
      size_t elementSize = getElementSingleDataSize(inElementIndex, &containsList);
      if (containsList == false)
        return elementSize * mElements[inElementIndex].num;
      //
      size_t offset = 0;
      bool  flipEndian = doesNeedToFlipEndian(mDataFormat);
      for (size_t i = 0; i < mElements[inElementIndex].num; i++)
        for (size_t j = 0; j < mProperties.size(); j++)
          if (mProperties[j].elementIndex == inElementIndex)
          {
            if (mProperties[j].isList == false)
              offset += sizeofDataType(mProperties[j].dataType);
            else
            {
              double num = getValue((const unsigned char *)inDataPtr, offset,
                                    mProperties[j].listNumType, flipEndian);
              offset += sizeofDataType(mProperties[j].listNumType);
              offset += sizeofDataType(mProperties[j].dataType) * num;
            }
            if (offset > inDataSize)
              return 0;
          }
      return offset;
    }
    // -------------------------------------------------------------------------
    // getElementDataOffset
    // -------------------------------------------------------------------------
    bool getElementDataOffset(size_t inElementIndex, const void *inDataPtr,
                                    size_t inDataSize, size_t *outOffset) const
    {
      IBC_GL_FILE_PLY_TRACE();
      *outOffset = 0;
      if (inElementIndex >= mElements.size())
        return false;
      if (mDataFormat != DATA_FORMAT_ASCII)
      {
        unsigned char *elementDataPtr = (unsigned char *)inDataPtr;
        for (size_t i = 0; i < inElementIndex; i++)
        {
          size_t elementSize = getElementTotalSize(i, elementDataPtr, inDataSize);
          if (elementSize == 0)
            return false;
          inDataSize -= elementSize;
          (*outOffset) += elementSize;
          elementDataPtr += elementSize;
        }
        return true;
      }
      char *elementStrPtr = (char *)inDataPtr;
      for (size_t i = 0; i < inElementIndex; i++)
      {
        size_t offset;
        if (Common::skipLines(elementStrPtr, inDataSize,
                              mElements[inElementIndex].num, &offset) == false)
          return false;
        elementStrPtr += offset;
        (*outOffset) += offset;
        inDataSize -= offset;
      }
      return true;
    }
    // -------------------------------------------------------------------------
    // getPropertyOffset
    // -------------------------------------------------------------------------
    bool  getPropertyOffset(size_t inElementIndex, PropertyType inType,
                            size_t *outOffset, DataType *outDataType) const
    {
      IBC_GL_FILE_PLY_TRACE();
      if (inType == PROPERTY_TYPE_NOT_SPECIFIED)
        return false;
      *outOffset = 0;
      for (size_t i = 0; i < mProperties.size(); i++)
        if (mProperties[i].elementIndex == inElementIndex)
        {
          if (mProperties[i].isList)  // We can't calculate the offset here in this case
            return false;
          if (mProperties[i].type == inType)
          {
            *outDataType = mProperties[i].dataType;
            return true;
          }
          if (mDataFormat != DATA_FORMAT_ASCII)
            (*outOffset) += sizeofDataType(mProperties[i].dataType);
          else
            (*outOffset) += 1;
        }
      return false;
    }

    // -------------------------------------------------------------------------
    // getColorFormatStr
    // -------------------------------------------------------------------------
    std::string  getColorFormatStr(ElementType inType) const
    {
      IBC_GL_FILE_PLY_TRACE();
      size_t  index;
      if (findElementIndex(inType, &index) == false)
        return std::string();
      return getColorFormatStr(index);
    }
    // -------------------------------------------------------------------------
    // getColorFormatStr
    // -------------------------------------------------------------------------
    std::string  getColorFormatStr(size_t inElementIndex) const
    {
      size_t  index;
      std::string str;
      if (findPropertyIndex(inElementIndex, PROPERTY_RED, &index))
        str.append("R");
      if (findPropertyIndex(inElementIndex, PROPERTY_GREEN, &index))
        str.append("G");
      if (findPropertyIndex(inElementIndex, PROPERTY_BLUE, &index))
        str.append("B");
      if (findPropertyIndex(inElementIndex, PROPERTY_ALPHA, &index))
        str.append("A");
      if (findPropertyIndex(inElementIndex, PROPERTY_INTENSITY, &index))
        str.append("I");
      if (findPropertyIndex(inElementIndex, PROPERTY_CONFIDENCE, &index))
        str.append("C");
      return str;
    }

    // Debug Functions ---------------------------------------------------------
    void  debugDumpHeader(std::ostream *outStream) const
    {
      IBC_GL_FILE_PLY_TRACE();
      *outStream << "PLY Header Object Debug Dump" << std::endl;
      *outStream << "Format : ";
      *outStream << std::string_view(getDataFormatWord(mDataFormat));
      *outStream << " " << mFormatVersionStr << std::endl;
      *outStream << "[Comments]" << std::endl; // TODO revise this
      *outStream << mCommentStr << std::endl;
      *outStream << "[ObjInfos]" << std::endl; // TODO revise this
      *outStream << mObjInfoStr << std::endl;
      size_t  processedNum = 0;
      for (size_t i = 0; i < mElements.size(); i++)
      {
        *outStream << "Element : ";
        *outStream << std::string_view(getElementTypeWord(mElements[i].type));
        *outStream << " ";
        *outStream << mElements[i].name;
        *outStream << " ";
        *outStream << mElements[i].num << std::endl;
        for (size_t j = 0; j < mProperties.size(); j++)
        {
          if (mProperties[j].elementIndex == i)
          {
            if (mProperties[j].isList == false)
            {
              *outStream << " Property : ";
              *outStream << std::string_view(getDataTypeWord(mProperties[j].dataType));
            }
            else
            {
              *outStream << " Property List: ";
              *outStream << std::string_view(getDataTypeWord(mProperties[j].listNumType));
              *outStream << " ";
              *outStream << std::string_view(getDataTypeWord(mProperties[j].dataType));
            }
            *outStream << " ";
            *outStream << std::string_view(getPropertyTypeWord(mProperties[j].type));
            *outStream << std::endl;
            processedNum++;
          }
        }
      }
      if (processedNum != mProperties.size())
      {
        *outStream << "<ERROR> : There are unreferenced properties" << std::endl;
      }
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // create
    // -------------------------------------------------------------------------
    static PLYHeader *create(const char *inHeaderStr, size_t inLen,
                             size_t *outHeaderSize)
    {
      IBC_GL_FILE_PLY_TRACE();
      *outHeaderSize = findPLYHeader(inHeaderStr, inLen);
      if (*outHeaderSize == 0)
        return NULL;
      PLYHeader *header = new PLYHeader();
      if (parseHeader(inHeaderStr, *outHeaderSize, header) == false)
      {
        delete header;
        return NULL;
      }
      return header;
    }
    // -------------------------------------------------------------------------
    // parseHeader
    // -------------------------------------------------------------------------
    static bool parseHeader(const char *inHeaderStr, size_t inLen,
                            PLYHeader *outHeader)
    {
      IBC_GL_FILE_PLY_TRACE();
      bool  foundStart    = false;
      bool  foundEnd      = false;
      bool  foundFormat   = false;
      bool  foundElement  = false;
      size_t  elementCount  = 0;
      size_t  propertyCount = 0;
      size_t  elementIndex = 0;
      size_t  elementNum = 0;
      size_t  len;
      std::vector<Common::Range> lines;
      std::vector<Common::Range> words;
      DataFormat  format;

      outHeader->clear();

      Common::findLines(inHeaderStr, inLen, &lines);
      if (lines.size() < 2)
      {
        IBC_LOG_ERROR("Invaild header string");
        return false;
      }
      //
      for (size_t i = 0; i < lines.size(); i++)
      {
        words.clear();
        const char  *linePtr = &(inHeaderStr[lines[i].from]);
        Common::findWords(linePtr, lines[i].len, &words);
        if (words.size() == 0)
        {
          IBC_LOG_ERROR("A blank line in the PLY header");
          return false;
        }
        const char  *wordPtr = &(linePtr[words[0].from]);
        LineType type = findReservedWord(wordPtr, words[0].len);
        // START word check
        if (foundStart)
        {
          if (type == LINE_TYPE_START)
          {
            IBC_LOG_ERROR("Found a \"ply\" more than twice");
            return false;
          }
        }
        else
        {
          if (type != LINE_TYPE_START)
          {
            IBC_LOG_ERROR("The header does not start with \"ply\"");
            return false;
          }
        }
        // Element definition check
        if (foundElement)
        {
          if (type != LINE_TYPE_PROPERTY)
          {
            if (propertyCount == 0)
            {
              IBC_LOG_WARNING("The element defintion with no property");
              //return false;
            }
            foundElement = false;
            propertyCount = 0;
          }
        }
        // Handle each line
        switch (type)
        {
          case LINE_TYPE_START:
            foundStart = true;
            break;
          case LINE_TYPE_END:
            foundEnd = true;
            break;
          case LINE_TYPE_FORMAT:
            if (foundFormat)
            {
              IBC_LOG_ERROR("Found a \"format\" more than twice");
              return false;
            }
            if (words.size() != 3)
            {
              IBC_LOG_ERROR("Invalid format line");
              return false;
            }
            format = findDataFormatWord(&(linePtr[words[1].from]), words[1].len);
            if (format == DATA_FORMAT_NOT_SPECIFIED)
            {
              IBC_LOG_ERROR("Unknown data format");
              return false;
            }
            // ToDo check version string (at least output warning, if it is not "1.0")
            outHeader->setFormat(format, &(linePtr[words[2].from]), words[2].len);
            foundFormat = true;
            break;
          case LINE_TYPE_COMMENT:
            if (words.size() > 1) // Skip a blank comment line
            {
              len = lines[i].len - words[1].from;
              outHeader->addComment(&(linePtr[words[1].from]), len);
            }
            break;
          case LINE_TYPE_OBJ_INFO:
            if (words.size() > 1) // Skip a blank obj_info line
            {
              len = lines[i].len - words[1].from;
              outHeader->addObjInfo(&(linePtr[words[1].from]), len);
            }
            break;
          case LINE_TYPE_ELEMENT:
            if (words.size() < 3)
            {
              IBC_LOG_ERROR("Invalid element line");
              return false;
            }
            // TODO : Clean up here
            //num = std::stoul(std::string_view(&(linePtr[words[2].from]), words[2].len));
            elementNum = std::stoul(std::string(&(linePtr[words[2].from]), words[2].len));
            if (elementNum != 0)
            {
              elementIndex = outHeader->addElement(&(linePtr[words[1].from]), words[1].len, elementNum);
              elementCount++;
            }
            foundElement = true;
            break;
          case LINE_TYPE_PROPERTY:
            if (foundElement == false)
            {
              IBC_LOG_ERROR("Property without an element line");
              return false;
            }
            if (elementNum == 0) // Just skip
            {
              propertyCount++;
              break;
            }
            if (words.size() < 3)
            {
              IBC_LOG_ERROR("Invalid element line");
              return false;
            }
            if (isPropertyList(&(linePtr[words[1].from]), words[1].len) == false)
            {
              outHeader->addProperty(elementIndex, &(linePtr[words[2].from]), words[2].len,
                                     findDataTypeWord(&(linePtr[words[1].from]), words[1].len));
            }
            else
            {
              if (words.size() != 5)
              {
                IBC_LOG_ERROR("Invalid element line");
                return false;
              }
              outHeader->addPropertyList(elementIndex, &(linePtr[words[4].from]), words[4].len,
                                     findDataTypeWord(&(linePtr[words[3].from]), words[3].len),
                                     findDataTypeWord(&(linePtr[words[2].from]), words[2].len));
            }
            propertyCount++;
            break;
          default:
            IBC_LOG_ERROR("Invalid line");
            return false;
        }
        if (foundEnd)
          break;
      }
      if (foundEnd == false)
      {
        IBC_LOG_ERROR("\"end_header\" is missing");
        return false;
      }
      if (elementCount == 0)
      {
        IBC_LOG_ERROR("element is missing");
        return false;
      }
      if (foundFormat == false)
      {
        IBC_LOG_ERROR("format is missing");
        return false;
      }
      //
      return true;
    }
    // -------------------------------------------------------------------------
    // findPLYHeader
    // -------------------------------------------------------------------------
    static size_t findPLYHeader(const char *inHeaderStr, size_t inLen)
    {
      IBC_GL_FILE_PLY_TRACE();
      if (inLen < MIN_HEADER_LEN)
      {
        IBC_LOG_ERROR("inLen < MIN_HEADER_LEN");
        return 0;
      }
      // Check the PLY header start string "ply"
      const char  *bufPtr = inHeaderStr;
      const char  *startStr = getReservedWord(LINE_TYPE_START);
      while (*startStr != 0)
      {
        if (*bufPtr != *startStr)
        {
          IBC_LOG_ERROR("\"ply\" is missing");
          return 0;
        }
        bufPtr++;
        startStr++;
      }
      // Check the enf of line marker size
      size_t  sizeOfEOLMaker = 1;
      if (bufPtr[0] == Common::CHAR_CR_CODE && bufPtr[1] == Common::CHAR_LF_CODE)
        sizeOfEOLMaker = 2;
      // Find the end string "end_header"
      std::string_view  strView(inHeaderStr, inLen);
      const char  *endStr = getReservedWord(LINE_TYPE_END);
      size_t  pos = strView.find(endStr);
      if (pos == std::basic_string<char>::npos)
      {
        IBC_LOG_ERROR("\"end_header\" is missing");
        return 0;
      }
      pos += strlen(endStr);
      // Check the last EOL marker code (Sanity check)
      if (sizeOfEOLMaker == 1)
      {
        if (!(strView[pos] == Common::CHAR_CR_CODE || strView[pos] == Common::CHAR_LF_CODE))
        {
          IBC_LOG_ERROR("Invalid EOL code after \"end_header\" #1");
          return 0;
        }
      }
      else
      {
        if (strView[pos] != Common::CHAR_CR_CODE && strView[pos+1] != Common::CHAR_LF_CODE)
        {
          IBC_LOG_ERROR("Invalid EOL code after \"end_header\" #2");
          return 0;
        }
      }
      return pos+sizeOfEOLMaker;
    }
    // -------------------------------------------------------------------------
    // isAsciiFormat
    // -------------------------------------------------------------------------
    static bool isAsciiFormat(DataFormat inFormat)
    {
      IBC_GL_FILE_PLY_TRACE();
      if (inFormat == DATA_FORMAT_ASCII)
        return false;
      return false;
    }
    // -------------------------------------------------------------------------
    // doesNeedToFlipEndian
    // -------------------------------------------------------------------------
    static bool doesNeedToFlipEndian(DataFormat inFormat)
    {
      IBC_GL_FILE_PLY_TRACE();
    #ifdef __LITTLE_ENDIAN__
      if (inFormat == DATA_FORMAT_BINARY_BIG_ENDIAN)
        return true;
    #endif
    #ifdef __BIG_ENDIAN__
      if (inFormat == DATA_FORMAT_BINARY_LITTLE_ENDIAN)
        return false;
    #endif
      return false;
    }
    // -------------------------------------------------------------------------
    // getValue
    // -------------------------------------------------------------------------
    static double getValue(const unsigned char *inDataPtr, size_t inOffset,
                            PLYHeader::DataType inType, bool inFlipEndian)
    {
      IBC_GL_FILE_PLY_TRACE_ONCE();
      unsigned char data[8];
      // We need to do this to avoid the data alignment issue
      ::memcpy(data, &(inDataPtr[inOffset]), PLYHeader::sizeofDataType(inType));
      //
      if (inFlipEndian)
        flipEndian(data, inType);
      //
      switch (inType)
      {
        case DATA_TYPE_INT8:
        case DATA_TYPE_CHAR:
          return (double )(*((int8 *)data));
        case DATA_TYPE_UINT8:
        case DATA_TYPE_UCHAR:
          return (double )(*((uint8 *)data));
        case DATA_TYPE_INT16:
        case DATA_TYPE_SHORT:
          return (double )(*((int16 *)data));
        case DATA_TYPE_UINT16:
        case DATA_TYPE_USHORT:
          return (double )(*((uint16 *)data));
        case DATA_TYPE_INT32:
        case DATA_TYPE_INT:
          return (double )(*((int32  *)data));
        case DATA_TYPE_UINT32:
        case DATA_TYPE_UINT:
          return (double )(*((uint32  *)data));
        case DATA_TYPE_FLOAT32:
        case DATA_TYPE_FLOAT:
          return (double )(*((float  *)data));
        case DATA_TYPE_FLOAT64:
        case DATA_TYPE_DOUBLE:
          return (double )(*((double  *)data));
        default:
          break;
      }
      return 0;
    }
    // -------------------------------------------------------------------------
    // setValue
    // -------------------------------------------------------------------------
    static void setValue(double inValue, unsigned char *outDataPtr, size_t inOffset,
                          DataType inType, bool inFlipEndian)
    {
      IBC_GL_FILE_PLY_TRACE_ONCE();
      unsigned char data[8];

      switch (inType)
      {
        case DATA_TYPE_INT8:
        case DATA_TYPE_CHAR:
          *((int8 *)data) = (int8 )inValue;
          break;
        case DATA_TYPE_UINT8:
        case DATA_TYPE_UCHAR:
          *((uint8 *)data) = (uint8 )inValue;
          break;
        case DATA_TYPE_INT16:
        case DATA_TYPE_SHORT:
          *((int16 *)data) = (int16 )inValue;
          break;
        case DATA_TYPE_UINT16:
        case DATA_TYPE_USHORT:
          *((uint16 *)data) = (uint16 )inValue;
          break;
        case DATA_TYPE_INT32:
        case DATA_TYPE_INT:
          *((int32 *)data) = (int32 )inValue;
          break;
        case DATA_TYPE_UINT32:
        case DATA_TYPE_UINT:
          *((uint32 *)data) = (uint32 )inValue;
          break;
        case DATA_TYPE_FLOAT32:
        case DATA_TYPE_FLOAT:
          *((float *)data) = (float )inValue;
          break;
        case DATA_TYPE_FLOAT64:
        case DATA_TYPE_DOUBLE:
          *((double *)data) = (double )inValue;
          break;
        default:
          break;
      }
      if (inFlipEndian)
        flipEndian(data, inType);
      //
      // We need to do this to avoid the data alignment issue
      ::memcpy(&(outDataPtr[inOffset]), data, sizeofDataType(inType));
    }
    // -------------------------------------------------------------------------
    // flipEndian
    // -------------------------------------------------------------------------
    static void flipEndian(unsigned char *ioDataPtr, DataType inType)
    {
      IBC_GL_FILE_PLY_TRACE();
      switch (inType)
      {
        case DATA_TYPE_INT16:
        case DATA_TYPE_UINT16:
        case DATA_TYPE_SHORT:
        case DATA_TYPE_USHORT:
          ibc::Endian::swapBytes(ioDataPtr, 2);
          break;
        case DATA_TYPE_INT32:
        case DATA_TYPE_UINT32:
        case DATA_TYPE_FLOAT32:
        case DATA_TYPE_INT:
        case DATA_TYPE_UINT:
        case DATA_TYPE_FLOAT:
          ibc::Endian::swapBytes(ioDataPtr, 4);
          break;
        case DATA_TYPE_FLOAT64:
        case DATA_TYPE_DOUBLE:
          ibc::Endian::swapBytes(ioDataPtr, 8);
          break;
        default:
          break;
      }
    }
    // -------------------------------------------------------------------------
    // getValueFromStr
    // -------------------------------------------------------------------------
    static double getValueFromStr(const char *inDataStrPtr, size_t inDataSize,
                            PLYHeader::DataType inType)
    {
      IBC_GL_FILE_PLY_TRACE_ONCE();
      std::string str(inDataStrPtr, inDataSize);
      double  v = 0;

      switch (inType)
      {
        case DATA_TYPE_INT8:
        case DATA_TYPE_CHAR:
          v = (double )std::stol(str);
          break;
        case DATA_TYPE_UINT8:
        case DATA_TYPE_UCHAR:
          v = (double )std::stoul(str);
          break;
        case DATA_TYPE_INT16:
        case DATA_TYPE_SHORT:
          v = (double )std::stol(str);
          break;
        case DATA_TYPE_UINT16:
        case DATA_TYPE_USHORT:
          v = (double )std::stoul(str);
          break;
        case DATA_TYPE_INT32:
        case DATA_TYPE_INT:
          v = (double )std::stol(str);
          break;
        case DATA_TYPE_UINT32:
        case DATA_TYPE_UINT:
          v = (double )std::stoul(str);
          break;
        case DATA_TYPE_FLOAT32:
        case DATA_TYPE_FLOAT:
          v = (double )std::stof(str);
          break;
        case DATA_TYPE_FLOAT64:
        case DATA_TYPE_DOUBLE:
          v = std::stod(str);
          break;
        default:
          break;
      }

      clipToDataTypeRange(inType, &v);
      return v;
    }

  protected:
    // Constants ---------------------------------------------------------------
    static const size_t  MIN_HEADER_LEN  = (3+1+10);   // "ply" + "\r" + "end_header"

    // Member variables --------------------------------------------------------
    DataFormat    mDataFormat;
    std::string   mFormatVersionStr;
    std::string   mCommentStr;
    std::string   mObjInfoStr;
    std::vector<Element>  mElements;
    std::vector<Property> mProperties;

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // getReservedWord
    // -------------------------------------------------------------------------
    static const char *getReservedWord(LineType inLine)
    {
      IBC_GL_FILE_PLY_TRACE();
      switch (inLine)
      {
        case LINE_TYPE_START:
          return "ply";
        case LINE_TYPE_END:
          return "end_header";
        case LINE_TYPE_FORMAT:
          return "format";
        case LINE_TYPE_COMMENT:
          return "comment";
        case LINE_TYPE_OBJ_INFO:
          return "obj_info";
        case LINE_TYPE_ELEMENT:
          return "element";
        case LINE_TYPE_PROPERTY:
          return "property";
        case LINE_TYPE_PROPERTY_LIST:
          return "list";
        default:
          break;
      }
      return "";
    }
    // -------------------------------------------------------------------------
    // findReservedWord
    // -------------------------------------------------------------------------
    static LineType findReservedWord(const char *inStr, size_t inLen)
    {
      IBC_GL_FILE_PLY_TRACE();
      const LineType  wordIndex[] = {
        LINE_TYPE_START, LINE_TYPE_END, LINE_TYPE_FORMAT,
        LINE_TYPE_COMMENT, LINE_TYPE_OBJ_INFO, LINE_TYPE_ELEMENT,
        LINE_TYPE_PROPERTY, LINE_TYPE_PROPERTY_LIST};
      //
      std::string_view  strView(inStr, inLen);
      size_t num = sizeof(wordIndex) / sizeof(PropertyType);
      for (size_t i = 0; i < num; i++)
        if (strView == getReservedWord(wordIndex[i]))
          return wordIndex[i];
      //
      return LINE_TYPE_NOT_SPECIFIED;
    }
    // -------------------------------------------------------------------------
    // isPropertyList
    // -------------------------------------------------------------------------
    static bool isPropertyList(const char *inStr, size_t inLen)
    {
      IBC_GL_FILE_PLY_TRACE();
      std::string_view  strView(inStr, inLen);
      if (strView == getReservedWord(LINE_TYPE_PROPERTY_LIST))
        return true;
      return false;
    }
    // -------------------------------------------------------------------------
    // getDataFormatWord
    // -------------------------------------------------------------------------
    static const char *getDataFormatWord(DataFormat inFormat)
    {
      IBC_GL_FILE_PLY_TRACE();
      switch (inFormat)
      {
        case DATA_FORMAT_ASCII:
          return "ascii";
        case DATA_FORMAT_BINARY_LITTLE_ENDIAN:
          return "binary_little_endian";
        case DATA_FORMAT_BINARY_BIG_ENDIAN:
          return "binary_big_endian";
        default:
          break;
      }
      return "";
    }
    // -------------------------------------------------------------------------
    // findDataFormatWord
    // -------------------------------------------------------------------------
    static DataFormat findDataFormatWord(const char *inStr, size_t inLen)
    {
      IBC_GL_FILE_PLY_TRACE();
      const DataFormat  wordIndex[] = {
        DATA_FORMAT_ASCII,
        DATA_FORMAT_BINARY_LITTLE_ENDIAN,
        DATA_FORMAT_BINARY_BIG_ENDIAN};
      //
      std::string_view  strView(inStr, inLen);
      size_t num = sizeof(wordIndex) / sizeof(PropertyType);
      for (size_t i = 0; i < num; i++)
        if (strView == getDataFormatWord(wordIndex[i]))
          return wordIndex[i];
      //
      return DATA_FORMAT_NOT_SPECIFIED;
    }
    // -------------------------------------------------------------------------
    // getDataTypeWord
    // -------------------------------------------------------------------------
    static const char *getDataTypeWord(DataType inFormat)
    {
      IBC_GL_FILE_PLY_TRACE();
      switch (inFormat)
      {
        case DATA_TYPE_INT8:
          return "int8";
        case DATA_TYPE_UINT8:
          return "uint8";
        case DATA_TYPE_INT16:
          return "int16";
        case DATA_TYPE_UINT16:
          return "uint16";
        case DATA_TYPE_INT32:
          return "int32";
        case DATA_TYPE_UINT32:
          return "uint32";
        case DATA_TYPE_FLOAT32:
          return "float32";
        case DATA_TYPE_FLOAT64:
          return "float64";
        //
        case DATA_TYPE_CHAR:
          return "char";
        case DATA_TYPE_UCHAR:
          return "uchar";
        case DATA_TYPE_SHORT:
          return "short";
        case DATA_TYPE_USHORT:
          return "ushort";
        case DATA_TYPE_INT:
          return "int";
        case DATA_TYPE_UINT:
          return "uint";
        case DATA_TYPE_FLOAT:
          return "float";
        case DATA_TYPE_DOUBLE:
          return "double";
        default:
          break;
      }
      return "";
    }
    // -------------------------------------------------------------------------
    // findDataTypeWord
    // -------------------------------------------------------------------------
    static DataType findDataTypeWord(const char *inStr, size_t inLen)
    {
      IBC_GL_FILE_PLY_TRACE();
      const DataType  wordIndex[] = {
        DATA_TYPE_INT8,     DATA_TYPE_UINT8,
        DATA_TYPE_INT16,    DATA_TYPE_UINT16,
        DATA_TYPE_INT32,    DATA_TYPE_UINT32,
        DATA_TYPE_FLOAT32,  DATA_TYPE_FLOAT64,
        //
        DATA_TYPE_CHAR,     DATA_TYPE_UCHAR,
        DATA_TYPE_SHORT,    DATA_TYPE_USHORT,
        DATA_TYPE_INT,      DATA_TYPE_UINT,
        DATA_TYPE_FLOAT,    DATA_TYPE_DOUBLE};
      //
      std::string_view  strView(inStr, inLen);
      size_t num = sizeof(wordIndex) / sizeof(PropertyType);
      for (size_t i = 0; i < num; i++)
        if (strView == getDataTypeWord(wordIndex[i]))
        {
          if (wordIndex[i] <  DATA_TYPE_CHAR)
            return wordIndex[i];
          else
            // Since we will not use the obsolete style internally
            return (DataType )(wordIndex[i] - DATA_TYPE_CHAR + DATA_TYPE_INT8);
        }
      //
      return DATA_TYPE_NOT_SPECIFIED;
    }
    // -------------------------------------------------------------------------
    // sizeofDataType
    // -------------------------------------------------------------------------
    static size_t sizeofDataType(DataType inType)
    {
      IBC_GL_FILE_PLY_TRACE_ONCE();
      switch (inType)
      {
        case DATA_TYPE_INT8:
        case DATA_TYPE_UINT8:
        case DATA_TYPE_CHAR:
        case DATA_TYPE_UCHAR:
          return 1;
        case DATA_TYPE_INT16:
        case DATA_TYPE_UINT16:
        case DATA_TYPE_SHORT:
        case DATA_TYPE_USHORT:
          return 2;
        case DATA_TYPE_INT32:
        case DATA_TYPE_UINT32:
        case DATA_TYPE_FLOAT32:
        case DATA_TYPE_INT:
        case DATA_TYPE_UINT:
        case DATA_TYPE_FLOAT:
          return 4;
        case DATA_TYPE_FLOAT64:
        case DATA_TYPE_DOUBLE:
          return 8;
        default:
          break;
      }
      return 0;
    }
    // -------------------------------------------------------------------------
    // clipToDataTypeRange
    // -------------------------------------------------------------------------
    static void clipToDataTypeRange(DataType inType, double *ioValue)
    {
      IBC_GL_FILE_PLY_TRACE();
      switch (inType)
      {
        case DATA_TYPE_INT8:
        case DATA_TYPE_CHAR:
          if (*ioValue < -128)
            *ioValue = -128;
          if (*ioValue > 127)
            *ioValue = 127;
          return;
        case DATA_TYPE_UINT8:
        case DATA_TYPE_UCHAR:
          if (*ioValue < 0)
            *ioValue = 0;
          if (*ioValue > 255)
            *ioValue = 255;
          return;
        case DATA_TYPE_INT16:
        case DATA_TYPE_SHORT:
          if (*ioValue < -32768)
            *ioValue = -32768;
          if (*ioValue > 32767)
            *ioValue = 32767;
          return;
        case DATA_TYPE_UINT16:
        case DATA_TYPE_USHORT:
          if (*ioValue < 0)
            *ioValue = 0;
          if (*ioValue > 65535)
            *ioValue = 65535;
          return;
        case DATA_TYPE_INT32:
        case DATA_TYPE_INT:
          if (*ioValue < -2147483648)
            *ioValue = -2147483648;
          if (*ioValue > 2147483647)
            *ioValue = 2147483647;
          return;
        case DATA_TYPE_UINT32:
        case DATA_TYPE_UINT:
          if (*ioValue < 0)
            *ioValue = 0;
          if (*ioValue > 4294967295)
            *ioValue = 4294967295;
          return;
        default:
          break;
      }
    }
    // -------------------------------------------------------------------------
    // getElementTypeWord
    // -------------------------------------------------------------------------
    static const char *getElementTypeWord(ElementType inType)
    {
      IBC_GL_FILE_PLY_TRACE();
      switch (inType)
      {
        case ELEMENT_TYPE_VERTEX:
          return "vertex";
        case ELEMENT_TYPE_FACE:
          return "face";
        case ELEMENT_TYPE_EDGE:
          return "edge";
        case ELEMENT_TYPE_MATERIAL:
          return "material";
        default:
          break;
      }
      return "";
    }
    // -------------------------------------------------------------------------
    // findElementTypetWord
    // -------------------------------------------------------------------------
    static ElementType findElementTypetWord(const char *inStr, size_t inLen)
    {
      IBC_GL_FILE_PLY_TRACE();
      const ElementType  wordIndex[] = {
        ELEMENT_TYPE_VERTEX, ELEMENT_TYPE_FACE,
        ELEMENT_TYPE_EDGE, ELEMENT_TYPE_MATERIAL};
      //
      std::string_view  strView(inStr, inLen);
      size_t num = sizeof(wordIndex) / sizeof(PropertyType);
      for (size_t i = 0; i < num; i++)
        if (strView == getElementTypeWord(wordIndex[i]))
          return wordIndex[i];
      //
      return ELEMENT_TYPE_USER;
    }
    // -------------------------------------------------------------------------
    // getPropertyTypeWord
    // -------------------------------------------------------------------------
    static const char *getPropertyTypeWord(PropertyType inType)
    {
      IBC_GL_FILE_PLY_TRACE();
      switch (inType)
      {
        case PROPERTY_X:
          return "x";
        case PROPERTY_Y:
          return "y";
        case PROPERTY_Z:
          return "z";
        case PROPERTY_NX:
          return "nx";
        case PROPERTY_NY:
          return "ny";
        case PROPERTY_NZ:
          return "nz";
        case PROPERTY_RED:
          return "red";
        case PROPERTY_GREEN:
          return "green";
        case PROPERTY_BLUE:
          return "blue";
        case PROPERTY_ALPHA:
          return "alpha";
        case PROPERTY_INTENSITY:
          return "intensity";
        case PROPERTY_CONFIDENCE:
          return "confidence";
        case PROPERTY_VERTEX_METERIAL_INDEX:
          return "material_index";
        case PROPERTY_FACE_VERTEX_INDEX:
          return "vertex_index";
        case PROPERTY_EDGE_VERTEX1:
          return "vertex1";
        case PROPERTY_EDGE_VERTEX2:
          return "vertex2";
        //
        default:
          break;
      }
      return "";
    }
    // -------------------------------------------------------------------------
    // findPropertyTypeWord
    // -------------------------------------------------------------------------
    static PropertyType findPropertyTypeWord(const char *inStr, size_t inLen)
    {
      IBC_GL_FILE_PLY_TRACE();
      const PropertyType  wordIndex[] = {
        PROPERTY_X,   PROPERTY_Y,     PROPERTY_Z,
        PROPERTY_NX,  PROPERTY_NY,    PROPERTY_NZ,
        PROPERTY_RED, PROPERTY_GREEN, PROPERTY_BLUE, PROPERTY_ALPHA,
        PROPERTY_INTENSITY, PROPERTY_CONFIDENCE,
        PROPERTY_VERTEX_METERIAL_INDEX,
        PROPERTY_FACE_VERTEX_INDEX,
        PROPERTY_EDGE_VERTEX1, PROPERTY_EDGE_VERTEX2};
      //
      std::string_view  strView(inStr, inLen);
      size_t num = sizeof(wordIndex) / sizeof(PropertyType);
      for (size_t i = 0; i < num; i++)
      {
        if (strView == getPropertyTypeWord(wordIndex[i]))
          return wordIndex[i];
      }
      //
      return PROPERTY_TYPE_USER;
    }

    // Friend classes ----------------------------------------------------------
    friend class PLYFile;
  };

  // ---------------------------------------------------------------------------
  // PLYFile class
  // ---------------------------------------------------------------------------
  class PLYFile
  {
  public:
    // -------------------------------------------------------------------------
    // readHeader
    // -------------------------------------------------------------------------
    static bool readHeader(const char *inFileName,
                    PLYHeader **outHeader,
                    unsigned char **outDataPtr, size_t *outDataSize,
                    char **outHeaderStrBufPtr = NULL)
    {
      IBC_GL_FILE_PLY_TRACE();
      int fd = ::open(inFileName, O_RDONLY | O_BINARY);
      if (fd == -1)
      {
        IBC_LOG_ERROR("Failed : open()");
        return false;
      }
      FILE  *fp = ::fdopen(fd, "rb");
      if (fp == NULL)
      {
        IBC_LOG_ERROR("Failed : fdopen()");
        return false;
      }
      struct stat stbuf;
      if (::fstat(fd, &stbuf) == -1)
      {
        IBC_LOG_ERROR("Failed : fstat()");
        return false;
      }
      size_t  fileSize = stbuf.st_size;
      unsigned char *buf = new unsigned char[fileSize];
      if (buf == NULL)
      {
        IBC_LOG_ERROR("Failed : buf == NULL");
        ::fclose(fp);
        return false;
      }
      if (::fread(buf, sizeof(unsigned char), fileSize, fp) != fileSize)
      {
        IBC_LOG_ERROR("Failed : fread() returned");
        delete buf;
        ::fclose(fp);
        return false;
      }
      ::fclose(fp);
      size_t  headerSize;
      *outHeader = PLYHeader::create((char *)buf, fileSize, &headerSize);
      if (*outHeader == NULL)
      {
        IBC_LOG_ERROR("Failed : *outHeader == NULL");
        delete buf;
        return false;
      }
      if (outHeaderStrBufPtr != NULL)
      {
        *outHeaderStrBufPtr = new char[headerSize + 1];
        if (*outHeaderStrBufPtr == NULL)
        {
          IBC_LOG_ERROR("Failed : *outHeaderStrBufPtr == NULL");
          delete outHeader;
          delete buf;
          return false;
        }
        ::memcpy(*outHeaderStrBufPtr, buf, headerSize);
        (*outHeaderStrBufPtr)[headerSize] = 0;
      }
      *outDataSize = fileSize - headerSize;
      memmove(buf, &(buf[headerSize]), *outDataSize);
      *outDataPtr = buf;
      return true;
    }
    // -------------------------------------------------------------------------
    // get_glXYZf_RGBAub
    // -------------------------------------------------------------------------
    static bool get_glXYZf_RGBAub(const PLYHeader &inHeader,
                    const void *inDataPtr, size_t inDataSize,
                    ibc::gl::glXYZf_RGBAub **outDataPtr, size_t *outDataNum)
    {
      IBC_GL_FILE_PLY_TRACE();
      DestinationInfo infos[] = {
        { PLYHeader::PROPERTY_X, PLYHeader::DATA_TYPE_FLOAT32,
          offsetof(ibc::gl::glXYZf_RGBAub, x),
          true, 0.0, false, 1.0, 0.0, false, 0, 0 },
        { PLYHeader::PROPERTY_Y, PLYHeader::DATA_TYPE_FLOAT32,
          offsetof(ibc::gl::glXYZf_RGBAub, y),
          true, 0.0, false, 1.0, 0.0, false, 0, 0 },
        { PLYHeader::PROPERTY_Z, PLYHeader::DATA_TYPE_FLOAT32,
          offsetof(ibc::gl::glXYZf_RGBAub, z),
          true, 0.0, false, 1.0, 0.0, false, 0, 0 },
        { PLYHeader::PROPERTY_RED, PLYHeader::DATA_TYPE_UINT8,
          offsetof(ibc::gl::glXYZf_RGBAub, r),
          false, 255.0, false, 1.0, 0.0, false, 0, 0 },
        { PLYHeader::PROPERTY_GREEN, PLYHeader::DATA_TYPE_UINT8,
          offsetof(ibc::gl::glXYZf_RGBAub, g),
          false, 255.0, false, 1.0, 0.0, false, 0, 0 },
        { PLYHeader::PROPERTY_BLUE, PLYHeader::DATA_TYPE_UINT8,
          offsetof(ibc::gl::glXYZf_RGBAub, b),
          false, 255.0, false, 1.0, 0.0, false, 0, 0 },
        { PLYHeader::PROPERTY_TYPE_NOT_SPECIFIED, PLYHeader::DATA_TYPE_UINT8,
          offsetof(ibc::gl::glXYZf_RGBAub, a),
          false, 255.0, false, 1.0, 0.0, false, 0, 0 }};
      //
      return parseData(inHeader, PLYHeader::ELEMENT_TYPE_VERTEX,
                infos, sizeof(infos)/sizeof(DestinationInfo),
                sizeof(ibc::gl::glXYZf_RGBAub),
                inDataPtr, inDataSize,
                (void **)outDataPtr, outDataNum);
    }
    // -------------------------------------------------------------------------
    // calcFitParam_glXYZf_RGBAub
    // -------------------------------------------------------------------------
    static void calcFitParam_glXYZf_RGBAub(
                    const ibc::gl::glXYZf_RGBAub *inDataPtr, size_t inDataNum,
                    GLfloat outParam[4], GLfloat outMinMax[6])
    {
      IBC_GL_FILE_PLY_TRACE();
      GLfloat x_min, y_min, z_min;
      GLfloat x_max, y_max, z_max;
      x_min = inDataPtr->x;
      y_min = inDataPtr->y;
      z_min = inDataPtr->z;
      x_max = inDataPtr->x;
      y_max = inDataPtr->y;
      z_max = inDataPtr->z;
      for (size_t i = 0; i < inDataNum; i++)
      {
        if (inDataPtr[i].x < x_min)
          x_min = inDataPtr[i].x;
        if (inDataPtr[i].y < y_min)
          y_min = inDataPtr[i].y;
        if (inDataPtr[i].z < z_min)
          z_min = inDataPtr[i].z;
        //
        if (inDataPtr[i].x > x_max)
          x_max = inDataPtr[i].x;
        if (inDataPtr[i].y > y_max)
          y_max = inDataPtr[i].y;
        if (inDataPtr[i].z > z_max)
          z_max = inDataPtr[i].z;
      }
      GLfloat size_x = x_max - x_min;
      GLfloat size_y = y_max - y_min;
      GLfloat size_z = z_max - z_min;
      GLfloat size_max = size_x;
      if (size_y > size_max)
        size_max = size_y;
      if (size_z > size_max)
        size_max = size_z;
      //
      outParam[0] = -1.0 * (x_min + size_x / 2.0);
      outParam[1] = -1.0 * (y_min + size_y / 2.0);
      outParam[2] = -1.0 * (z_min + size_z / 2.0);
      if (size_max != 0)
        outParam[3] = 2.0 / size_max;
      else
        outParam[3] = 1.0;
      //
      outMinMax[0] = x_min;
      outMinMax[1] = x_max;
      outMinMax[2] = y_min;
      outMinMax[3] = y_max;
      outMinMax[4] = z_min;
      outMinMax[5] = z_max;
    }

  protected:
    // Static Functions --------------------------------------------------------
    typedef struct
    {
      PLYHeader::PropertyType sourcePropertyType;
      PLYHeader::DataType     destinationDataType;
      size_t  destinationDataOffset;
      //
      bool        mandatory;
      double      defaultValue;
      //
      bool        convert;
      double      gain;
      double      offset;
      //
      bool        clip;
      double      min;
      double      max;
    } DestinationInfo;
    //
    typedef struct
    {
      bool    exist;
      size_t  offset;
      PLYHeader::DataType type;
    } SourceInfo;

    // -------------------------------------------------------------------------
    // parseData
    // -------------------------------------------------------------------------
    static bool parseData(const PLYHeader &inHeader, PLYHeader::ElementType inType,
                    const DestinationInfo *inDstInfoPtr, size_t inDstInfoNum,
                    size_t inDstDataStructSize,
                    const void *inSrcDataPtr, size_t inSrcDataSize,
                    void **outDstDataPtr, size_t *outDstDataNum)
    {
      IBC_GL_FILE_PLY_TRACE();
      size_t  index;
      std::vector<SourceInfo> sourceInfos;
      bool  flipEndian = PLYHeader::doesNeedToFlipEndian(inHeader.mDataFormat);

      if (inHeader.findElementIndex(inType, &index) == false)
      {
          IBC_LOG_ERROR("Can't find the %s element",
                        PLYHeader::getElementTypeWord(inType));
          return false;
      }
      sourceInfos.resize(inDstInfoNum);
      for (size_t i = 0; i < inDstInfoNum; i++)
      {
        sourceInfos[i].exist = inHeader.getPropertyOffset(
                                  index, inDstInfoPtr[i].sourcePropertyType,
                                  &sourceInfos[i].offset, &sourceInfos[i].type);
        if (sourceInfos[i].exist == false && inDstInfoPtr[i].mandatory == true)
        {
            IBC_LOG_ERROR("Can't find the property %s in the %s element",
                PLYHeader::getPropertyTypeWord(inDstInfoPtr[i].sourcePropertyType),
                PLYHeader::getElementTypeWord(inType));
            return false;
        }
      }
      //
      bool  containsList;
      size_t  elementSize = inHeader.getElementSingleDataSize(index, &containsList);
      if (containsList)
      {
        IBC_LOG_ERROR("The specified element contains a property list");
        return false;
      }
      size_t  offset;
      if (inHeader.getElementDataOffset(index, inSrcDataPtr, inSrcDataSize, &offset) == false)
      {
        IBC_LOG_ERROR("getElementDataOffset() returned false");
        return false;
      }
      //
      *outDstDataNum = inHeader.getElements()[index].num;
      *outDstDataPtr = new unsigned char[inDstDataStructSize * (*outDstDataNum)];
      if (*outDstDataPtr == NULL)
      {
        IBC_LOG_ERROR("*outDstDataPtr == NULL");
        return false;
      }
      //
      if (inHeader.mDataFormat != PLYHeader::DATA_FORMAT_ASCII)
      {
        inSrcDataSize -= offset;
        if (inSrcDataSize < elementSize * (*outDstDataNum))
        {
          IBC_LOG_ERROR("The source data buffer is smaller than needed");
          delete ((unsigned char *)*outDstDataPtr);
          *outDstDataPtr = NULL;
          return false;
        }
        const unsigned char *srcDataPtr = ((unsigned char *)inSrcDataPtr) + offset;
        unsigned char *dstDataPtr = (unsigned char *)*outDstDataPtr;
        for (size_t i = 0; i < *outDstDataNum; i++)
        {
          for (size_t j = 0; j < inDstInfoNum; j++)
          {
            double v;
            if (sourceInfos[j].exist == false)
              v = inDstInfoPtr[j].defaultValue;
            else
            {
              v = PLYHeader::getValue(srcDataPtr, sourceInfos[j].offset,
                          sourceInfos[j].type, flipEndian);
              if (inDstInfoPtr[j].convert)
              {
                v = v * inDstInfoPtr[j].gain;
                v = v + inDstInfoPtr[j].offset;
              }
              if (inDstInfoPtr[j].clip)
              {
                if (v < inDstInfoPtr[j].min)
                  v = inDstInfoPtr[j].min;
                if (v > inDstInfoPtr[j].max)
                  v = inDstInfoPtr[j].max;
              }
              PLYHeader::clipToDataTypeRange(inDstInfoPtr[j].destinationDataType, &v);
            }
            PLYHeader::setValue(v, dstDataPtr, inDstInfoPtr[j].destinationDataOffset,
                    inDstInfoPtr[j].destinationDataType, false);
          }
          srcDataPtr += elementSize;
          dstDataPtr += inDstDataStructSize;
        }
        return true;
      }
      //
      const char *linePtr = ((char *)inSrcDataPtr) + offset;
      unsigned char *dstDataPtr = (unsigned char *)*outDstDataPtr;
      inSrcDataSize -= offset;
      std::vector<Common::Range>  words;
      for (size_t i = 0; i < *outDstDataNum; i++)
      {
        size_t  lineLen;
        if (Common::getLineLength(linePtr, inSrcDataSize, &lineLen) == false)
        {
          IBC_LOG_ERROR("Can't find line");
          delete ((unsigned char *)*outDstDataPtr);
          *outDstDataPtr = NULL;
          return false;
        }
        words.clear();
        Common::findWords(linePtr, lineLen, &words);
        for (size_t j = 0; j < inDstInfoNum; j++)
        {
          double v;
          if (sourceInfos[j].exist == false)
            v = inDstInfoPtr[j].defaultValue;
          else
          {
            size_t index = sourceInfos[j].offset;
            if (index > words.size())
            {
              IBC_LOG_ERROR("Can't find element data");
              delete ((unsigned char *)*outDstDataPtr);
              *outDstDataPtr = NULL;
              return false;
            }
            v = PLYHeader::getValueFromStr(
                    &(linePtr[words[index].from]),
                    words[index].len,
                    sourceInfos[j].type);
            if (inDstInfoPtr[j].convert)
            {
              v = v * inDstInfoPtr[j].gain;
              v = v + inDstInfoPtr[j].offset;
            }
            if (inDstInfoPtr[j].clip)
            {
              if (v < inDstInfoPtr[j].min)
                v = inDstInfoPtr[j].min;
              if (v > inDstInfoPtr[j].max)
                v = inDstInfoPtr[j].max;
            }
            PLYHeader::clipToDataTypeRange(inDstInfoPtr[j].destinationDataType, &v);
          }
          PLYHeader::setValue(v, dstDataPtr, inDstInfoPtr[j].destinationDataOffset,
                  inDstInfoPtr[j].destinationDataType, false);
        }
        linePtr += lineLen;
        dstDataPtr += inDstDataStructSize;
      }

      return true;
    }
  };
};};};

#endif  // #ifdef IBC_GL_FILE_PLY_H_
