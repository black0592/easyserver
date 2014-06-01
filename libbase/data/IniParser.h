#ifndef __INIPARSER_H__
#define __INIPARSER_H__


namespace easygame {

	class IniParser
	{
	public:
		IniParser(void);
		virtual ~IniParser(void);

		// ����ini�ļ�
		bool load(const char* filename);

		// ����ini�ļ�
		bool load(const void* data, uint len);

		// ��ȡ��������
		int readInt(const char* app, const char* key, int defaultValue = 0);

		// ��ȡ�ַ�������
		string readString(const char* app, const char* key, const char* defaultValue = "");

		// д����������
		void writeInt(const char* app, const char* key, int value);

		// д���ַ�������
		void writeString(const char* app, const char* key, const char* value);

	private:
		string m_filename;
	};

}	// namespace


#endif