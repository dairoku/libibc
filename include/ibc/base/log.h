// =============================================================================
//  log.h
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
  \file     ibc/base/log.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/05/03
  \brief    Header file for handling a message logging
*/

#ifndef IBC_LOG_H_
#define IBC_LOG_H_

// Includes --------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <ostream>
#include "ibc/base/types.h"

// Macros ----------------------------------------------------------------------
#define IBC_LOG_AT_STRINGIFY(x) #x
#define IBC_LOG_AT_TOSTRING(x)  IBC_LOG_AT_STRINGIFY(x)
#define IBC_LOG_AT              IBC_LOG_AT_TOSTRING(__FILE__) ":" IBC_LOG_AT_TOSTRING(__LINE__)
#define IBC_LOG_LOCATION_MACRO  "(" IBC_LOG_AT ")"
#ifndef _MSC_VER
#define IBC_FUNC_NAME_MACRO  __PRETTY_FUNCTION__
#else
#define IBC_FUNC_NAME_MACRO  __FUNCTION__
#endif

#ifndef IBC_LOG_WR_DISABLE
#define IBC_LOG_WR(log, type, level, locstr, funcstr, outstr, ...) { if(log!=NULL)log->write(type, level, locstr, funcstr, outstr, ##__VA_ARGS__); }
#define IBC_LOG_WR_LIMIT(log, type, level, locstr, funcstr, limit, outstr, ...) { {static int c = 0; if (c < limit) {c++; IBC_LOG_WR(log, type, level, locstr, funcstr, outstr, ##__VA_ARGS__);};}; }
#define IBC_LOG_WR_ONCE(log, type, level, locstr, funcstr, outstr, ...) { IBC_LOG_WR_LIMIT(log, type, level, locstr, funcstr, 1, outstr, ##__VA_ARGS__);  }
#define IBC_LOCAL_LOG_OUT(log, type, level, outstr, ...) { IBC_LOG_WR(log, type, level, IBC_LOG_LOCATION_MACRO, IBC_FUNC_NAME_MACRO, outstr, ##__VA_ARGS__); }
#define IBC_LOCAL_LOG_OUT_ONCE(log, type, level, outstr, ...) { IBC_LOG_WR_ONCE(log, type, level, IBC_LOG_LOCATION_MACRO, IBC_FUNC_NAME_MACRO, outstr, ##__VA_ARGS__); }
#define IBC_LOG_OUT(type, level, outstr, ...) { IBC_LOCAL_LOG_OUT(IBC_GLOBAL_LOG, type, level, outstr, ##__VA_ARGS__); }
#define IBC_LOG_OUT_ONCE(type, level, outstr, ...) { IBC_LOCAL_LOG_OUT_ONCE(IBC_GLOBAL_LOG, type, level, outstr, ##__VA_ARGS__); }
#else
#define IBC_LOG_WR(log, type, level, locstr, outstr, ...)
#define IBC_LOG_WR_LIMIT(log, type, level, limit, locstr, outstr, ...)
#define IBC_LOG_WR_ONCE(log, type, level, locstr, outstr, ...)
#define IBC_LOG_OUT(log, type, level, outstr, ...)
#define IBC_LOG_OUT_ONCE(log, type, level, outstr, ...)
#endif

#ifndef IBC_LOG_INFO_DISABLE
#define IBC_LOG_INFO(outstr, ...) { IBC_LOG_OUT(ibc::Log::INFO_MSG, ibc::Log::NORMAL_LEVEL, outstr, ##__VA_ARGS__); }
#else
#define IBC_LOG_INFO(outstr, ...)
#endif

#ifndef IBC_LOG_WARNING_DISABLE
#define IBC_LOG_WARNING(outstr, ...) { IBC_LOG_OUT(ibc::Log::WARNING_MSG, ibc::Log::NORMAL_LEVEL, outstr, ##__VA_ARGS__); }
#else
#define IBC_LOG_WARNING(outstr, ...)
#endif

#ifndef IBC_LOG_ERROR_DISABLE
#define IBC_LOG_ERROR(outstr, ...) { IBC_LOG_OUT(ibc::Log::ERROR_MSG, ibc::Log::NORMAL_LEVEL, outstr, ##__VA_ARGS__); }
#else
#define IBC_LOG_ERROR(outstr, ...)
#endif

