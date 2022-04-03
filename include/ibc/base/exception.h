// =============================================================================
//  exception.h
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
  \file     ibc/base/exception.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/10
  \brief    Header file for handling the exception
*/

#ifndef IBC_EXCEPTION_H_
#define IBC_EXCEPTION_H_

// Includes --------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

// Macros ----------------------------------------------------------------------
#define IBC_EXCEPT_AT_STRINGIFY(x)     #x
#define IBC_EXCEPT_AT_TOSTRING(x)      IBC_EXCEPT_AT_STRINGIFY(x)
#define IBC_EXCEPTION_AT               IBC_EXCEPT_AT_TOSTRING(__FILE__) ":" IBC_EXCEPT_AT_TOSTRING(__LINE__)
#define IBC_EXCEPTION_LOCATION_MACRO  IBC_EXCEPT_AT_TOSTRING(__FUNCTION__) "  (" IBC_EXCEPTION_AT ")"

// Namespace -------------------------------------------------------------------
namespace ibc
{
  // ---------------------------------------------------------------------------
  // Throwable interface class
  // ---------------------------------------------------------------------------
  class  Throwable
  {
  public:
    //  member functions
    virtual int        getExceptionCode()   const = 0;
    virtual int        getOSErrorCode()     const = 0;
    virtual const char *getExceptionName()  const = 0;
    virtual const char *getDescription()    const = 0;
    virtual const char *getLocation()       const = 0;
  };

  // ---------------------------------------------------------------------------
  // Exception class
  // ---------------------------------------------------------------------------
  class Exception : public Throwable
  {
  public:
    // Destructor --------------------------------------------------------------
    virtual ~Exception() {};

    // Constants ---------------------------------------------------------------
    static const int  MESSAGE_BUFFER_SIZE   = 128;
    enum ExceptionCode
    {
      OS_ERROR          = 1,
      MEMORY_ERROR,
      PARAM_ERROR,
      INVALID_OPERATION_ERROR,
      NULL_POINTER_ACCESS_ERROR,
      INTERNAL_ERROR,

      SUB_CLASS_ERROR   = 50000
    };

    // Member Functions --------------------------------------------------------
    virtual int         getExceptionCode() const { return mExceptionCode; };
    virtual int         getOSErrorCode() const { return mOSErrorCode; };
    virtual const char  *getExceptionName() const { return mExceptionName; };
    virtual const char  *getDescription() const { return mDescription; };
    virtual const char  *getLocation() const { return mLocation; };
    virtual void  dump()
    {
      printf("Catch %s\n",
          getExceptionName());
      printf("Desc=%s\n",
          getDescription());
      printf("ExCode=%d (OSError=%d) in\n",
          getExceptionCode(), getOSErrorCode());
      printf("%s\n",
          getLocation());
    };

  protected:
    // Constructors ------------------------------------------------------------
    Exception(const char *inExceptionName, int inExceptionCode, const char *inDescription,
              const char *inLocation, int inOSErrorCode = 0)
    {
      set(inExceptionName, inExceptionCode, inDescription,
          inLocation, inOSErrorCode);
    }
    Exception(const Throwable &inEx)
    {
      set(inEx.getExceptionName(), inEx.getExceptionCode(), inEx.getDescription(),
          inEx.getLocation(), inEx.getOSErrorCode());
    }
    Exception(const Exception &inEx)
    {
      set(inEx.getExceptionName(), inEx.getExceptionCode(), inEx.getDescription(),
          inEx.getLocation(), inEx.getOSErrorCode());
    }

    // Member Functions --------------------------------------------------------
    virtual void  set(const char *inExceptionName, int inExceptionCode, const char *inDescription,
                      const char *inLocation, int inOSErrorCode)
    {
      setExceptionName(inExceptionName);
      setExceptionCode(inExceptionCode);
      setDescription(inDescription);
      setLocation(inLocation);
      setOSErrorCode(inOSErrorCode);
    };

    virtual void  setExceptionCode(int inExceptionCode) { mExceptionCode = inExceptionCode; };
    virtual void  setOSErrorCode(int inOSErrorCode) { mOSErrorCode = inOSErrorCode; };
    virtual void  setExceptionName(const char *inExceptionName)
    {
      setMessageToBuffer(inExceptionName, mExceptionName);
    };
    virtual void  setDescription(const char *inDescription)
    {
      setMessageToBuffer(inDescription, mDescription);
    };
    virtual void  setLocation(const char *inLocation)
    {
      setMessageToBuffer(inLocation, mLocation);
    };
    virtual void  setMessageToBuffer(const char *inMessage, char *inBuf)
    {
      size_t  len;

      len = ::strlen(inMessage);
      if (len >= MESSAGE_BUFFER_SIZE)
        len = MESSAGE_BUFFER_SIZE - 1;
      ::memcpy(inBuf, inMessage, len);
      inBuf[len] = 0;
    };

  private:
    // Member Variables --------------------------------------------------------
    int   mExceptionCode, mOSErrorCode;
    char  mExceptionName[MESSAGE_BUFFER_SIZE];
    char  mDescription[MESSAGE_BUFFER_SIZE];
    char  mLocation[MESSAGE_BUFFER_SIZE];
  };
};

#endif  // IBC_EXCEPTION_H_
