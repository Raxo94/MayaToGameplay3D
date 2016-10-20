#include "MayaData.h"

bool MayaData::read()
{
	if (circBuffer->pop(message))
	{
		HeaderType* header;
		header = (HeaderType*)message;
		return true;
	}
	else
		return false;
}

MayaData::MayaData()
{
	this->message = new char[5000];
	this->circBuffer = new CircBufferFixed(L"buff", false, 1 << 20, 256);
}

MayaData::~MayaData()
{
	delete[] this->message;
	delete   this->circBuffer;
}