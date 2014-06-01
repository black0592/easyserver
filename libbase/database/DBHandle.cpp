#include "DBHandle.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#if	ENABLE_DATABASE

#define INVALID_FILED_INDEX	(-1)

//#pragma comment(lib,"libmysql.lib")

#pragma warning(disable:4244)

namespace easygame {

	void TRACE(const char* format, ...)
	{
		char buffer[1024] = {0};
		va_list argptr;				
		va_start(argptr, format);	
		vsnprintf( buffer, sizeof(buffer), format, argptr );	
		va_end(argptr);
		printf("%s", buffer);
	}

	/*
	char* _ConvertGBKToUtf8(char* rtnBuffer,const char* strGBK)
	{ 
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0); 
	ushort * wszUtf8 = new ushort[len+1]; 
	memset(wszUtf8, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len); 
	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
	char *szUtf8=new char[len + 1]; 
	memset(szUtf8, 0, len + 1); 
	WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL); 
	strncpy(rtnBuffer, szUtf8, sizeof(rtnBuffer));
	delete[] szUtf8; 
	delete[] wszUtf8;

	return rtnBuffer;
	} 


	char* _ConvertUtf8ToGBK(char* rtnBuffer,const char* strUtf8)
	{ 
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0); 
	ushort * wszGBK = new ushort[len+1]; 
	memset(wszGBK, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len); 
	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	char *szGBK=new char[len + 1]; 
	memset(szGBK, 0, len + 1); 
	WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL,NULL); 
	strncpy(rtnBuffer, szGBK, sizeof(rtnBuffer));
	delete[] szGBK; 
	delete[] wszGBK; 

	return rtnBuffer;
	}
	*/


	DBHandle::DBHandle(void)
	{
		mFilterList.reserve(50);
		m_numfields = 0;
		m_numrows = 0;
		m_pResult = NULL;
		m_pFields = NULL;
		mFieldLengths = NULL;
		m_row = NULL;
		m_bUTF8 = true;

		mysql_init(&m_MySql);
		mysql_options(&m_MySql,MYSQL_OPT_RECONNECT,"odbc");

		//mysql_options(&m_MySql,MYSQL_READ_DEFAULT_GROUP,"odbc");
	}

	DBHandle::~DBHandle(void)
	{
		//close();
	}

	bool DBHandle::setCharset(const char* charset)
	{
		// exp: strCharacterset = "utf8"
		if ( mysql_set_character_set(&m_MySql, charset) != 0 ) {
			TRACE("Failed to connect to database: Error: %s\n", mysql_error(&m_MySql));
			return false;
		}

		//printf("New client character set: %s\n", 
		//	mysql_character_set_name(&m_MySql));

		return true;
	}

	void DBHandle::releaseResult()
	{
		if (m_pResult)
		{
			mysql_free_result(m_pResult);
			m_pResult = NULL;
			m_pFields = NULL;
			m_row = NULL;
			m_numfields = 0;
			m_numrows = 0;
		}
	}

	bool DBHandle::connect(const char* host, const char* user, const char* passwd, const char *db, ushort port, bool bUTF8)
	{
		releaseResult();

		ulong flag = CLIENT_COMPRESS|CLIENT_MULTI_STATEMENTS|CLIENT_INTERACTIVE;
		if ( !mysql_real_connect(&m_MySql, host, user, passwd, db, port, NULL, flag) )
		{
			TRACE("Failed to connect to database: Error: %s\n", mysql_error(&m_MySql));
			return false;
		}

		// 允许自动重连
		mysql_options(&m_MySql,MYSQL_OPT_RECONNECT,"odbc");

		// 设置编码
		m_bUTF8 = bUTF8;
		if (m_bUTF8) {
			setCharset("utf8");
		}

		return true;
	}

	void DBHandle::close()
	{
		releaseResult();
		mysql_close(&m_MySql);
	}

	const char* DBHandle::filter(void* srcBuffer, uint srcLen)
	{
		if (srcLen == 0)
			return "";

		char dest[64 * 1024];
		mysql_real_escape_string(&m_MySql, dest, (char*)srcBuffer, srcLen);
		mFilterList.push_back(dest);
		string* pTemp = &mFilterList[mFilterList.size()-1];
		return pTemp->c_str();
	}

	const char* DBHandle::filter(string& strSrc)
	{
		/*char* buffer = new char[2 * strSrc.length() + 1];
		memset(buffer,0,sizeof(buffer));
		mysql_real_escape_string(&m_MySql,buffer,strSrc.c_str(),(ulong)strSrc.length());
		strDest = buffer;
		delete[] buffer;
		return strDest;*/
		return filter((void*)strSrc.c_str(), (uint)strSrc.length());
	}

