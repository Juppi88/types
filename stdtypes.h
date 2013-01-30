/**********************************************************************
 *
 * PROJECT:		Mylly Libraries
 * FILE:		stdtypes.h
 * LICENCE:		See Licence.txt
 * PURPOSE:		Core data types and utility macros.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#pragma once
#ifndef __STDTYPES_H
#define __STDTYPES_H

#include <stddef.h>

#ifdef _WIN32
	#ifndef __MYLLY_USE_GDIPLUS
	#define WIN32_LEAN_AND_MEAN
	#endif
	
	#ifdef __MYLLY_USE_WINSOCK
	#include <WinSock2.h>
	#endif
	
	#include <Windows.h>

	#define MYLLY_API		extern
	#define MYLLY_EXPORT	__declspec(dllexport)
#else
	#define MYLLY_API		extern
	#define MYLLY_EXPORT	__attribute__((dllexport))
#endif


// Basic integer types.
#ifdef _MSC_VER
	typedef signed __int8		int8;
	typedef signed __int16		int16;
	typedef signed __int32		int32;
	typedef signed __int64		int64;
	typedef unsigned __int8		uint8;
	typedef unsigned __int16	uint16;
	typedef unsigned __int32	uint32;
	typedef unsigned __int64	uint64;
#else
	typedef signed char			int8;
	typedef signed short		int16;
	typedef signed int			int32;
	typedef signed long long	int64;
	typedef unsigned char		uint8;
	typedef unsigned short		uint16;
	typedef unsigned int		uint32;
	typedef unsigned long long	uint64;
#endif

typedef uint32		uint;
typedef uint16		pixel_t;

// Basic boolean types.
#ifndef __cplusplus

#ifndef _MSC_VER
	#include <stdbool.h>
#else
	typedef char	bool;
	#define true	(1)
	#define false	(0)
#endif /* _MSC_VER */

#endif /* __cpplusplus */


// Some useful macros
#define BIT_ON(flags,mask)		((flags & mask) != 0)
#define BIT_OFF(flags,mask)		((flags & mask) == 0)

#define UNREFERENCED_PARAM(P)	(void)P


// Make sure C function names aren't mangled if this code is compiled with a C++ compiler.
#ifdef __cplusplus
	#define __BEGIN_DECLS extern "C" {
	#define __END_DECLS }
#else
	#define __BEGIN_DECLS
	#define __END_DECLS
#endif


#ifdef MYLLY_UNICODE
	typedef wchar_t		char_t;
	typedef uint16		uchar_t;
	#define _MTEXT(x)	L ## x
	#define UNICODE
	#define _UNICODE
#else
	typedef char		char_t;
	typedef uint8		uchar_t;
	#define _MTEXT(x)	x
#endif /* MYLLY_UNICODE */


#endif /* __STDTYPES_H */
