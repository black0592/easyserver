#ifndef __EVENT_TEMPLATE_H__
#define __EVENT_TEMPLATE_H__


// 事件基类
class EventArgs
{
public:
	EventArgs() {}
	virtual ~EventArgs() {}
};

// 处理函数的基类
class FunctorBase
{
public:
	virtual ~FunctorBase() {};

	// 函数对象方式的调用接口
	virtual bool operator()(const EventArgs& args) = 0;
};


// 成员函数绑定
template<typename T>
class MemberFunction : public FunctorBase
{
public:
	// 函数指针定义
	typedef bool(T::*MemberFunctionType)(const EventArgs& args);

	// 初始化结构，把成员函数和对象进行关联
	MemberFunction(MemberFunctionType func, T* obj) :
		mFunction(func),
		mObject(obj)
	{
	}

	// 以函数对象方式调用成员函数
	virtual bool operator()(const EventArgs& args)
	{
		return (mObject->*mFunction)(args);
	}

private:
	MemberFunctionType mFunction;
	T* mObject;
};

// 函数指针
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

// 消息分发
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

	// 绑定成员函数
	template<typename T>
	Subscriber(bool (T::*function)(const EventArgs& args), T* obj) :
		d_functor_impl(new MemberFunction<T>(function, obj))
	{}

	// 绑定全局函数
	template<typename T>
	Subscriber(T* functor) :
		d_functor_impl(new FunctorPointer<T>(functor))
	{}

private:
	FunctorBase* d_functor_impl;
};




#endif