	const char* DBHandle::filter(const char* srcStr)
	{
		return filter((void*)srcStr, (uint)strlen(srcStr));	
	}

	bool DBHandle::isConnected()
	{
		if ( mysql_ping(&m_MySql) != 0 )
		{
			TRACE("Failed to IsConnect to database: Error: %s\n", mysql_error(&m_MySql));
			return false;
		}

		return true;
	}

	bool DBHandle::isUTF8Set()
	{
		return m_bUTF8;
	}

	void DBHandle::ping()
	{
		mysql_ping(&m_MySql);
	}

	bool DBHandle::setAutoCommit(bool isOpen)
	{
		if ( mysql_autocommit(&m_MySql, isOpen) != 0 )
		{
			TRACE("Failed to AutoCommit: Error: %s\n", mysql_error(&m_MySql));
			return false;
		}

		return true;
	}


	bool DBHandle::commit()
	{
		if ( mysql_commit(&m_MySql) != 0 )
		{
			TRACE("Failed to Commit: Error: %s\n", mysql_error(&m_MySql));
			return false;
		}

		return true;
	}

	bool DBHandle::rollback()
	{
		if ( mysql_rollback(&m_MySql) != 0 )
		{
			TRACE("Failed to Rollback: Error: %s\n", mysql_error(&m_MySql));
			return false;
		}

		return true;
	}

	bool DBHandle::openDatabase(const char* dbName)
	{
		releaseResult();
		if ( mysql_select_db(&m_MySql, dbName) != 0 )
		{
			TRACE("Failed to open database: Error: %s\n", mysql_error(&m_MySql));
			return false;
		}

		if (m_bUTF8) {
			setCharset("utf8");
		}

		return true;
	}

	bool DBHandle::query(string& strSQL)
	{
		if ( mysql_real_query(&m_MySql, strSQL.c_str(), (ulong)strSQL.length()) != 0 )
		{
			TRACE("%s\n", strSQL.c_str());
			TRACE("Failed to query: Error: %s\n", mysql_error(&m_MySql));
			return false;
		}

		// free Result
		releaseResult();

		m_pResult = mysql_store_result(&m_MySql);
		if (m_pResult)  // there are rows
		{
			m_numfields = mysql_num_fields(m_pResult);
			// retrieve rows, then call mysql_free_result(result)
			m_numrows = mysql_num_rows(m_pResult);
			// fetch fields
			m_pFields = mysql_fetch_fields(m_pResult);

			// 生成字段名和索引号的查询表
			mFieldNameIndexMap.clear();
			const MYSQL_FIELD* pf;
			for (uint index=0; index<m_numfields; index++) {
				pf = mysql_fetch_field(m_pResult);
				if (pf == NULL)
					continue;

				mFieldNameIndexMap[pf->name] = index;
			}
			mysql_field_seek(m_pResult, 0);		// semantics break otherwise!

		}
		else  // mysql_store_result() returned nothing; should it have?
		{
			if(mysql_field_count(&m_MySql) == 0)
			{
				// query does not return data
				// (it was not a SELECT)
				m_numrows = mysql_affected_rows(&m_MySql);
			}
			else // mysql_store_result() should have returned data
			{
				TRACE("Error: %s\n", mysql_error(&m_MySql));
				return false;
			}
		}

		return true;
	}

