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
  \brief    Header file for the PLY file

  This file defines the class for handling the PLY polygon file format.
*/

#ifndef IBC_GL_FILE_PLY_H_
#define IBC_GL_FILE_PLY_H_

// Includes --------------------------------------------------------------------
#include <vector>
#include <string>
#include <string_view>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ibc/base/log.h"
#include "ibc/gl/data.h"
#include "ibc/gl/file/common.h"

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
      DATA_FORMAT__ANY                  = 32767
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
      mDataFormat = DATA_FORMAT_NOT_SPECIFIED;
    }
    // -------------------------------------------------------------------------
    // ~PLYHeader
    // -------------------------------------------------------------------------
    virtual ~PLYHeader()
    {
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // clear
    // -------------------------------------------------------------------------
    void clear()
    {
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
      mDataFormat = inFormat;
      mFormatVersionStr.assign(inVersionStr, inVersionStrLen);
    }
    // -------------------------------------------------------------------------
    // getFormat
    // -------------------------------------------------------------------------
    DataFormat  getFormat()
    {
      return mDataFormat;
    }
    // -------------------------------------------------------------------------
    // getFormatVersionStr
    // -------------------------------------------------------------------------
    const std::string &getFormatVersionStr()
    {
      return mFormatVersionStr;
    }
    // -------------------------------------------------------------------------
    // addComment
    // -------------------------------------------------------------------------
    void  addComment(const char *inStr, size_t inStrLen)
    {
      if (inStr != NULL && inStrLen != 0)
        mCommentStr.append(inStr, inStrLen);
      mCommentStr.append(1, Common::CHAR_EOL_CODE);
    }
    // -------------------------------------------------------------------------
    // getCommentStr
    // -------------------------------------------------------------------------
    const std::string &getCommentStr()
    {
      return mCommentStr;
    }
    // -------------------------------------------------------------------------
    // addObjInfo
    // -------------------------------------------------------------------------
    void  addObjInfo(const char *inStr, size_t inStrLen)
    {
      if (inStr != NULL && inStrLen != 0)
        mObjInfoStr.append(inStr, inStrLen);
      mObjInfoStr.append(1, Common::CHAR_EOL_CODE);
    }
    // -------------------------------------------------------------------------
    // getObjInfoStr
    // -------------------------------------------------------------------------
    const std::string &getObjInfoStr()
    {
      return mObjInfoStr;
    }
    // -------------------------------------------------------------------------
    // addElement
    // -------------------------------------------------------------------------
    size_t  addElement(const char *inNameStr, size_t inNameStrLen, size_t inElementNum)
    {
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
    const std::vector<Element> &getElements()
    {
      return mElements;
    }
    // -------------------------------------------------------------------------
    // findElementIndex
    // -------------------------------------------------------------------------
    bool findElementIndex(ElementType inType, size_t *outIndex)
    {
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
    const std::vector<Property> &getProperties()
    {
      return mProperties;
    }
    // -------------------------------------------------------------------------
    // getElementProperties
    // -------------------------------------------------------------------------
    size_t getElementProperties(size_t inElementIndex, std::vector<Property> *outProperties)
    {
      for (size_t i = 0; i < mProperties.size(); i++)
        if (mProperties[i].elementIndex == inElementIndex)
          outProperties->push_back(mProperties[i]);
      return outProperties->size();
    }
    // -------------------------------------------------------------------------
    // getElementSize
    // -------------------------------------------------------------------------
    size_t getElementSize(size_t inElementIndex, bool *outContainsList)
    {
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
    // getElementDataPtr
    // -------------------------------------------------------------------------
    const void *getElementDataPtr(size_t inElementIndex, const void *inDataPtr, size_t inDataSize)
    {
      UNUSED(inDataSize); // TODO : Remove this
      //
      if (inElementIndex >= mElements.size())
        return NULL;
      if (mDataFormat == DATA_FORMAT_ASCII)
        return NULL; // TODO : Need to implement this path
      size_t  offset = 0;
      bool  containsList = false;
      for (size_t i = 0; i < inElementIndex; i++)
      {
        size_t elementSize = getElementSize(i, &containsList);
        if (containsList) // TODO : Need to implement this path
          return NULL;
        offset += elementSize * mElements[i].num;
      }
      const unsigned char *elementDataPtr = (const unsigned char *)inDataPtr;
      // TODO : Need to check memory size
      return (void *)(elementDataPtr + offset);
    }
    // -------------------------------------------------------------------------
    // getPropertyOffset
    // -------------------------------------------------------------------------
    bool  getPropertyOffset(size_t inElementIndex, PropertyType inType, size_t *outOffset)
    {
      if (mDataFormat == DATA_FORMAT_ASCII)
        return false;
      *outOffset = 0;
      for (size_t i = 0; i < mProperties.size(); i++)
        if (mProperties[i].elementIndex == inElementIndex)
        {
          if (mProperties[i].isList)  // We can't calculate the offset here in this case
            return false;
          if (mProperties[i].type == inType)
            return true;
          (*outOffset) += sizeofDataType(mProperties[i].dataType);
        }
      return false;
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // create
    // -------------------------------------------------------------------------
    static PLYHeader *create(const char *inHeaderStr, size_t inLen,
                             size_t *outHeaderSize)
    {
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
      const LineType  wordIndex[] = {
        LINE_TYPE_START, LINE_TYPE_END, LINE_TYPE_FORMAT,
        LINE_TYPE_COMMENT, LINE_TYPE_OBJ_INFO, LINE_TYPE_ELEMENT,
        LINE_TYPE_PROPERTY, LINE_TYPE_PROPERTY_LIST};
      //
      std::string_view  strView(inStr, inLen);
      for (size_t i = 0; i < sizeof(wordIndex); i++)
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
      const DataFormat  wordIndex[] = {
        DATA_FORMAT_ASCII,
        DATA_FORMAT_BINARY_LITTLE_ENDIAN,
        DATA_FORMAT_BINARY_BIG_ENDIAN};
      //
      std::string_view  strView(inStr, inLen);
      for (size_t i = 0; i < sizeof(wordIndex); i++)
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
      for (size_t i = 0; i < sizeof(wordIndex); i++)
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
    static size_t sizeofDataType(DataType inFormat)
    {
      switch (inFormat)
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
    // getElementTypeWord
    // -------------------------------------------------------------------------
    static const char *getElementTypeWord(ElementType inType)
    {
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
      const ElementType  wordIndex[] = {
        ELEMENT_TYPE_VERTEX, ELEMENT_TYPE_FACE,
        ELEMENT_TYPE_EDGE, ELEMENT_TYPE_MATERIAL};
      //
      std::string_view  strView(inStr, inLen);
      for (size_t i = 0; i < sizeof(wordIndex); i++)
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
      for (size_t i = 0; i < sizeof(wordIndex); i++)
        if (strView == getPropertyTypeWord(wordIndex[i]))
          return wordIndex[i];
      //
      return PROPERTY_TYPE_USER;
    }
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
                    void **outDataPtr, size_t *outDataSize,
                    char **outHeaderStrBufPtr = NULL)
    {
      int fd = ::open(inFileName, O_RDONLY);
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
        IBC_LOG_ERROR("Failed : fread()");
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
    // get_glXYZf_RGBub
    // -------------------------------------------------------------------------
    static bool get_glXYZf_RGBub(PLYHeader &inHeader,
                    const void *inDataPtr, size_t inDataSize,
                    ibc::gl::glXYZf_RGBAub **outDataPtr, size_t *outDataNum,
                    bool  inReuireRGB = false)
    {
      UNUSED(inReuireRGB);  // TODO : Clean up
      bool  hasRGB = false;
      UNUSED(hasRGB);
      size_t  index;
      size_t  x_offset, y_offset, z_offset;
      size_t  r_offset, g_offset, b_offset;
      
      if (inHeader.findElementIndex(PLYHeader::ELEMENT_TYPE_VERTEX, &index) == false)
      {
          IBC_LOG_ERROR("Can't find the vertex element");
          return false;
      }
      if (inHeader.getPropertyOffset(index, PLYHeader::PROPERTY_X, &x_offset) == false)
      {
          IBC_LOG_ERROR("Can't find the property x in the vertex element");
          return false;
      }
      if (inHeader.getPropertyOffset(index, PLYHeader::PROPERTY_Y, &y_offset) == false)
      {
          IBC_LOG_ERROR("Can't find the property y in the vertex element");
          return false;
      }
      if (inHeader.getPropertyOffset(index, PLYHeader::PROPERTY_Z, &z_offset) == false)
      {
          IBC_LOG_ERROR("Can't find the property z in the vertex element");
          return false;
      }
      //
      if (inHeader.getPropertyOffset(index, PLYHeader::PROPERTY_RED, &r_offset) == false)
        hasRGB = false;
      if (inHeader.getPropertyOffset(index, PLYHeader::PROPERTY_GREEN, &g_offset) == false)
        hasRGB = false;
      if (inHeader.getPropertyOffset(index, PLYHeader::PROPERTY_BLUE, &b_offset) == false)
        hasRGB = false;
      //
      unsigned char *srcDataPtr = (unsigned char *)inHeader.getElementDataPtr(index, inDataPtr, inDataSize);
      if (srcDataPtr == NULL)
      {
        IBC_LOG_ERROR("getElementDataPtr() returned NULL");
        return false;
      }
      bool  containsList;
      size_t  elementSize = inHeader.getElementSize(index, &containsList);
      if (containsList)
      {
        IBC_LOG_ERROR("Vertex element contains a property list");
        return false;
      }
      *outDataNum = inHeader.getElements()[index].num;
      *outDataPtr = new ibc::gl::glXYZf_RGBAub[*outDataNum];
      if (*outDataPtr == NULL)
      {
        IBC_LOG_ERROR("*outDataPtr == NULL");
        return false;
      }
      ibc::gl::glXYZf_RGBAub *dstDataPtr = *outDataPtr;
      for (size_t i = 0; i < *outDataNum ;i++)
      {
        dstDataPtr->x = *((float *)&(srcDataPtr[x_offset]));
        dstDataPtr->y = *((float *)&(srcDataPtr[y_offset]));
        dstDataPtr->z = *((float *)&(srcDataPtr[z_offset]));
        //
        // if (hasRGB)
        dstDataPtr->r = *((unsigned char *)&(srcDataPtr[r_offset]));
        dstDataPtr->g = *((unsigned char *)&(srcDataPtr[g_offset]));
        dstDataPtr->b = *((unsigned char *)&(srcDataPtr[g_offset]));
        dstDataPtr++;
        //
        srcDataPtr += elementSize;
      }
      return true;
    }

  protected:
    // Static Functions --------------------------------------------------------
    typedef struct
    {
      PLYHeader::Property   sourceProperty;
      PLYHeader::DataType   targetDataType;
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
  };
};};};

#endif  // #ifdef IBC_GL_FILE_PLY_H_
