#ifndef __INIPARSER_H__
#define __INIPARSER_H__


namespace easygame {

	class IniParser
	{
	public:
		IniParser(void);
		virtual ~IniParser(void);

		// 加载ini文件
		bool load(const char* filename);

		// 加载ini文件
		bool load(const void* data, uint len);

		// 读取整型数据
		int readInt(const char* app, const char* key, int defaultValue = 0);

		// 读取字符串数据
		string readString(const char* app, const char* key, const char* defaultValue = "");

		// 写入整形数据
		void writeInt(const char* app, const char* key, int value);

		// 写入字符串数据
		void writeString(const char* app, const char* key, const char* value);

	private:
		string m_filename;
	};

}	// namespace


#endif