#pragma once

// �༭������
class EditorConfig : public Vek::Singleton<EditorConfig>
{
public:
	EditorConfig(void);
	virtual ~EditorConfig(void);

	// ��ȡ�����ļ�
	bool loadConfig();

	const char* getClientPath();
	const char* makeClientResPath(const char* subPath);

	const char* getServerPath();
	const char* makeServerResPath(const char* subPath);

private:
	string mClientPath;
	string mServerPath;
};




