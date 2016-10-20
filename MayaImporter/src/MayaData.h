#pragma once
#include "CircBuffer.h"
#include <iostream>

using namespace std;

struct HeaderType
{
	char messageType[256];
	size_t vertexArray;
	size_t vertexCount;
};

struct Vertex
{
	float pos[3];
	float norm[3];
	float UV[2];
};

class MayaData
{
private:
	char Name[256];
	Vertex* vertexArray;
	char* message;
	CircBufferFixed* circBuffer;
public:
	bool read();
	MayaData();
	~MayaData();
};