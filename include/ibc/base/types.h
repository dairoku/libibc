// =============================================================================
//  types.h
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
  \file     ibc/base/types.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/03/11
  \brief    Header file for basic types
*/

#ifndef IBC_TYPES_H_
#define IBC_TYPES_H_

// Macros ----------------------------------------------------------------------
#define IBC_MACRO_TOSTRING_i(x)  #x
#define IBC_MACRO_JOIN_i(x, y)   x##y
//
#define IBC_MACRO_TOSTRING(x)           IBC_MACRO_TOSTRING_i(x)
#define IBC_MACRO_JOIN(x, y)            IBC_MACRO_JOIN_i(x,y)

#define UNUSED(x)   (void)(x)

#endif  // IBC_TYPES_H_
