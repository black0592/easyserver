// component.cpp : �������̨Ӧ�ó������ڵ㡣
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
		printf("���ļ�ʧ��\n");
	}

	for (int y=0; y<Y_COUNT; y++) {
		for (int x=0; x<X_COUNT; x++) {
			char szTemp[255];
#ifdef NETWORK_TYPE
			// ������Ϣ��Կ����
			byte key = rand() % 250 + 1;
			sprintf(szTemp, "0x%02x, ", key);
#else
			// ��Դ��Կ����
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

