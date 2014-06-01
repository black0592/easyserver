#ifndef __DBHANDLE_H__
#define __DBHANDLE_H__

#include "BaseConfig.h"

#if	ENABLE_DATABASE

#ifdef WIN32
#include <Windows.h>
#include <WinSock2.h>
#endif
#include "BaseTypes.h"
#include <mysql.h>
#include <vector>
#include <string>
#include <map>
#include <sstream> 
using namespace std;



namespace easygame {

	enum DBError {
		DB_ERROR = -1,
		DB_SUCCESS = 0,
		DB_UPDATE_FAIL = 1,
	};

	enum FieldType {
		DB_NULL,			// ��Ч
		DB_BYTE,		// 1�ֽ�
		DB_USHORT,		// 2�ֽ�
		DB_UINT,		// 4�ֽ�
		DB_UINT64,		// 8�ֽ�
		DB_STR,			// �ַ���
		DB_BIN,			// ����������
		DB_TIME,		// ���ݿ�ʱ��(����/������NOW(),��ȡ��UNIX_TIMESTAMP)
	};

	// �ֶνṹ
	struct dbField
	{
		const char*		name;	// �ֶ�����
		FieldType		type;	// ��������
		uint			size;	// ���ݳ���
	};



	class DBHandle
	{
	public:
		DBHandle(void);
		virtual ~DBHandle(void);

		// �ֶ�����index��ӳ��
		typedef map<string,uint>			FieldNameIndexMap;
		typedef FieldNameIndexMap::iterator	FieldNameIndexIter;

		//////////////////////////////////////////////////////////////////////////
		// �������ݿ�����ӿڣ��Ƚ���

		// connect mysql database
		bool connect(const char* host, const char* user, const char* passwd, const char *db, ushort port = 3306, bool bUTF8 = true);

		// close the connection
		void close();

		// �����ַ�����
		bool setCharset(const char* charset);

		// �Ƿ�������
		bool isConnected();

		// �Ƿ�UTF8����
		bool isUTF8Set();

		// ping���ݿ�
		void ping();

		// ����SQL��䣬��ֹע�빥��(���֧��64K��С�����ݣ����ַ������ƣ�Ч�ʲ��Ǻܺ�)
		const char* filter(string& strSrc);
		const char* filter(void* srcBuffer, uint srcLen);
		const char* filter(const char* srcStr);

		// �����Զ��ύģʽ
		bool setAutoCommit(bool isOpen);

		// �����ύ
		bool commit();

		// �ع�
		bool rollback();

		// ��һ�����ݿ�
		bool openDatabase(const char* dbName);

		// ִ��SQL���
		bool query(string& strSQL);

		// ִ��SQL��䣬֧���ַ�����ʽ��
		bool queryF(const char* format, ...);

		// ����ֶε�����
		uint getFieldsNum();

		//  ������ݵ�����
		uint getRowNum();

		// ����ֶε�����
		const char* getFieldName(uint index);

		// ��ȡĳ���ֶ����ݵĳ���
		uint getFieldDataLen(uint index);
		uint getFieldDataLen(const string& fieldName);

		// ��ȡ������
		bool fetchRow();

		// ȡ���ַ����ֶ�ֵ
		string getStrField(uint index);
		string getStrField(const string& fieldName);

		// ȡ�������ֶ�ֵ
		uint getIntField(uint index);
		uint getIntField(const string& fieldName);

		// ȡ������64�ֶ�ֵ
		uint64 getInt64Field(uint index);
		uint64 getInt64Field(const string& fieldName);

		// ȡ�ö������ֶ�ֵ
		// 0 - ���ɹ���������ʾʵ�ʵĴ�С
		uint getBinField(uint index, void* data, uint len);
		uint getBinField(const string& fieldName, void* data, uint len);

		// ȡ�����һ�β����ID��
		uint64 getInsertId();

		//////////////////////////////////////////////////////////////////////////
		// ��չ���ݿ�����ӿڣ��ṹ�ȽϹ̶���

		// ��ѯ����
		uint exeSelect(const char* tableName, const char* where, const char* order, dbField* fields, byte** data, uint limit = 0, uint limitFrom = 0);

		// ��������
		uint exeInsert(const char* tableName, dbField* fields, void* data);

		// ��������
		uint exeUpdate(const char* tableName, const char* where, dbField* fields, void* data);

		// ɾ������
		uint exeDelete(const char* tableName, const char* where);

	private:
		// release Result
		void releaseResult();

		// ���ָ���ֶ������±�
		uint getFieldIndex(const char* fieldName);

		uint fillDataByRow(MYSQL_ROW row, ulong* lengths, const dbField* fields, byte* data);

	private:
		MYSQL m_MySql;
		MYSQL_RES *m_pResult;
		MYSQL_FIELD *m_pFields;
		MYSQL_ROW m_row;
		ulong* mFieldLengths;
		FieldNameIndexMap mFieldNameIndexMap;
		uint m_numfields;
		uint64 m_numrows;
		bool m_bUTF8;

		vector<string> mFilterList;
	};

}	// namespace



#endif	// ENABLE_DATABASE

#endif


