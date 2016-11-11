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
			mesh = (HeaderTypeMesh*)(message + offset);
			offset += sizeof(HeaderTypeMesh);
			memcpy(this->vertexArray, (message + offset), sizeof(Vertex) *  mesh->vertexCount);
			mesh->vertexArray = this->vertexArray;
			return true;

		}
		else if (messageType == MessageType::MayaCamera)
		{
			cam = (HeaderTypeCamera*)(message + offset);
			return true;

		}
		else if (messageType == MessageType::MayaTransform)
		{
			transform = (HeaderTypeTransform*)(message + offset);
			return true;
		}
		else if(messageType == MessageType::MayaMaterial)
		{
			material = (HeaderTypeMaterial*)(message + offset);
			offset += sizeof(HeaderTypeMaterial);

			memcpy(meshName, message + offset, sizeof(MeshName) * material->amountOfMeshes);
			meshName->meshName;
			sizeof(MeshName) * material->amountOfMeshes;
			return true;

		}


	}
	else
		return false;
}




MayaData::MayaData()
{
	int messageSize = (1 << 20) / 4;
	this->message = new char[messageSize];
	this->vertexArray = new Vertex[messageSize];
	this->circBuffer = new CircBufferFixed(L"buff", false, 1 << 20, 256);
	this->meshName = new MeshName[1000];
}

MayaData::~MayaData()
{
	delete[] this->message;
	delete[] this-> vertexArray;
	delete   this->circBuffer;
	delete[] this->meshName;

}