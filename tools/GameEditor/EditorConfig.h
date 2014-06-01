#pragma once

// ±à¼­Æ÷ÅäÖÃ
class EditorConfig : public Vek::Singleton<EditorConfig>
{
public:
	EditorConfig(void);
	virtual ~EditorConfig(void);

	// ¶ÁÈ¡ÅäÖÃÎÄ¼ş
	bool loadConfig();

	const char* getClientPath();
	const char* makeClientResPath(const char* subPath);

	const char* getServerPath();
	const char* makeServerResPath(const char* subPath);

private:
	string mClientPath;
	string mServerPath;
};




