#include "Mutex.h"

Mutex::Mutex()
{
	ms = INFINITE;
	myMutex = CreateMutex(nullptr, false, L"uniqueName");
}

void Mutex::lock()
{
	WaitForSingleObject(myMutex, ms); // LOCK
}

void Mutex::unlock()
{
	ReleaseMutex(myMutex); // UNLOCK

}
