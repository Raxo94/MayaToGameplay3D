#pragma once
#include "CircBuffer.h"

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
	void readMayaData();
	MayaData();
	~MayaData();
};