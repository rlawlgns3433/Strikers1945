#pragma once

template <typename T>
class Singleton
{
protected :
	Singleton()						= default;
	Singleton(const Singleton& ref) = delete;
	Singleton(Singleton&& ref)		= delete;
	virtual ~Singleton()			= default;

	Singleton& operator=(const Singleton& ref) = delete;
	Singleton& operator=(Singleton&& ref)	   = delete;

public :
	static T& Instance()
	{
		static T instance; // static 지역 변수 : 함수 당 하나의 지역 변수를 갖는다.

		return instance;
	}
};