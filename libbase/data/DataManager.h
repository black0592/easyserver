#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

namespace easygame {

template <class T>
struct stDataCallBack
{
	virtual bool exec(T* obj) { return true; }
};

// SData �������id�ֶ�

template<typename SData>
class DataManager
{
public:
	DataManager(void){}
	virtual ~DataManager(void){m_dataMap.clear();}

	// ��ȡ���ݱ�
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
				printf("��error���������ظ�������ֵ\n");
			}
			m_dataMap[datas[i].id] = datas[i];
		}

		delete[] datas;

		return true;
	}

	// ȡ��ָ��ID������
	SData* getData(uint id)
	{
		typename map<uint,SData>::iterator itFind = m_dataMap.find(id);
		if (itFind == m_dataMap.end())
			return NULL;
        return &itFind->second;
	}

	// ȡ��ָ��ID������
	void getAllData(vector<SData>& vecList)
	{
		if (size() == 0)
			return;

		for (typename map<uint,SData>::iterator it = m_dataMap.begin(); it!=m_dataMap.end(); it++) {
			vecList.push_back(it->second);
		}
	}

	// ȡ��ָ��ID������ָ��
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

	// ִ�лص�����
	// ���� true ����ִ�У� false ִ�б����
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





