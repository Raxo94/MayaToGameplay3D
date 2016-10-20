#include "MayaData.h"


bool MayaData::read()
{
	if (circBuffer->pop(message))
	{
		HeaderType* header;
		header = (HeaderType*)message;
		this->vertexCount = header->vertexCount;

		memcpy(vertexArray, (message + sizeof(HeaderType)), sizeof(Vertex) * vertexCount);

		std::vector<Vertex> vertexVector;

		for (size_t i = 0; i < vertexCount; i++)
			{
				vertexVector.push_back(vertexArray[i]);
			}
		
		return true;
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