	// 支持的格式  %d, %u, %l, %llu, '%s', '%b'
	// query with a formated sql string
	bool DBHandle::queryF(const char *format, ...)
	{
#if 0
		char buffer[64*1024] = {0};
		va_list argptr;				
		va_start(argptr, format);	
		vsnprintf( buffer, sizeof(buffer), format, argptr );	
		va_end(argptr);

		// 可以清理了
		mFilterList.clear();

		string strQuery = buffer;
		return query(strQuery);
#else
		int intValue = 0;
		uint uintValue = 0;
		int64 int64Value = 0;
		uint64 uint64Value = 0;
		char hexValue[50] = "";
		const char* strValue = NULL;

		std::ostringstream oss;
		size_t byLen = strlen( format );
		va_list lpParam;
		va_start ( lpParam, format );

		for (size_t i = 0; i < byLen; i++ ) {
			if( format[i] != '%' ) {
				//不是格式符开始
				oss << (char)format[i];
			} else {
				switch (format[i+1])
				{
					//整型
				case 'd':
					intValue = va_arg ( lpParam, int );
					oss << intValue;
					i++;
					break;

					//无符号整型
				case 'u':
					uintValue = va_arg ( lpParam, uint );
					oss << uintValue;
					i++;
					break;

					//长整型
				case 'l':
					{
						bool is64Data = false;
						if (byLen - i - 2 >= 2) {
							// 还剩余2个，就判断是否64位格式化
							char fmt1 = format[i+2];
							char fmt2 = format[i+3];
							if (fmt1 == 'l') {
								if (fmt2 == 'd') {
									int64Value = va_arg ( lpParam, int64 );
									oss << int64Value;
								} else if (fmt2 == 'u') {
									uint64Value = va_arg ( lpParam, uint64 );
									oss << uint64Value;
								}

								i+=3;
								is64Data = true;
							}
						} 

						if (!is64Data) {
							intValue = va_arg ( lpParam, long );
							oss << intValue;
							i++;
						}
					}
					break;

					//字符串
				case 's':
					strValue = va_arg ( lpParam, const char* );
					oss << filter(strValue);	// 字符串需要过滤sql注入
					i++;
					break;

					//格式化后的二进制
				case 'b':
					strValue = va_arg ( lpParam, const char* );
					oss << strValue;	// 已经格式化过，不需要像字符串那样格式化
					i++;
					break;
				default:
					oss << (char)format[i];
					break;
				}
			}
		}

		va_end ( lpParam );

		// 必须写上字符串结束符
		oss << '\0';

		// 可以清理了
		mFilterList.clear();

		std::string strQuery = oss.str();
		return query(strQuery);
#endif
	}

	uint DBHandle::getFieldsNum()
	{
		return m_numfields;
	}

	uint DBHandle::getRowNum()
	{
		return (uint)m_numrows;
	}

	const char* DBHandle::getFieldName(uint index)
	{
		if (m_pFields == NULL)
			return " ";

		if (index > m_numfields-1)
			return " ";

		return m_pFields[index].name;
	}

	uint DBHandle::getFieldDataLen(uint index)
	{
		if (mFieldLengths == NULL)
			return -1;

		if (index > m_numfields-1)
			return -1;

		return mFieldLengths[index];
	}

	uint DBHandle::getFieldDataLen(const string& fieldName)
	{
		return getFieldDataLen( getFieldIndex(fieldName.c_str()) );
	}

	bool DBHandle::fetchRow()
	{
		if (m_pFields == NULL)
			return false;

		m_row = mysql_fetch_row(m_pResult);
		if (m_row == NULL)
		{
			// TRACE("FetchRow Error: %s\n", mysql_error(&m_MySql));
			return false;
		}

		mFieldLengths = mysql_fetch_lengths(m_pResult);

		return true;
	}

	uint DBHandle::getFieldIndex(const char* fieldName)
	{
		uint index = INVALID_FILED_INDEX;
		FieldNameIndexIter it = mFieldNameIndexMap.find(fieldName);
		if (it != mFieldNameIndexMap.end()) {
			index = it->second;
		}

		return index;
	}

	string DBHandle::getStrField(uint index)
	{
		if (m_row == NULL) return " ";
		if (m_pFields == NULL) return " ";
		if (index > m_numfields-1) return " ";

		string strField = (m_row[index] ? m_row[index] : "(NULL)");
		// 	if ( IsUTF8Set() )
		// 	{
		// 		_ConvertUtf8ToGBK(szFieldValue, szFieldValue);
		// 	}
		return strField;
	}

	string DBHandle::getStrField(const string& fieldName)
	{
		return getStrField( getFieldIndex(fieldName.c_str()) );
	}

	uint DBHandle::getIntField(uint index)
	{
		if (m_row == NULL) {
			TRACE("Error: 取数据之前，请调用fetchRow函数\n");
			return -1;
		}

		if (m_pFields == NULL) return -1;
		if (index > m_numfields-1) return -1;

		uint value = 0;
		//value = m_row[index] ? strtoul(m_row[index],NULL,10) : 0;
		if (m_row[index])
			sscanf(m_row[index], "%u", &value);
		else 
			value = 0;

		return value;
	}

	uint DBHandle::getIntField(const string& fieldName)
	{
		return getIntField( getFieldIndex(fieldName.c_str()) );
	}

	uint64 DBHandle::getInt64Field(uint index)
	{
		if (m_row == NULL) return -1;
		if (m_pFields == NULL) return -1;
		if (index > m_numfields-1) return -1;

		uint64 value = 0;
		if (m_row[index])
			sscanf(m_row[index], "%llu", &value);
		else 
			value = 0;

		return value;
	}

	uint64 DBHandle::getInt64Field(const string& fieldName)
	{
		return getInt64Field( getFieldIndex(fieldName.c_str()) );
	}

