#pragma once
#include <windows.h>
class Mutex
{
private:
	HANDLE myMutex;
	DWORD ms;
public:
	Mutex();

	void lock();
	void unlock();
};