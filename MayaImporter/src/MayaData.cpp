#include "MayaData.h"

void MayaData::readMayaData()
{
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