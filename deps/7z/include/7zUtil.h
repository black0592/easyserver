/* 7zUtil.h -- Util functions
2010-10-29 : Igor Pavlov : Public domain */

#ifndef __7Z_UTIL_H
#define __7Z_UTIL_H

#include <stdio.h>
#include <string.h>

#include "7z.h"
#include "7zAlloc.h"
#include "7zCrc.h"
#include "7zFile.h"
#include "7zVersion.h"

#ifndef USE_WINDOWS_FILE
/* for mkdir */
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <errno.h>
#endif
#endif

int Buf_EnsureSize(CBuf *dest, size_t size);
Bool Utf16_To_Utf8(Byte *dest, size_t *destLen, const UInt16 *src, size_t srcLen);
SRes Utf16_To_Utf8Buf(CBuf *dest, const UInt16 *src, size_t srcLen);
SRes Utf16_To_Char(CBuf *buf, const UInt16 *s, int fileMode);
WRes MyCreateDir(const UInt16 *name);
WRes OutFile_OpenUtf16(CSzFile *p, const UInt16 *name);
SRes PrintString(const UInt16 *s);
void UInt64ToStr(UInt64 value, char *s);
char *UIntToStr(char *s, unsigned value, int numDigits);
void ConvertFileTimeToString(const CNtfsFileTime *ft, char *s);
void PrintError(char *sz);
void GetAttribString(UInt32 wa, Bool isDir, char *s);
void GetAttribString(UInt32, Bool, char *s);






#endif
