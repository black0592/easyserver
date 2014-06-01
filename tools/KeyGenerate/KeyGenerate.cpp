// component.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <fstream>
using namespace std;

//#define NETWORK_TYPE
#define X_COUNT		20
#define Y_COUNT		25

typedef unsigned char byte;


int _tmain(int argc, _TCHAR* argv[])
{
	srand( (unsigned int)time(NULL) );

	ofstream ofOut;
	ofOut.open("./encrypt_key.txt");
	if (!ofOut.is_open()) {
		printf("打开文件失败\n");
	}

	for (int y=0; y<Y_COUNT; y++) {
		for (int x=0; x<X_COUNT; x++) {
			char szTemp[255];
#ifdef NETWORK_TYPE
			// 网络消息密钥生成
			byte key = rand() % 250 + 1;
			sprintf(szTemp, "0x%02x, ", key);
#else
			// 资源密钥生成
			byte key = 0;
			while (true) {
				key = 33 + rand() % 90;
				if (key == '"')
					continue;
				if (key == '\\')
					continue;
				if (key == '/')
					continue;
				if (key == '<')
					continue;
				if (key == '>')
					continue;
				if (key == '&')
					continue;
				if (key == '%')
					continue;

				break;
			}
			sprintf(szTemp, "%c", key);
#endif
			ofOut << szTemp;
		}

#ifdef NETWORK_TYPE
		ofOut << "\n";
#endif
	}

	ofOut.flush() ;  
	int i = 0 ;  

	return 0;
}

