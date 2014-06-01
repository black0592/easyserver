/* 7zTest.cpp - Test application for 7z Decoder
2010-10-28 : Igor Pavlov : Public domain */

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "7zFilePack.h"

int main()
{
	SZFilePack pack("test.7z", "test/test2.txt");


	return 0;
}
