#include "StdAfx.h"
#include "EditorConfig.h"


EditorConfig::EditorConfig(void)
{
	mClientPath = ".\\Resources\\";
	mServerPath = ".\\datas\\";
}

EditorConfig::~EditorConfig(void)
{
}

const char* EditorConfig::getClientPath()
{
	return mClientPath.c_str();
}

const char* EditorConfig::getServerPath()
{
	return mServerPath.c_str();
}

const char* EditorConfig::makeClientResPath(const char* subPath)
{
	static string tempClientPath = "";
	tempClientPath = mClientPath;
	tempClientPath += subPath;
	return tempClientPath.c_str();
}

const char* EditorConfig::makeServerResPath(const char* subPath)
{
	static string tempServerPath = "";
	tempServerPath = mServerPath;
	tempServerPath += subPath;
	return tempServerPath.c_str();
}

// ∂¡»°≈‰÷√Œƒº˛
bool EditorConfig::loadConfig()
{
	int lastChar = 0;
	xml_document doc;  
	File file;
	if (!file.open("./editorconfig.xml"))
		return false;

	doc.load_buffer(file.getBufferPtr(), file.getLength());

	pugi::xml_node node1 = doc.child("config").child("clientpath");
	mClientPath = node1.text().as_string();
	lastChar = mClientPath.at(mClientPath.length()-1);
	if (lastChar != '\\') {
		mClientPath += "\\";
	}

	pugi::xml_node node2 = doc.child("config").child("serverpath");
	mServerPath = node2.text().as_string();
	lastChar = mServerPath.at(mServerPath.length()-1);
	if (lastChar != '\\') {
		mServerPath += "\\";
	}

	return true;
}




