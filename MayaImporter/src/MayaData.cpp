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
			memcpy(mesh, (message + offset), sizeof(HeaderTypeMesh));
			offset += sizeof(HeaderTypeMesh);
			memcpy(this->vertexArray, (message + offset), sizeof(Vertex) *  mesh->vertexCount);
			mesh->vertexArray = this->vertexArray;
			return true;

		}
		else if (messageType == MessageType::MayaCamera)
		{
			//memcpy(cam, (message + offset), sizeof(HeaderTypeCamera));
			cam = (HeaderTypeCamera*)(message + offset);
			return true;

		}
		else if (messageType == MessageType::MayaTransform)
		{
			//memcpy(transform, (message + offset), sizeof(HeaderTypeTransform));
			transform = (HeaderTypeTransform*)(message + offset);
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
	int messageSize = (1 << 20) / 4;
	this->message = new char[messageSize];
	this->vertexArray = new Vertex[messageSize];
	this->circBuffer = new CircBufferFixed(L"buff", false, 1 << 20, 256);
	//this->cam = new HeaderTypeCamera;
	this->mesh = new HeaderTypeMesh;
	//this->transform = new HeaderTypeTransform;
}

MayaData::~MayaData()
{
	delete[] this->message;
	delete[] this-> vertexArray;
	delete   this->circBuffer;
	//delete   this->cam;
	delete   this->mesh;
	//delete   this->transform;
}