#ifndef IBC_LOG_TRACE_DISABLE
#define IBC_LOG_TRACE(outstr, ...) { IBC_LOG_OUT(ibc::Log::TRACE_MSG, ibc::Log::NORMAL_LEVEL, outstr, ##__VA_ARGS__); }
#define IBC_LOG_TRACE_ONCE(outstr, ...) { IBC_LOG_OUT_ONCE(ibc::Log::TRACE_MSG, ibc::Log::NORMAL_LEVEL, outstr); }
#define IBC_TRACE() { IBC_LOG_TRACE(""); }
#define IBC_TRACE_ONCE() { IBC_LOG_TRACE_ONCE(""); }
#else
#define IBC_LOG_TRACE(outstr, ...)
#define IBC_LOG_TRACE_ONCE(outstr, ...)
#define IBC_TRACE()
#define IBC_TRACE_ONCE()
#endif

#ifndef IBC_LOG_DEBUG_DISABLE
#define IBC_LOG_DEBUG(outstr, ...) { IBC_LOG_OUT(ibc::Log::DEBUG_MSG, ibc::Log::NORMAL_LEVEL, outstr, ##__VA_ARGS__); }
#else
#define IBC_LOG_DEBUG(outstr, ...)
#endif

#define IBC_DEFAULT_LOG   (ibc::ConsoleLog::getInstance())
#define IBC_NULL_LOG      (ibc::NullLog::getInstance())
#define IBC_GLOBAL_LOG    (ibc::GlobalLog::getInstance()->getLog())

// Namespace -------------------------------------------------------------------
namespace ibc
{
  // ---------------------------------------------------------------------------
  // Log interface class
  // ---------------------------------------------------------------------------
  class  Log
  {
  public:
    // Enum --------------------------------------------------------------------
    const static uint32_t INFO_MSG    = 0x00000001;
    const static uint32_t WARNING_MSG = 0x00000002;
    const static uint32_t ERROR_MSG   = 0x00000004;
    const static uint32_t DUMP_MSG    = 0x00000008;
    const static uint32_t TRACE_MSG   = 0x40000000;
    const static uint32_t DEBUG_MSG   = 0x80000000;

    enum  DumpType
    {
      DUMP_TYPE_RAW       = 0x0001,
      DUMP_TYPE_HEX       = 0x0002,
      DUMP_TYPE_SEPARATE  = 0x0004
    };

    // Constatns ---------------------------------------------------------------
    const static uint8_t  GLOBAL_LEVEL  = 0;
    const static uint8_t  NORMAL_LEVEL  = 127;
    const static uint8_t  DETAIL_LEVEL  = 255;

    // Member Functions --------------------------------------------------------
    virtual void  write(uint32_t inType, uint8_t inLenvel,
                        const char *inLocation, const char *inFuncName,
                        const char *inMessage, ...) = 0;
    virtual void  dump(DumpType inDumpType, const char *inDumpName,
                            const char *inLocation, const char *inFuncName,
                            const unsigned char *inData, size_t inDataLen) = 0;
  };

  // ---------------------------------------------------------------------------
  // LogBase class
  // ---------------------------------------------------------------------------
  class  LogBase : public virtual Log
  {
  public:
    // Destructor --------------------------------------------------------------
    // -------------------------------------------------------------------------
    // ~LogBase
    // -------------------------------------------------------------------------
    virtual ~LogBase()
    {
    }
    // Member Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // getLogOutTypeMask
    // -------------------------------------------------------------------------
    uint32_t getLogOutTypeMask()
    {
      return mOutTypeMask;
    }
    // -------------------------------------------------------------------------
    // setLogOutTypeMask
    // -------------------------------------------------------------------------
    void  setLogOutTypeMask(uint32_t inOutTypeMask)
    {
      mOutTypeMask = inOutTypeMask;
    }
    // -------------------------------------------------------------------------
    // getLogOutMaxLevel
    // -------------------------------------------------------------------------
    uint8_t getLogOutMaxLevel()
    {
      return mOutMaxLevel;
    }
    // -------------------------------------------------------------------------
    // setLogOutLevel
    // -------------------------------------------------------------------------
    void  setLogOutMaxLevel(uint8_t inOutMaxLevel)
    {
      mOutMaxLevel = inOutMaxLevel;
    }
    // -------------------------------------------------------------------------
    // isLogOutMessage
    // -------------------------------------------------------------------------
    bool  isLogOutMessage(uint32_t inType, uint8_t inLevel)
    {
      if ((inType & mOutTypeMask) == 0)
        return false;
      if (inLevel > mOutMaxLevel)
        return false;
      return true;
    }
    // Static Member functions -------------------------------------------------
    // -------------------------------------------------------------------------
    // makeTypeStr
    // -------------------------------------------------------------------------
    static void makeTypeStr(uint32_t inType, char *inBuf, const size_t inBufSize)
    {
      switch (inType)
      {
        case INFO_MSG:
          ::snprintf(inBuf, inBufSize, "[ INFO  ] ");
          break;
        case WARNING_MSG:
          ::snprintf(inBuf, inBufSize, "[WARNING] ");
          break;
        case ERROR_MSG:
          ::snprintf(inBuf, inBufSize, "[ ERROR ] ");
          break;
        case DUMP_MSG:
          ::snprintf(inBuf, inBufSize, "[ DUMP  ] ");
          break;
        case TRACE_MSG:
          ::snprintf(inBuf, inBufSize, "[ TRACE ] ");
          break;
        case DEBUG_MSG:
          ::snprintf(inBuf, inBufSize, "[ DEBUG ] ");
          break;
        default:
          ::snprintf(inBuf, inBufSize, "[USR:%03d]", inType);
          break;
      }
    }
    // -------------------------------------------------------------------------
    // makeLevelStr
    // -------------------------------------------------------------------------
    static void makeLevelStr(uint8_t inLevel, char *inBuf, size_t inBufSize)
    {
      ::snprintf(inBuf, inBufSize, "[LEVEL:%03d] ", (int )inLevel);
    }
    // -------------------------------------------------------------------------
    // makeTimeStampStr
    // -------------------------------------------------------------------------
    static void makeTimeStampStr(char *inBuf, size_t inBufSize)
    {
    #ifdef _WIN32  //  Win32 specific ------------------------------------------
      SYSTEMTIME  time;

      ::GetLocalTime(&time);
      ::snprintf(inBuf, inBufSize,
          "%4d/%02d/%02d %02d:%02d:%02d ",
          time.wYear, time.wMonth, time.wDay, time.wHour,
          time.wMinute, time.wSecond);

    #else  //  posix -----------------------------------------------------------
      time_t  t;
      struct tm   *tmPtr;

      time(&t);
      tmPtr = ::localtime(&t);
      ::snprintf(inBuf, inBufSize,
        "%4d/%02d/%02d %02d:%02d:%02d ",
        tmPtr->tm_year, tmPtr->tm_mon, tmPtr->tm_mday, tmPtr->tm_hour,
        tmPtr->tm_min, tmPtr->tm_sec);
    #endif
    }

