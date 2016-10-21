#include "MayaData.h"


bool MayaData::read()
{
	if (circBuffer->pop(message))
	{
		size_t offset = 0;

		memcpy(&messageType, message, sizeof(int)); 
		offset += sizeof(int);
		
		if (messageType == MessageType::MayaMesh)
		{
			HeaderType* header;
			header = (HeaderType*)(message + offset);
			offset += sizeof(HeaderType);
			this->vertexCount = header->vertexCount;
			this->NodeName = header->Name; //this shouldn't cause trouble if buffer works.

			memcpy(vertexArray, (message + offset), sizeof(Vertex) * vertexCount);
			return true;

		}

	}
	else
		return false;
}

Vertex * MayaData::GetVertexArray()
{
	return vertexArray;
}

unsigned int MayaData::GetVertexCount()
{
	return vertexCount;
}

char* MayaData::GetNodeName()
{
	return NodeName;
}

MayaData::MayaData()
{
	this->message = new char[5000];
	this->vertexArray = new Vertex[5000];
	this->circBuffer = new CircBufferFixed(L"buff", false, 1 << 20, 256);
}

MayaData::~MayaData()
{
	delete[] this->message;
	delete   this->circBuffer;
}