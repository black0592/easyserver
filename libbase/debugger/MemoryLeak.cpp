#include "MemoryLeak.h"
#include <string.h>


#if ENABLE_MEMORYCHECK

#pragma warning(disable:4311)

FILE* g_logfile = NULL;

#ifdef WIN32
#include <Windows.h>
#define LEAK_TRACE(msg)	\
	printf("%s",msg);	\
	OutputDebugString(msg);	\
	if (g_logfile) fwrite(msg, strlen(msg), 1, g_logfile);
#else	// linux
#define LEAK_TRACE(msg)	\
	printf("%s",msg);	\
	if (g_logfile) fwrite(msg, strlen(msg), 1, g_logfile);
#endif


typedef uint64	mem_adress;

struct stTrackInfo 
{
	size_t size;
	const char* file;
	uint line;

	stTrackInfo()
	{
		size = 0;
		file = "空字符串";
		line = 0;
	}
};

typedef map<mem_adress, stTrackInfo> TrackInfoMap;
typedef map<mem_adress, stTrackInfo>::iterator TrackInfoMapIter;
TrackInfoMap trackInfoMap;

void addTrack(void* ptr, size_t size, const char* file, uint line)
{
	stTrackInfo trackInfo;
	trackInfo.size = size;
	trackInfo.file = file;
	trackInfo.line = line;
	mem_adress address = (mem_adress)ptr;

	//char szBuf[2048];
	//sprintf(szBuf,"%s(%u), size=%lu, address=0x%08x\n",
	//	trackInfo.file, trackInfo.line, trackInfo.size, address);
	//LEAK_TRACE(szBuf);

	trackInfoMap[address] = trackInfo;
}


void removeTrack(void* ptr)
{
	if (trackInfoMap.empty())
		return;

	mem_adress address = (mem_adress)ptr;
	trackInfoMap.erase(address);
}


void printTrack()
{
	if (trackInfoMap.empty()) {
		LEAK_TRACE("\n------------------------------------------------\r\n");
		LEAK_TRACE("great! memory not leak!\n");
		LEAK_TRACE("------------------------------------------------\r\n\r\n");
		return;
	}

	// 如果有泄露写入文件
	g_logfile = fopen("memory_leak.txt", "a+t");

	LEAK_TRACE("\n------------------------------------------------\r\n");
	LEAK_TRACE("!!!memory leak!!!\r\n");
	char szBuf[2048];
	for (TrackInfoMapIter it=trackInfoMap.begin(); it != trackInfoMap.end(); ++it) {
		mem_adress address = (mem_adress)it->first;
		stTrackInfo& info = it->second;
		sprintf(szBuf,"%s(%u), size=%lu, address=0x%08x\r\n",
			info.file, info.line, info.size, (uint)address);
		LEAK_TRACE(szBuf);
	}
	LEAK_TRACE("------------------------------------------------\r\n\r\n");

	if (g_logfile) {
		fclose(g_logfile);
		g_logfile = NULL;
	}
}

class LeakAutoPrinter
{
public:
	LeakAutoPrinter()
	{

	}

	~LeakAutoPrinter()
	{
		printTrack();
	}
};

LeakAutoPrinter leakPrinter;




#endif