  protected:
    // Member variables (protected) --------------------------------------------
    uint32_t mOutTypeMask;
    uint8_t mOutMaxLevel;

    // Constructor -------------------------------------------------------------
    // -------------------------------------------------------------------------
    // LogBase
    // -------------------------------------------------------------------------
    LogBase(uint32_t inOutTypeMask, uint8_t inOutMaxLenvel)
    {
      mOutTypeMask  = inOutTypeMask;
      mOutMaxLevel  = inOutMaxLenvel;
    }
  };

  // ---------------------------------------------------------------------------
  // StreamLog class
  // ---------------------------------------------------------------------------
  class  StreamLog : public LogBase
  {
  public:
    const static size_t MSG_BUF_SIZE    = 256;  // TODO : Rethink this

    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // StreamLog
    // -------------------------------------------------------------------------
    StreamLog(std::ostream *inMessageStream, std::ostream *inErrorStream,
              uint32_t inOutTypeMask, uint8_t inOutMaxLenvel)
      : LogBase(inOutTypeMask, inOutMaxLenvel)
    {
      mMessageStream  = inMessageStream;
      mErrorStream    = inErrorStream;
    }
    // -------------------------------------------------------------------------
    // ~StreamLog
    // -------------------------------------------------------------------------
    virtual ~StreamLog()
    {
    }
    // Member Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // write
    // -------------------------------------------------------------------------
    virtual void  write(uint32_t inType, uint8_t inLevel,
                        const char *inLocation, const char *inFuncName,
                        const char *inMessage, ...)
    {
      if (isLogOutMessage(inType, inLevel) == false)
        return;

      std::ostream *stream;
      bool  outMessage = false;
     char  buf[MSG_BUF_SIZE];

      if (inType == ERROR_MSG)
        stream = mErrorStream;
      else
        stream = mMessageStream;

      makeTimeStampStr(buf, MSG_BUF_SIZE);
      *stream << buf;
      makeTypeStr(inType, buf, MSG_BUF_SIZE);
      *stream << buf;
      makeLevelStr(inLevel, buf, MSG_BUF_SIZE);
      *stream << buf;
      // TODO : sanity check the following input string lengthes...
      if (inMessage != NULL)
        if (inMessage[0] != 0)
        {
          va_list arg;
          va_start(arg, inMessage);
          vsnprintf(buf, MSG_BUF_SIZE, inMessage, arg);
          va_end(arg);
          *stream << buf;
          outMessage = true;
        }
      if (inFuncName != NULL)
        if (inFuncName[0] != 0)
        {
          if (outMessage)
            *stream << " ";
          *stream << " @[" << inFuncName << "]";
          outMessage = true;
        }
      if (inLocation != NULL)
        if (inLocation[0] != 0)
        {
          if (outMessage)
            *stream << " ";
          *stream << "<< " << inLocation;
        }
      *stream << std::endl;
    }
    // -------------------------------------------------------------------------
    // dump
    // -------------------------------------------------------------------------
    virtual void  dump(DumpType inDumpType, const char *inDumpName,
                            const char *inLocation, const char *inFuncName,
                            const uint8_t *inData, size_t inDataLen)
    {
      UNUSED(inDumpType);
      UNUSED(inDumpName);
      UNUSED(inLocation);
      UNUSED(inData);
      UNUSED(inDataLen);
      UNUSED(inFuncName);
    }

