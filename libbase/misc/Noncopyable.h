#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

namespace easygame {

	class Noncopyable
	{
	protected:
		Noncopyable(){}
		virtual ~Noncopyable(){}

	private:
		Noncopyable(const Noncopyable&);
		const Noncopyable& operator=(const Noncopyable&);
	};


}	// namespace

#endif