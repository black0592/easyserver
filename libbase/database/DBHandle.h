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
		DB_NULL,			// 无效
		DB_BYTE,		// 1字节
		DB_USHORT,		// 2字节
		DB_UINT,		// 4字节
		DB_UINT64,		// 8字节
		DB_STR,			// 字符串
		DB_BIN,			// 二进制数据
		DB_TIME,		// 数据库时间(更新/插入用NOW(),读取用UNIX_TIMESTAMP)
	};

	// 字段结构
	struct dbField
	{
		const char*		name;	// 字段名称
		FieldType		type;	// 数据类型
		uint			size;	// 数据长度
	};



	class DBHandle
	{
	public:
		DBHandle(void);
		virtual ~DBHandle(void);

		// 字段名到index的映射
		typedef map<string,uint>			FieldNameIndexMap;
		typedef FieldNameIndexMap::iterator	FieldNameIndexIter;

		//////////////////////////////////////////////////////////////////////////
		// 基础数据库操作接口（比较灵活）

		// connect mysql database
		bool connect(const char* host, const char* user, const char* passwd, const char *db, ushort port = 3306, bool bUTF8 = true);

		// close the connection
		void close();

		// 设置字符编码
		bool setCharset(const char* charset);

		// 是否连接中
		bool isConnected();

		// 是否UTF8编码
		bool isUTF8Set();

		// ping数据库
		void ping();

		// 过滤SQL语句，防止注入攻击(最大支持64K大小的数据，有字符串复制，效率不是很好)
		const char* filter(string& strSrc);
		const char* filter(void* srcBuffer, uint srcLen);
		const char* filter(const char* srcStr);

		// 设置自动提交模式
		bool setAutoCommit(bool isOpen);

		// 立即提交
		bool commit();

		// 回滚
		bool rollback();

		// 打开一个数据库
		bool openDatabase(const char* dbName);

		// 执行SQL语句
		bool query(string& strSQL);

		// 执行SQL语句，支持字符串格式化
		bool queryF(const char* format, ...);

		// 获得字段的数量
		uint getFieldsNum();

		//  获得数据的行数
		uint getRowNum();

		// 获得字段的名称
		const char* getFieldName(uint index);

		// 获取某个字段数据的长度
		uint getFieldDataLen(uint index);
		uint getFieldDataLen(const string& fieldName);

		// 获取行数据
		bool fetchRow();

		// 取得字符串字段值
		string getStrField(uint index);
		string getStrField(const string& fieldName);

		// 取得整形字段值
		uint getIntField(uint index);
		uint getIntField(const string& fieldName);

		// 取得整形64字段值
		uint64 getInt64Field(uint index);
		uint64 getInt64Field(const string& fieldName);

		// 取得二进制字段值
		// 0 - 不成功，其他表示实际的大小
		uint getBinField(uint index, void* data, uint len);
		uint getBinField(const string& fieldName, void* data, uint len);

		// 取个最近一次插入的ID号
		uint64 getInsertId();

		//////////////////////////////////////////////////////////////////////////
		// 扩展数据库操作接口（结构比较固定）

		// 查询数据
		uint exeSelect(const char* tableName, const char* where, const char* order, dbField* fields, byte** data, uint limit = 0, uint limitFrom = 0);

		// 插入数据
		uint exeInsert(const char* tableName, dbField* fields, void* data);

		// 更新数据
		uint exeUpdate(const char* tableName, const char* where, dbField* fields, void* data);

		// 删除数据
		uint exeDelete(const char* tableName, const char* where);

	private:
		// release Result
		void releaseResult();

		// 获得指定字段名的下标
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