  protected:
    // Member variables (protected) --------------------------------------------
    std::ostream *mMessageStream;
    std::ostream *mErrorStream;
  };

  // ---------------------------------------------------------------------------
  // ConsoleLog class
  // ---------------------------------------------------------------------------
  class  ConsoleLog :  public StreamLog
  {
  public:
    // Static Member functions -------------------------------------------------
    // -------------------------------------------------------------------------
    // getInstance
    // -------------------------------------------------------------------------
    static ConsoleLog *getInstance()
    {
      static ConsoleLog sLogInstance(INFO_MSG + WARNING_MSG + ERROR_MSG + DUMP_MSG + TRACE_MSG + DEBUG_MSG,
                                    DETAIL_LEVEL);
      return &sLogInstance;
    }

  private:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // ConsoleLog
    // -------------------------------------------------------------------------
    ConsoleLog(uint32_t inOutTypeMask, uint8_t inOutMaxLenvel)
      : StreamLog(&(std::cout), &(std::cerr), inOutTypeMask, inOutMaxLenvel)
    {
    }
    // -------------------------------------------------------------------------
    // ~ConsoleLog
    // -------------------------------------------------------------------------
    virtual ~ConsoleLog()
    {
    }
  };

  // ---------------------------------------------------------------------------
  // NullLog class
  // ---------------------------------------------------------------------------
  class  NullLog :  public Log
  {
  public:
    // -------------------------------------------------------------------------
    // write
    // -------------------------------------------------------------------------
    virtual void  write(uint32_t inType, uint8_t inLenvel,
                        const char *inLocation, const char *inFuncName,
                        const char *inMessage, ...)
    {
      UNUSED(inType);
      UNUSED(inLenvel);
      UNUSED(inLocation);
      UNUSED(inFuncName);
      UNUSED(inMessage);
    }
    // -------------------------------------------------------------------------
    // dump
    // -------------------------------------------------------------------------
    virtual void  dump(DumpType inDumpType, const char *inDumpName,
                            const char *inLocation, const char *inFuncName,
                            const unsigned char *inData, size_t inDataLen)
    {
      UNUSED(inDumpType);
      UNUSED(inDumpName);
      UNUSED(inLocation);
      UNUSED(inData);
      UNUSED(inDataLen);
      UNUSED(inFuncName);
    }

    // Static Member functions -------------------------------------------------
    // -------------------------------------------------------------------------
    // getInstance
    // -------------------------------------------------------------------------
    static NullLog *getInstance()
    {
      static NullLog sLogInstance;
      return &sLogInstance;
    }

  private:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // NullLog
    // -------------------------------------------------------------------------
    NullLog()
    {
    }
    // -------------------------------------------------------------------------
    // ~NullLog
    // -------------------------------------------------------------------------
    virtual ~NullLog()
    {
    }
  };

  // ---------------------------------------------------------------------------
  // GlobalLog class
  // ---------------------------------------------------------------------------
  class  GlobalLog
  {
  public:
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // getLog
    // -------------------------------------------------------------------------
    Log *getLog()
    {
      if (mLog == NULL)
        return IBC_DEFAULT_LOG;
      return mLog;
    }
    // -------------------------------------------------------------------------
    // setLog
    // -------------------------------------------------------------------------
    void  setLog(Log *inLog)
    {
      mLog = inLog;
    }

    // Static Member functions -------------------------------------------------
    // -------------------------------------------------------------------------
    // getInstance
    // -------------------------------------------------------------------------
    static GlobalLog *getInstance()
    {
      static GlobalLog sLogInstance;
      return &sLogInstance;
    }

  protected:
    // Member variables (protected) --------------------------------------------
    Log *mLog;

  private:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // GlobalLog
    // -------------------------------------------------------------------------
    GlobalLog()
    {
      mLog = NULL;
    }
    // -------------------------------------------------------------------------
    // ~GlobalLog
    // -------------------------------------------------------------------------
    virtual ~GlobalLog()
    {
    }
  };

};

#endif  // IBC_LOG_H_