	uint DBHandle::getBinField(uint index, void* data, uint len)
	{
		if (m_row == NULL) return 0;
		if (m_pFields == NULL) return 0;
		if (index > m_numfields-1) return 0;
		if (data == NULL) return 0;
		if (m_row[index] == NULL) return 0;

		// 防止溢出
		uint maxLen = getFieldDataLen(index);
		if (len > maxLen)
			len = maxLen;

		memcpy(data, m_row[index], len);

		return len;
	}

	uint DBHandle::getBinField(const string& fieldName, void* data, uint len)
	{
		return getBinField( getFieldIndex(fieldName.c_str()), data, len );
	}

	uint64 DBHandle::getInsertId()
	{
		return mysql_insert_id(&m_MySql);
	}

	//////////////////////////////////////////////////////////////////////////
	//  扩展操作接口


	uint DBHandle::fillDataByRow(MYSQL_ROW row, ulong* lengths, const dbField* fields, byte* data)
	{
		uint i = 0;
		uint realSize = 0;
		uint fillSize = 0;
		while(fields->name != NULL) {
			if (strlen(fields->name) > 0 && row[i] != NULL) {
				realSize = fields->size > lengths[i] ? lengths[i] : fields->size;
				switch (fields->type)
				{
				case DB_BYTE:
					{
						sscanf(row[i], "%u", data);
					}
					break;
				case DB_USHORT:
					{
						sscanf(row[i], "%u", data);
					}
					break;
				case DB_UINT:
					{
						sscanf(row[i], "%u", data);
					}
					break;
				case DB_UINT64:
					{
						sscanf(row[i], "%llu", data);
					}
					break;
				case DB_STR:
					{
						memcpy(data,row[i],realSize);
					}
					break;
				case DB_BIN:
					{
						memcpy(data,row[i],realSize);
					}
					break;
				case DB_TIME:
					{
						sscanf(row[i], "%u", data);
					}
					break;
				default:
					break;
				}
			}

			i++;
			data += fields->size;
			fillSize += fields->size;
			fields++;
		}

		return fillSize;
	}

	uint DBHandle::exeSelect(const char* tableName, const char* where, const char* order, dbField* fields, byte** data, uint limit, uint limitFrom)
	{
		ostringstream oss;
		ulong totalSize = 0;

		oss << "SELECT ";

		// 处理字段
		dbField* temp = fields;
		bool isFirst = true;
		while (temp->name != NULL)
		{
			totalSize += temp->size;
			if ( strlen(temp->name) > 0 ) {
				if (isFirst) {
					isFirst = false;
				} else {
					oss << ",";
				}

				if (temp->type == DB_TIME) {
					oss << "UNIX_TIMESTAMP(" << temp->name << ")";
				} else {
					oss << temp->name;
				}
			}
			temp++;
		}

		oss << " FROM " << tableName;

		// 处理where子句
		if (where && strlen(where) > 0) {
			oss << " WHERE " << where;
		}

		// 处理order子句
		if (order && strlen(order) > 0) {
			oss << " ORDER BY " << order;
		}

		// 处理limit子句
		if (limit > 0) {
			oss << " LIMIT " << limitFrom << "," << limit;
		}

		const char* str = oss.str().c_str();

		if ( mysql_real_query(&m_MySql, oss.str().c_str(), (ulong)oss.str().length()) != 0 )
		{
			TRACE("Failed to query: Error: %s\n", mysql_error(&m_MySql));
			return -1;
		}

		MYSQL_RES *result = NULL;
		uint rowNum = 0;
		result = mysql_store_result(&m_MySql);
		if (result == NULL) {
			return -1;
		}

		rowNum = mysql_num_rows(result);
		if (rowNum == 0) {
			mysql_free_result(result);
			return 0;
		}

		*data = new byte[rowNum * totalSize];
		if (*data == NULL) {
			mysql_free_result(result);
			return -1;
		}
		memset(*data,0,rowNum * totalSize);

		MYSQL_ROW row;
		byte* tempData = *data;
		while( (row = mysql_fetch_row(result)) ) {
			ulong* lengths = mysql_fetch_lengths(result);
			uint fillSize = this->fillDataByRow(row,lengths,fields,tempData);
			if (fillSize == 0) {
				delete[] *data;
				*data = NULL;
				mysql_free_result(result);
				return -1;
			}
			tempData += fillSize;
		}

		mysql_free_result(result);
		return rowNum;
	}


