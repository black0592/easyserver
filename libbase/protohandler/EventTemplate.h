#ifndef __EVENT_TEMPLATE_H__
#define __EVENT_TEMPLATE_H__


// �¼�����
class EventArgs
{
public:
	EventArgs() {}
	virtual ~EventArgs() {}
};

// �������Ļ���
class FunctorBase
{
public:
	virtual ~FunctorBase() {};

	// ��������ʽ�ĵ��ýӿ�
	virtual bool operator()(const EventArgs& args) = 0;
};


// ��Ա������
template<typename T>
class MemberFunction : public FunctorBase
{
public:
	// ����ָ�붨��
	typedef bool(T::*MemberFunctionType)(const EventArgs& args);

	// ��ʼ���ṹ���ѳ�Ա�����Ͷ�����й���
	MemberFunction(MemberFunctionType func, T* obj) :
		mFunction(func),
		mObject(obj)
	{
	}

	// �Ժ�������ʽ���ó�Ա����
	virtual bool operator()(const EventArgs& args)
	{
		return (mObject->*mFunction)(args);
	}

private:
	MemberFunctionType mFunction;
	T* mObject;
};

// ����ָ��
template<typename T>

class FunctorPointer : public FunctorBase
{
public:
	FunctorPointer(T* functor) :
		d_functor(functor)
	{}

	virtual bool operator()(const EventArgs& args)
	{
		return (*d_functor)(args);
	}

private:
	T* d_functor;
};

// ��Ϣ�ַ�
class Subscriber
{
public:
	Subscriber():d_functor_impl(0){}
	virtual ~Subscriber(){ delete d_functor_impl; }

	bool operator()(const EventArgs& args)
	{
		return (*d_functor_impl)(args);
	}

	bool connected() const
	{
		return d_functor_impl != 0;
	}

	// �󶨳�Ա����
	template<typename T>
	Subscriber(bool (T::*function)(const EventArgs& args), T* obj) :
		d_functor_impl(new MemberFunction<T>(function, obj))
	{}

	// ��ȫ�ֺ���
	template<typename T>
	Subscriber(T* functor) :
		d_functor_impl(new FunctorPointer<T>(functor))
	{}

private:
	FunctorBase* d_functor_impl;
};




#endif