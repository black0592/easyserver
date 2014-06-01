#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

namespace easygame {

template <class T>
struct stDataCallBack
{
	virtual bool exec(T* obj) { return true; }
};

// SData 必须包含id字段

template<typename SData>
class DataManager
{
public:
	DataManager(void){}
	virtual ~DataManager(void){m_dataMap.clear();}

	// 读取数据表
	bool load(void* pData, uint dataLen)
	{
		if (pData == NULL || dataLen == 0)
			return false;

		byte* tmp = (byte*)pData;

		uint count = 0;
		memcpy(&count, tmp, sizeof(count));
		tmp += sizeof(count);

		if (count > 0) 
			m_dataMap.clear();

		SData* datas = new SData[count];
		memcpy(datas, tmp, sizeof(SData)*count);

		for (uint i=0; i<count; i++) {
			if (m_dataMap.find(datas[i].id) != m_dataMap.end()) {
				printf("【error】表格出现重复的主键值\n");
			}
			m_dataMap[datas[i].id] = datas[i];
		}

		delete[] datas;

		return true;
	}

	// 取得指定ID的数据
	SData* getData(uint id)
	{
		typename map<uint,SData>::iterator itFind = m_dataMap.find(id);
		if (itFind == m_dataMap.end())
			return NULL;
        return &itFind->second;
	}

	// 取得指定ID的数据
	void getAllData(vector<SData>& vecList)
	{
		if (size() == 0)
			return;

		for (typename map<uint,SData>::iterator it = m_dataMap.begin(); it!=m_dataMap.end(); it++) {
			vecList.push_back(it->second);
		}
	}

	// 取得指定ID的数据指针
	void getAllDataPtr(vector<SData*>& vecList)
	{
		if (size() == 0)
			return;

		for (typename map<uint,SData>::iterator it = m_dataMap.begin(); it!=m_dataMap.end(); it++) {
			vecList.push_back(&it->second);
		}
	}

	uint size()
	{
		return (uint)m_dataMap.size();
	}

	// 执行回调函数
	// 返回 true 完整执行， false 执行被打断
	bool exec(stDataCallBack<SData> &cb)
	{
		if (size() == 0)
			return true;

		for (typename map<uint,SData>::iterator it = m_dataMap.begin(); it!=m_dataMap.end(); it++) {
			if (!cb.exec((SData*)&it->second))
				return false;
		}

		return true;
	}

private:
	map<uint,SData> m_dataMap;
};


}	// namespace


#endif