	uint DBHandle::exeInsert(const char* tableName,dbField* fields, void* data)
	{
		if (tableName == NULL || fields == NULL || data == NULL)
			return -1;

		if (strlen(tableName) <= 0)
			return -1;

		std::ostringstream oss;
		oss << "INSERT INTO ";
		oss << tableName << " ( ";

		bool isFirst = true;
		dbField* temp = fields;
		while (temp->name && strlen(temp->name) > 0) {
			if (isFirst)
				isFirst = false;
			else
				oss << ",";
			oss << temp->name;
			temp++;
		}

		oss << ") VALUES (";

		isFirst = true;
		temp = fields;
		byte* pData = (byte*)data;
		while (temp->name && strlen(temp->name) > 0) {
			if (isFirst)
				isFirst = false;
			else
				oss << ",";

			switch (temp->type)
			{
			case DB_BYTE:
				{
					uint value = *((byte*)pData);
					oss << value;
				}
				break;
			case DB_USHORT:
				{
					oss << *((ushort*)pData);
				}
				break;
			case DB_UINT:
				{
					oss << *((uint*)pData);
				}
				break;
			case DB_UINT64:
				{
					oss << *((uint64*)pData);
				}
				break;
			case DB_STR:
				{
					int len = (int)strlen((char*)pData);
					char* buff = new char[len*2+1];
					mysql_real_escape_string(&m_MySql,buff,(char*)pData,len);
					oss << "\'" << buff << "\'";
					delete[] buff;
				}
				break;
			case DB_BIN:
				{
					int len = temp->size;
					char* buff = new char[len*2+1];
					mysql_real_escape_string(&m_MySql,buff,(char*)pData,len);
					oss << "\'" << buff << "\'";
					delete[] buff;
				}
				break;
			case DB_TIME:
				{
					oss << "NOW()";
				}
				break;
			default:
				break;
			}

			pData += temp->size;
			temp++;
		}

		oss << ");";

		string strQuery = oss.str();
		if ( !query(strQuery) )
			return -1;

		return mysql_insert_id(&m_MySql);
	}

	uint DBHandle::exeUpdate(const char* tableName, const char* where, dbField* fields, void* data)
	{
		if (tableName == NULL || fields == NULL || data == NULL)
			return -1;

		if (strlen(tableName) <= 0)
			return -1;

		std::ostringstream oss;
		oss << "UPDATE " << tableName << " SET ";

		bool isFirst = true;
		dbField* temp = fields;
		//while (temp->name && strlen(temp->name) > 0) {
		//	if (isFirst)
		//		isFirst = false;
		//	else
		//		oss << ",";
		//	oss << temp->name << "=";
		//	temp++;
		//}

		//isFirst = true;
		//temp = fields;
		byte* pData = (byte*)data;
		while (temp->name && strlen(temp->name) > 0) {
			if (isFirst)
				isFirst = false;
			else
				oss << ",";
			oss << temp->name << "=";
			switch (temp->type)
			{
			case DB_BYTE:
				{
					uint value = *((byte*)pData);
					oss << value;
				}
				break;
			case DB_USHORT:
				{
					oss << *((ushort*)pData);
				}
				break;
			case DB_UINT:
				{
					oss << *((uint*)pData);
				}
				break;
			case DB_UINT64:
				{
					oss << *((uint64*)pData);
				}
				break;
			case DB_STR:
				{
					int len = (int)strlen((char*)pData);
					char* buff = new char[len*2+1];
					mysql_real_escape_string(&m_MySql,buff,(char*)pData,len);
					oss << "\'" << buff << "\'";
					delete[] buff;
				}
				break;
			case DB_BIN:
				{
					int len = temp->size;
					char* buff = new char[len*2+1];
					mysql_real_escape_string(&m_MySql,buff,(char*)pData,len);
					oss << "\'" << buff << "\'";
					delete[] buff;
				}
				break;
			case DB_TIME:
				{
					oss << "NOW()";
				}
				break;
			default:
				break;
			}

			pData += temp->size;
			temp++;
		}

		if (where && strlen(where) > 0)
			oss << " WHERE " << where;

		string strQuery = oss.str();
		if ( !query(strQuery) )
			return -1;

		return mysql_affected_rows(&m_MySql);
	}


	uint DBHandle::exeDelete(const char* tableName, const char* where)
	{
		std::ostringstream oss;
		oss << "DELETE FROM " << tableName;
		if (strlen(where) > 0) {
			oss << " WHERE " << where;
		}

		string strQuery = oss.str();
		if ( !query(strQuery) )
			return -1;

		return mysql_affected_rows(&m_MySql);
	}


}	// namespace


#endif	// ENABLE_DATABASE